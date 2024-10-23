/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @section LICENSE
 *
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
 * @section DESCRIPTION
 *
 * @file rest_client.cpp
 *
 * @brief REST client using curl
 * */

#include "psme/rest/eventing/rest_client.hpp"

#include <cstring>
#include <curl/curl.h>
#include <map>
#include <memory>
#include <string>

using namespace psme::rest::eventing;

namespace {

static inline std::string& ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(
                           s.begin(), s.end(),
                           std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

static inline std::string& rtrim(std::string& s) {
    s.erase(
        std::find_if(
            s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace)))
            .base(),
        s.end());
    return s;
}

static inline std::string& trim(std::string& s) {
    return ltrim(rtrim(s));
}

size_t header_cb(void* data, size_t size, size_t nitems, void* userdata) {
    psme::rest::eventing::RestClient::Response* r =
        reinterpret_cast<psme::rest::eventing::RestClient::Response*>(userdata);
    std::string header(reinterpret_cast<char*>(data), size * nitems);
    trim(header);

    if (!header.empty()) {
        size_t pos = header.find_first_of(":");
        if (pos == std::string::npos) {
            /* no value */
            r->set_header(header, "1");
        } else {
            std::string k = header.substr(0, pos);
            std::string v = header.substr(pos + 1);
            r->set_header(rtrim(k), ltrim(v));
        }
    }

    return size * nitems;
}

size_t write_cb(void* data, size_t size, size_t nitems, void* userdata) {
    psme::rest::eventing::RestClient::Response* r =
        reinterpret_cast<psme::rest::eventing::RestClient::Response*>(userdata);
    r->append_body(reinterpret_cast<char*>(data), size * nitems);
    return size * nitems;
}

size_t read_cb(void* data, size_t size, size_t nitems, void* userdata) {
    return reinterpret_cast<psme::rest::eventing::RestClient::Upload*>(
               userdata)
        ->copy_out(data, size * nitems);
}

} // namespace

void RestClient::set_basic_auth(const std::string& user, const std::string& password) {
    m_basic_auth.clear();
    m_basic_auth += user;
    m_basic_auth += ":";
    m_basic_auth += password;
}

void RestClient::clear_basic_auth() {
    m_basic_auth.clear();
}

bool RestClient::has_basic_auth() {
    return not m_basic_auth.empty();
}

void RestClient::set_default_accept(const std::string& accept) {
    m_accept = accept;
}

void RestClient::set_default_content_type(const std::string& content_type) {
    m_content_type = content_type;
}

void RestClient::set_timeout(long timeout) {
    m_timeout = timeout;
}

long RestClient::get_timeout() const {
    return m_timeout;
}

RestClient::Response RestClient::rest_method_template(Method method,
                                                      const std::string& url, const std::string& data) {
    std::string target_url = m_base_url + url;

    RestClient::Response response;
    response.set_response_code(-1);

    CURL* curl = curl_easy_init();
    if (!curl) {
        return response;
    }

    struct curl_slist* custom_headers = nullptr;
    if (!m_accept.empty()) {
        custom_headers = curl_slist_append(custom_headers,
                                           ("Accept: " + m_accept).c_str());
    }
    if (((method == Method::PUT) || (method == Method::POST)) && (!m_content_type.empty())) {
        custom_headers = curl_slist_append(custom_headers,
                                           ("Content-Type: " + m_content_type).c_str());
    }

    bool curl_set_opt_fail = false;
    auto try_curl_setopt = [&curl_set_opt_fail, curl]<typename ParamT>(CURLoption opt, ParamT param) {
        if (curl_set_opt_fail) {
            return;
        }
        auto code = curl_easy_setopt(curl, opt, param);
        if (code != CURLE_OK) {
            curl_set_opt_fail = true;
            log_warning("rest", "Curl setopt exit code "
                                    << static_cast<int>(code)
                                    << " : " << curl_easy_strerror(code));
        }
    };

    if (!m_basic_auth.empty()) {
        try_curl_setopt(CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
        try_curl_setopt(CURLOPT_USERPWD, m_basic_auth.c_str());
    }

    try_curl_setopt(CURLOPT_USERAGENT, "psme 0.0");
    try_curl_setopt(CURLOPT_URL, target_url.c_str());

    if (method == Method::POST) {
        try_curl_setopt(CURLOPT_POST, 1L);
        try_curl_setopt(CURLOPT_POSTFIELDS, data.c_str());
        try_curl_setopt(CURLOPT_POSTFIELDSIZE, data.size());
    } else if (method == Method::PUT) {
        try_curl_setopt(CURLOPT_PUT, 1L);
        try_curl_setopt(CURLOPT_UPLOAD, 1L);
        try_curl_setopt(CURLOPT_READFUNCTION, read_cb);
        RestClient::Upload upload(data);
        try_curl_setopt(CURLOPT_READDATA, &upload);
        try_curl_setopt(CURLOPT_INFILESIZE,
                        static_cast<long>(upload.remaining()));
    }

    if (target_url.find("https://") == 0) {
        try_curl_setopt(CURLOPT_SSL_VERIFYPEER, 0L);
        try_curl_setopt(CURLOPT_SSL_VERIFYHOST, 0L);
    }

    try_curl_setopt(CURLOPT_WRITEFUNCTION, write_cb);
    try_curl_setopt(CURLOPT_WRITEDATA, &response);
    try_curl_setopt(CURLOPT_HEADERFUNCTION, header_cb);
    try_curl_setopt(CURLOPT_HEADERDATA, &response);

    try_curl_setopt(CURLOPT_HTTPHEADER, custom_headers);
    try_curl_setopt(CURLOPT_TIMEOUT_MS, m_timeout);

    CURLcode res = CURLE_OK;

    if (!curl_set_opt_fail && ((res = curl_easy_perform(curl)) == CURLE_OK)) {
        long response_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        response.set_response_code(static_cast<int>(response_code));
    } else {
        curl_slist_free_all(custom_headers);
        curl_easy_cleanup(curl);
        if (!curl_set_opt_fail) {
            log_warning("rest", "Curl exit code "
                                    << static_cast<int>(res)
                                    << " : " << curl_easy_strerror(res));
        }
        throw std::runtime_error("Curl call failed.");
    }

    log_debug("rest", " send EVENT -> " << target_url << " :"
                                        << curl_easy_strerror(res)
                                        << " (code " << static_cast<int>(res) << ")");

    curl_slist_free_all(custom_headers);
    curl_easy_cleanup(curl);

    return response;
}

RestClient::Response RestClient::get(const std::string& url) {
    return rest_method_template(Method::GET, url, "");
}

RestClient::Response RestClient::post(const std::string& url,
                                      const std::string& data) {
    return rest_method_template(Method::POST, url, data);
}

RestClient::Response RestClient::put(const std::string& url,
                                     const std::string& data) {
    return rest_method_template(Method::PUT, url, data);
}

RestClient::Response RestClient::del(const std::string& url) {
    return rest_method_template(Method::DELETE, url, "");
}

size_t RestClient::Upload::copy_out(void* dst, size_t size) {
    size_t copy_len = std::min(size, remaining());
    m_data.copy(reinterpret_cast<char*>(dst), copy_len, m_pos);
    m_pos += copy_len;
    return copy_len;
}
