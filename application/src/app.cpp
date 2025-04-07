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

#include "app.hpp"
#include "agent-framework/logger_loader.hpp"
#include "agent-framework/module/service_uuid.hpp"
#include "configuration/configuration.hpp"
#include "configuration/configuration_schema.hpp"
#include "context.hpp"
#include "database/database.hpp"
#include "psme/rest/registries/config/base_configuration.hpp"
#include "psme/rest/registries/config/registry_configurator.hpp"
#include "psme/rest/security/session/session_service.hpp"
#include "psme/ssdp/ssdp_config_loader.hpp"
#include "ssdp/ssdp_service.hpp"
#include "version.hpp"

#include <csignal>
#include <iostream>
#include <string>

#include <gcrypt.h>
#include <gnutls/gnutls.h>
#include <unistd.h>

using namespace psme::app;
using namespace configuration;
namespace fs = std::filesystem;

App::App(const char* config_path) { init(config_path); }

App::~App() { cleanup(); }

void App::run() {
    try {
        if (m_network_change_notifier) {
            m_network_change_notifier->start();
        }
        m_rest_session_service->start();
        m_rest_server->start();
        m_ssdp_service->start();

        wait_for_termination();
        log_info(LOGUSR, "Stopping Redfish Server");
    }
    catch (std::exception& e) {
        log_error("app", e.what());
    }
    catch (...) {
        log_error("app", "Unknown exception.");
    }
}

void App::init(const char* config_path) {
    std::cout << "Redfish server version " << psme::app::VERSION << std::endl;
    try {
        check_permissions();
        load_configuration(config_path);
        init_logger();
        init_database();
        agent_framework::module::ServiceUuid::get_instance();
        init_network_change_notifier();
        init_ssdp_service();
        Context::get_instance()->initialize();
        init_rest_session_service();
        init_registries();
        init_rest_server();
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to initialize Application: " << e.what() << std::endl;
        cleanup();
        exit(-1);
    }
    catch (...) {
        std::cerr << "Failed to initialize Application: Unknown exception." << std::endl;
        cleanup();
        exit(-1);
    }
}

void App::load_configuration(const char* config_path) {
    auto& config = Configuration::get_instance();

    /* Initialize configuration */

    config.add_file(config_path);

    const auto& json_config = Configuration::get_instance().to_json();

    nlohmann::json_schema::json_validator validator;
    try {
        validator.set_root_schema(psme::app::DEFAULT_VALIDATOR_JSON);
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("JSON schema incorrect. ") + e.what());
    }

    try {
        validator.validate(json_config);
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Incorrect configuration. ") + e.what());
    }
}

void App::init_database() {
    const auto& json_config = Configuration::get_instance().to_json();
    database::Database::set_default_location(json_config["database"]["location"]);
}

void App::init_logger() {
    const auto& json_config = Configuration::get_instance().to_json();
    logger_cpp::LoggerLoader loader(json_config);
    loader.load(logger_cpp::LoggerFactory::instance());
}

void App::init_network_change_notifier() {
    try {
        m_network_change_notifier = net::NetworkChangeNotifier::create();
    }
    catch (const std::exception&) {
        log_error("app", "Failed to initialize NetworkChangeNotifier.");
    }
}

void App::init_ssdp_service() {
    const auto& json_config = Configuration::get_instance().to_json();
    const auto& service_uuid = agent_framework::module::ServiceUuid::get_instance()->get_service_uuid();
    m_ssdp_service = std::make_shared<ssdp::SsdpService>(ssdp::load_ssdp_config(json_config, service_uuid));
    if (m_network_change_notifier) {
        m_network_change_notifier->add_listener(m_ssdp_service);
    }
}

void App::init_rest_server() {
    m_rest_server = std::make_unique<psme::rest::server::RestServer>();
}

void App::init_rest_session_service() {
    m_rest_session_service = std::make_unique<psme::rest::security::session::SessionService>();
}

void App::init_registries() {
    const std::string& base_configuration = rest::registries::make_base_configuration();
    rest::registries::RegistryConfigurator::get_instance()->load(base_configuration);
}

void App::cleanup() {
    if (m_rest_session_service) {
        m_rest_session_service->stop();
        m_rest_session_service.reset();
    }
    if (m_rest_server) {
        m_rest_server->stop();
        m_rest_server.reset();
    }
    if (m_ssdp_service) {
        m_ssdp_service->stop();
        m_ssdp_service.reset();
    }
    if (m_network_change_notifier) {
        m_network_change_notifier->stop();
        m_network_change_notifier.reset();
    }
    Configuration::cleanup();
}

void App::check_permissions() {
#ifdef INTEL_IPU
    if (getuid() != 0) {
        throw std::runtime_error("This application must be run as root");
    }
    check_folder_permissions("/work/redfish");
    check_folder_permissions("/work/redfish/certs");
    if (!gnutls_fips140_mode_enabled()) {
        throw std::runtime_error("FIPS mode is not enabled. Make sure to run the IPU Redfish server as a service!");
    }
#endif
    if (!gcry_check_version(nullptr)) {
        throw std::runtime_error("libgcrypt initialization failed");
    }
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
}

void App::check_folder_permissions(const std::string& dir_path) {
    if (!fs::exists(dir_path)) {
        throw std::runtime_error("The directory " + dir_path + " does not exist");
    }

    fs::perms perm = fs::status(dir_path).permissions();
    if ((perm & fs::perms::owner_all) != fs::perms::owner_all) {
        throw std::runtime_error("The required owner permissions are not set for " + dir_path);
    }
    if ((perm & (fs::perms::group_all | fs::perms::others_all)) != fs::perms::none) {
        throw std::runtime_error("The permissions for " + dir_path + " are too open!");
    }
}

void App::wait_for_termination() {
    log_debug(LOGUSR, "Waiting for termination signal...");

    sigset_t sset;
    sigemptyset(&sset);
    sigaddset(&sset, SIGINT);
    sigaddset(&sset, SIGQUIT);
    sigaddset(&sset, SIGTERM);
    sigprocmask(SIG_BLOCK, &sset, NULL);
    int sig;
    sigwait(&sset, &sig);
}
