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
 *
 * @file app.hpp
 *
 * @brief Declares App class wrapping main program logic.
 * */

#pragma once
#include "net/network_change_notifier.hpp"
#include "psme/rest/rest_server.hpp"
#include "psme/rest/security/session/session_service.hpp"

#include <memory>

namespace ssdp {
class SsdpService;
}

namespace psme {
namespace app {

/*!
 * @brief Application class wrapping main program logic.
 *
 * Responsible for application initialization and execution.
 */
class App final {
public:
    /*!
     * @brief Constructor
     * @param config_path: path to the json config file
     */
    App(const char* config_path);

    /*!
     * @brief Destructor
     *
     * Responsible for resource cleanup. Stops any running servers.
     */
    ~App();

    /*!
     * @brief Starts servers initialized in init method
     * and waits for interrupt signal.
     */
    void run();
private:
    /*!
     * @brief Initialization method
     *
     * Responsible for initialization of loggers, registration server
     * and rest server. Does not start the mentioned servers.
     */
    void init(const char* config_path);
    void load_configuration(const char* config_path);
    void init_database();
    void init_logger();
    void init_network_change_notifier();
    void init_ssdp_service();
    void init_rest_server();
    void init_rest_session_service();
    void init_registries();
    void cleanup();
    void check_permissions();
    void check_folder_permissions(const std::string& dir_path);
    void wait_for_termination();

    std::unique_ptr<psme::rest::server::RestServer> m_rest_server{};
    std::unique_ptr<psme::rest::security::session::SessionService> m_rest_session_service{};
    std::unique_ptr<net::NetworkChangeNotifier> m_network_change_notifier{};
    std::shared_ptr<ssdp::SsdpService> m_ssdp_service{};
};

} // namespace app
} // namespace psme
