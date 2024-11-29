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

#include "psme/rest/server/connector/connector_options.hpp"

using namespace psme::rest::server;

constexpr const char ConnectorOptions::NETWORK_INTERFACE_NAME[];
constexpr const char ConnectorOptions::PORT[];
constexpr const char ConnectorOptions::CERTS_DIR[];
constexpr const char ConnectorOptions::CLIENT_CERT_REQUIRED[];
constexpr const char ConnectorOptions::HOSTNAME[];
constexpr const char ConnectorOptions::THREAD_MODE[];
constexpr const char ConnectorOptions::THREAD_MODE_SELECT[];
constexpr const char ConnectorOptions::THREAD_MODE_THREAD_PER_CONNECTION[];
constexpr const char ConnectorOptions::AUTHENTICATION_TYPE[];
constexpr const char ConnectorOptions::AUTHENTICATION_TYPE_NONE[];
constexpr const char ConnectorOptions::AUTHENTICATION_TYPE_BASIC[];
constexpr const char ConnectorOptions::AUTHENTICATION_TYPE_REDFISH_SESSION[];
constexpr const char ConnectorOptions::AUTHENTICATION_TYPE_BASIC_OR_SESSION[];
constexpr const char ConnectorOptions::THREAD_POOL_SIZE[];
constexpr const char ConnectorOptions::DEBUG_MODE[];

ConnectorOptions::ConnectorOptions(const json::Json& config) {
    m_network_interface_name = config.value(NETWORK_INTERFACE_NAME, std::string{});
    m_port = config.value(PORT, std::uint16_t{});
    const auto& thread_mode = config.value(THREAD_MODE, std::string{});
    if (thread_mode == THREAD_MODE_SELECT) {
        m_thread_mode = ThreadMode::SELECT;
    } else if (thread_mode == THREAD_MODE_THREAD_PER_CONNECTION) {
        m_thread_mode = ThreadMode::THREAD_PER_CONNECTION;
    }
    const auto& auth_type = config.value(AUTHENTICATION_TYPE, std::string{});
    if (auth_type == AUTHENTICATION_TYPE_BASIC) {
        m_authentication_type = AuthenticationType::BASIC_AUTH;
    } else if (auth_type == AUTHENTICATION_TYPE_REDFISH_SESSION) {
        m_authentication_type = AuthenticationType::REDFISH_SESSION_AUTH;
    } else if (auth_type == AUTHENTICATION_TYPE_BASIC_OR_SESSION) {
        m_authentication_type = AuthenticationType::BASIC_AUTH_OR_REDFISH_SESSION_AUTH;
    } else if (auth_type == AUTHENTICATION_TYPE_NONE) {
        m_authentication_type = AuthenticationType::NONE;
    }
    if (config.count(DEBUG_MODE)) {
        m_use_debug = config.value(DEBUG_MODE, bool{});
    }
    if (config.count(THREAD_POOL_SIZE)) {
        m_thread_pool_size = config.value(THREAD_POOL_SIZE, std::uint16_t{});
    }
    m_certs_dir = config.value(CERTS_DIR, std::string{});
    m_is_client_cert_required = true;
    if (config.count(CLIENT_CERT_REQUIRED)) {
        m_is_client_cert_required = config.value(CLIENT_CERT_REQUIRED, bool{});
    }
    if (config.count(HOSTNAME)) {
        m_hostname = config.value(HOSTNAME, std::string{});
    }
}

const std::string& ConnectorOptions::get_certs_dir() const {
    return m_certs_dir;
}

uint16_t ConnectorOptions::get_port() const {
    return m_port;
}

bool ConnectorOptions::is_client_cert_required() const {
    return m_is_client_cert_required;
}

const std::string& ConnectorOptions::get_hostname() const {
    return m_hostname;
}

ConnectorOptions::ThreadMode ConnectorOptions::get_thread_mode() const {
    return m_thread_mode;
}

ConnectorOptions::AuthenticationType ConnectorOptions::get_authentication_type() const {
    return m_authentication_type;
}

unsigned int ConnectorOptions::get_thread_pool_size() const {
    return m_thread_pool_size;
}

bool ConnectorOptions::use_debug() const {
    return m_use_debug;
}

const std::string& ConnectorOptions::get_network_interface_name() const {
    return m_network_interface_name;
}
