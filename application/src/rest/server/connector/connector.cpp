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

#include "psme/rest/server/connector/connector.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/server/error/server_error.hpp"
#include "psme/rest/server/error/server_exception.hpp"
#include "psme/rest/server/methods_handler.hpp"
#include <psme/rest/security/authentication/client_cert_authentication.hpp>

#include <chrono>

using namespace psme::rest::server;
using namespace psme::rest::security::authentication;
using namespace psme::rest::error;
using namespace psme::rest::security::authentication;

Connector::Connector(const ConnectorOptions& options) : m_options(options),
                                                        m_access_callback{[](const Request&, const Response&) { return true; }},
                                                        m_callback{http_method_not_allowed},
                                                        m_public_access_callback{[](const std::string&, const std::string&) { return false; }} {}

Connector::~Connector() {}

void Connector::set_access_callback(const AccessCallback& access_callback) {
    m_access_callback = access_callback;
}

void Connector::set_callback(const Callback& callback) {
    m_callback = callback;
}

void Connector::set_unauthenticated_access_callback(const Connector::UnauthenticatedAccessCallback& callback) {
    m_public_access_callback = callback;
}

void Connector::set_authentication(std::vector<AuthenticationUPtr> authentications) {
    m_authentication = std::move(authentications);
}

const ConnectorOptions& Connector::get_options() const {
    return m_options;
}

bool Connector::is_authentication_enabled() const {
    return !m_authentication.empty();
}

AuthStatus Connector::authenticate(MHD_Connection* connection, const std::string& url, Response& response) const {
    for (auto&& authentication : m_authentication) {
        auto status = authentication->perform(connection, url, response);
        if (status == AuthStatus::SUCCESS) {
            return status;
        }
    }
    return AuthStatus::FAIL;
}

AuthStatus
Connector::client_cert_authenticate(MHD_Connection* connection, const std::string& url, Response& response) const {
    auto client_cert_auth = ClientCertAuthentication(get_options().get_hostname());
    return client_cert_auth.perform(connection, url, response);
}

void Connector::handle(const Request& request, Response& response) {
    std::string request_text;

    auto started_at = std::chrono::high_resolution_clock::now();
    try {
        request_text = request_to_string(request);
        log_debug("rest", "\nRequest: " << request_text);
        try_handle(request, response);
    }
    catch (const agent_framework::exceptions::NotFound& ex) {
        log_error("rest", "Not found exception: " << ex.what() << request_text);
        ServerError server_error = ErrorFactory::create_error_from_gami_exception(
            agent_framework::exceptions::NotFound(ex.get_message(), request.get_url()));
        response.set_status(server_error.get_http_status_code());
        response.set_body(server_error.as_string());
    }
    catch (const agent_framework::exceptions::GamiException& ex) {
        log_error("rest", "Agent framework exception: " << ex.what() << request_text);
        ServerError server_error = ErrorFactory::create_error_from_gami_exception(ex);
        response.set_status(server_error.get_http_status_code());
        response.set_body(server_error.as_string());
    }
    catch (const ServerException& ex) {
        log_error("rest", "ServerException: " << ex.what() << request.get_body());
        const auto& error = ex.get_error();
        response.set_status(error.get_http_status_code());
        response.set_body(error.as_string());
    }
    catch (const std::exception& ex) {
        log_error("rest", "std::exception: " << ex.what() << request_text);
        ServerError internal_server_error = ErrorFactory::create_internal_error();
        response.set_status(internal_server_error.get_http_status_code());
        response.set_body(internal_server_error.as_string());
    }
    catch (...) {
        log_error("rest", "Unknown exception." << request_text);
        ServerError internal_server_error = ErrorFactory::create_internal_error();
        response.set_status(internal_server_error.get_http_status_code());
        response.set_body(internal_server_error.as_string());
    }
    auto finished_at = std::chrono::high_resolution_clock::now();
    auto duration = finished_at - started_at;

    log_debug("rest", "\nResponse: "
                          << "\n\tSTATUS: " << response.get_status()
                          << "\n\tMSG: " << response.get_body()
                          << "\n\tProcessing Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms");
}

std::string Connector::request_to_string(const Request& request) {
    std::stringstream ss{};

    ss << "\n\tURL: " << request.get_url();
    ss << "\n\tMethod: " << request.get_method().to_string();

    const std::string& body = request.get_body();

    if (body.empty()) {
        ss << "\n\tBody: <empty>";
        return ss.str();
    }

    if (body.find("\"Password\"") == std::string::npos) {
        ss << "\n\tBody: " << body;
    } else {
        json::Json json;
        try {
            json = json::Json::parse(body);
        }
        catch (const json::Json::parse_error&) {
            throw ServerException(ErrorFactory::create_malformed_json_error());
        }
        json.erase("Password");
        ss << "\n\tBody: " << json.dump();
    }

    return ss.str();
}

void Connector::try_handle(const Request& request, Response& response) {
    if (m_access_callback(request, response)) {
        m_callback(request, response);
    } else {
        auto message = "Credentials do not have sufficient privileges for the requested operation.";
        throw ServerException(ErrorFactory::create_insufficient_privilege_error(message));
    }
}

void Connector::prepare_uri_too_long_response(const Request& request, Response& response) {
    auto error = ErrorFactory::create_uri_too_long_error(request.get_url());
    response.set_status(error.get_http_status_code());
    response.set_header(http_headers::ContentType::CONTENT_TYPE, http_headers::ContentType::JSON);
    response.set_body(error.as_string());
}

void Connector::prepare_payload_too_large_response(Response& response) {
    auto error = ErrorFactory::create_payload_too_large_error();
    response.set_status(error.get_http_status_code());
    response.set_header(http_headers::ContentType::CONTENT_TYPE, http_headers::ContentType::JSON);
    response.set_body(error.as_string());
}

bool Connector::unauthenticated_access_feasible(const std::string& http_header, const std::string& url) {
    if (!m_public_access_callback) {
        log_warning("rest", "Public access callback for Connector is not set.");
        return false;
    }
    try {
        return m_public_access_callback(http_header, url);
    }
    catch (const ServerException& ex) {
        log_error("rest", "ServerException while checking public access callback in connector. " << ex.what());
    }
    catch (...) {
        log_error("rest", "Exception while checking public access callback in connector.");
    }
    return false;
}
