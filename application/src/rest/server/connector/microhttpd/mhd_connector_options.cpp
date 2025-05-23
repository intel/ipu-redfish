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
 * @file mhd_connector_options.cpp
 *
 * */

#include "psme/rest/server/connector/microhttpd/mhd_connector_options.hpp"
#include "psme/rest/server/certs/cert_manager.hpp"

#include <microhttpd.h>

#include "logger/logger_factory.hpp"
#include "net/network_interface.hpp"
#include "net/socket_address.hpp"
#include <thread>

using namespace psme::rest::server;

class MHDConnectorOptionsImpl {
public:
    MHDConnectorOptionsImpl(const ConnectorOptions& options) {
        init_flags_and_options(options);
    }

    unsigned int get_flags() const {
        return m_flags;
    }

    MHD_OptionItem* get_options_array() {
        return m_option_array.data();
    }
private:
    unsigned int m_flags{0};
    std::vector<MHD_OptionItem> m_option_array{};
    net::SocketAddress m_socket_address{};

    void init_flags_and_options(const ConnectorOptions& options) {

        init_socket_address(options);

        init_threading_mode(options);

        init_ssl_options(options);

        init_debug_options(options);

        // The array must be terminated with an entry `{MHD_OPTION_END, 0, NULL}`
        m_option_array.emplace_back(MHD_OptionItem{MHD_OPTION_END, 0, nullptr});
    }

    void init_threading_mode(const ConnectorOptions& options) {
        switch (options.get_thread_mode()) {
        case ConnectorOptions::ThreadMode::THREAD_PER_CONNECTION:
            m_flags |= MHD_USE_THREAD_PER_CONNECTION;
            break;
        case ConnectorOptions::ThreadMode::SELECT: {
            m_flags |= MHD_USE_SELECT_INTERNALLY;
            auto thread_pool_size = options.get_thread_pool_size();
            if (0 == thread_pool_size) {
                thread_pool_size = std::max(std::thread::hardware_concurrency(), 1u);
            }
            log_debug("rest", "connector on port " << options.get_port()
                                                   << " thread_pool_size: " << thread_pool_size);
            m_option_array.emplace_back(MHD_OptionItem{
                MHD_OPTION_THREAD_POOL_SIZE,
                static_cast<intptr_t>(thread_pool_size), nullptr});
        } break;
        default:
            break;
        }
    }

    void init_ssl_options(const ConnectorOptions& options) {
        m_flags |= MHD_USE_SSL;
        auto* cert_manager = psme::rest::server::CertManager::get_instance();

        cert_manager->load_certs(options.get_port(),
                                 options.is_client_cert_required(),
                                 options.get_certs_dir());
        const auto port = options.get_port();

        m_option_array.emplace_back(MHD_OptionItem{
            MHD_OPTION_HTTPS_MEM_KEY, intptr_t(0),
            static_cast<void*>(const_cast<char*>(cert_manager->get_server_key(port)))});
        m_option_array.emplace_back(MHD_OptionItem{
            MHD_OPTION_HTTPS_MEM_CERT, intptr_t(0),
            static_cast<void*>(const_cast<char*>(cert_manager->get_server_cert(port)))});
        if (options.is_client_cert_required()) {
            m_option_array.emplace_back(MHD_OptionItem{
                MHD_OPTION_HTTPS_MEM_TRUST, intptr_t(0),
                static_cast<void*>(const_cast<char*>(cert_manager->get_ca_cert(port)))});
        }
    }

    void init_socket_address(const ConnectorOptions& options) {
        const auto& iface_name = options.get_network_interface_name();
        if (!iface_name.has_value()) {
            log_info("rest", "Starting MHD connector listening on 0.0.0.0");
            return;
        }
        auto iface = net::NetworkInterface::for_name(iface_name);
        const auto& ip_address = iface.get_first_address(net::AddressFamily::IPv4);
        m_socket_address = net::SocketAddress(ip_address, options.get_port());
        m_option_array.emplace_back(MHD_OptionItem{
            MHD_OPTION_SOCK_ADDR, intptr_t(0),
            static_cast<void*>(const_cast<struct sockaddr*>(m_socket_address.addr()))});
        log_info("rest", "Starting MHD connector listening on " << m_socket_address
                                                                << " (" << options.get_network_interface_name() << ")");
    }

    void init_debug_options(const ConnectorOptions& options) {
        if (options.use_debug()) {
            m_flags |= MHD_USE_DEBUG;
        }
    }
};

MHDConnectorOptions::MHDConnectorOptions(const ConnectorOptions& options)
    : m_options_impl(new MHDConnectorOptionsImpl(options)) {}

unsigned int MHDConnectorOptions::get_flags() const {
    return m_options_impl->get_flags();
}

MHD_OptionItem* MHDConnectorOptions::get_options_array() {
    return m_options_impl->get_options_array();
}
