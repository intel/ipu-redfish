/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2015-2019 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * */

#include "psme/rest/server/connector/microhttpd/mhd_connector.hpp"
#include "psme/rest/server/connector/microhttpd/mhd_connector_options.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/server/http_headers.hpp"
#include "psme/rest/server/status.hpp"
#include "psme/rest/server/utils.hpp"
#include <cstring>
#include <sstream>

#include <microhttpd.h>

using namespace psme::rest;
using namespace psme::rest::server;
using namespace psme::rest::security::authentication;
using namespace psme::rest::error;

namespace {

using MHDResponsePtr = std::unique_ptr<MHD_Response, decltype(&MHD_destroy_response)>;

MHDResponsePtr create_response(Response& response) {
    return MHDResponsePtr{
        MHD_create_response_from_buffer(
            response.get_body_size(),
            const_cast<char*>(response.get_body().c_str()),
            MHD_RESPMEM_MUST_COPY),
        &MHD_destroy_response};
}

void add_response_headers(MHD_Response* res, const Response& resp) {
    for (const auto& header : resp.get_headers()) {
        auto ret = MHD_add_response_header(res,
                                           header.first.c_str(),
                                           const_cast<char*>(header.second.c_str()));
        if (ret != MHD_YES) {
            log_error("rest", "failed to add response header " << header.first);
        }
    }
}

MHD_Result send_response(MHD_Connection* con, /*const*/ Response& res) {
    if (auto r = create_response(res)) {
        add_response_headers(r.get(), res);
        return MHD_queue_response(con, res.get_status(), r.get());
    }

    log_error("rest", "Cannot create response\n");
    return MHD_NO;
}

MHD_Result add_request_headers(void* cls, enum MHD_ValueKind /*kind*/,
                               const char* key, const char* value) {
    Request* request = static_cast<Request*>(cls);
    request->set_header(key, value);

    return MHD_YES;
}

Method get_request_method(const char* method) {
    try {
        return Method::from_string(method);
    }
    catch (...) {
        return Method::UNKNOWN;
    }
}

/* microhttpd's MHD_AccessHandlerCallback */
MHD_Result access_handler_callback(void* cls, struct MHD_Connection* connection,
                                   const char* url, const char* method, const char* version,
                                   const char* upload_data, size_t* upload_data_size, void** con_cls) {
    try {
        log_debug("rest", "HTTP Method " << method);

        const std::string odata_version = "OData-Version";
        const std::string odata_version_4_0 = "4.0";

        const char* odata_version_str = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, odata_version.c_str());
        if (odata_version_str && std::string(odata_version_str) != odata_version_4_0) {
            log_error("rest", "Incorrect OData-Version header in request: " << odata_version_str);
            Response response;
            response.set_status(server::status_4XX::PRECONDITION_FAILED);
            return send_response(connection, response);
        }

        auto* connector = static_cast<MHDConnector*>(cls);

        auto deleter = [&con_cls](Request* r) {
            delete r;
            *con_cls = nullptr;
        };
        std::unique_ptr<Request, decltype(deleter)> request(static_cast<Request*>(*con_cls), deleter);

        if (!request) {
            request.reset(new Request());
            request->set_destination(url);
            request->set_HTTP_version(version);
            request->set_method(get_request_method(method));
            *con_cls = request.release();
            return MHD_YES;
        }

        if (0 != *upload_data_size) {
            request->append_body(std::string{upload_data, *upload_data_size});
            *upload_data_size = 0;
            *con_cls = request.release();
            return MHD_YES;
        }

        if (connector->get_options().is_client_cert_required()) {
            Response response;
            if (connector->client_cert_authenticate(connection, url, response) == AuthStatus::FAIL) {
                return send_response(connection, response);
            }
        }
        if (!connector->unauthenticated_access_feasible(method, url) &&
            connector->is_authentication_enabled()) {
            Response response;
            auto status = connector->authenticate(connection, url, response);
            if (status == AuthStatus::FAIL) {
                return send_response(connection, response);
            }
        }

        const uint16_t MAX_URI_SIZE = 256;
        if (request->get_url().size() > MAX_URI_SIZE) {
            Response response;
            connector->prepare_uri_too_long_response(*request, response);
            return send_response(connection, response);
        }

        const uint16_t MAX_INCOME_MSG_SIZE = 1024;
        if (request->get_body().size() > MAX_INCOME_MSG_SIZE) {
            Response response;
            connector->prepare_payload_too_large_response(response);
            return send_response(connection, response);
        }

        MHD_get_connection_values(connection, MHD_HEADER_KIND,
                                  &add_request_headers, request.get());

        Response response;
        response.set_header("Cache-Control", "no-cache");
        response.set_header(odata_version, odata_version_4_0);

        connector->handle(*request, response);

        return send_response(connection, response);
    }
    catch (...) {
        log_error("rest", "Unexpected exception in access_handler_callback");
        return MHD_NO;
    }
}

} // namespace

MHDConnector::MHDConnector(const ConnectorOptions& options)
    : Connector(options), m_daemon{nullptr, &MHD_stop_daemon} {}

MHDConnector::~MHDConnector() {
    MHDConnector::stop();
}

void MHDConnector::start() {
    if (!m_daemon) {
        const auto port = get_options().get_port();
        MHDConnectorOptions options(get_options());
        m_daemon.reset(MHD_start_daemon(options.get_flags(),
                                        port,
                                        nullptr, nullptr,
                                        access_handler_callback, this,
                                        MHD_OPTION_ARRAY, options.get_options_array(),
                                        MHD_OPTION_END));

        if (!m_daemon) {
            std::stringstream str;
            str << "Cannot start connector on port " << port;
            if (errno) {
                str << ": " << strerror(errno);
            }
            throw std::runtime_error(str.str());
        }

        log_info("rest", "HTTPS connector started on port: " << port);
    }
}

void MHDConnector::stop() {
    if (m_daemon) {
        m_daemon.reset();
        log_info("rest", "HTTPS connector on port: " << get_options().get_port() << " stopped\n");
    }
}
