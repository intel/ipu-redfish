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
#include "psme/rest/eventing/event_service.hpp"
#include "psme/rest/eventing/manager/subscription_manager.hpp"
#include "psme/rest/registries/config/base_configuration.hpp"
#include "psme/rest/registries/config/registry_configurator.hpp"
#include "psme/rest/security/session/session_service.hpp"
#include "psme/ssdp/ssdp_config_loader.hpp"
#include "ssdp/ssdp_service.hpp"
#include "version.hpp"

#include <csignal>
#include <iostream>
#include <string>

namespace {
const json::Json& load_configuration(int argc, const char** argv) {
    log_info("app", "Redfish server version " << psme::app::VERSION);

    /* Initialize configuration */
    auto& config = configuration::Configuration::get_instance();

    if (argc < 2) {
        log_error("app", "No configuration file provided.");
        exit(-1);
    }
    config.add_file(argv[1]);
    const json::Json& json_configuration = config.to_json();

    nlohmann::json_schema::json_validator validator;
    try {
        validator.set_root_schema(psme::app::DEFAULT_VALIDATOR_JSON);
    }
    catch (const std::exception& e) {
        log_error("app", "JSON schema incorrect.");
        log_error("app", e.what());
        exit(-1);
    }

    try {
        validator.validate(json_configuration);
    }
    catch (const std::exception& e) {
        log_error("app", "Incorrect configuration.");
        log_error("app", e.what());
        exit(-1);
    }

    return json_configuration;
}

} // namespace

using namespace psme::app;

App::App(int argc, const char* argv[]) try : m_configuration(load_configuration(argc, argv)) {
    init();
}
catch (std::exception& e) {
    std::cerr << "\nFailed to initialize Application: " << e.what() << "\n";
    exit(-1);
}

App::~App() { cleanup(); }

void App::init_database() {
    if (m_configuration.value("database", json::Json::object()).value("location", json::Json()).is_string()) {
        database::Database::set_default_location(m_configuration["database"]["location"].get<std::string>());
    }
}

void App::init_logger() {
    logger_cpp::LoggerLoader loader(m_configuration);
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
    const auto& service_uuid = agent_framework::module::ServiceUuid::get_instance()->get_service_uuid();
    m_ssdp_service.reset(new ssdp::SsdpService{ssdp::load_ssdp_config(m_configuration, service_uuid)});
    if (m_network_change_notifier) {
        m_network_change_notifier->add_listener(m_ssdp_service);
    }
}

void App::init_rest_server() {
    using psme::rest::server::RestServer;
    m_rest_server.reset(new RestServer);
}

void App::init_rest_event_service() {
    using psme::rest::eventing::EventService;
    m_rest_event_service.reset(new EventService());
}

void App::init_rest_session_service() {
    using psme::rest::security::session::SessionService;
    m_rest_session_service.reset(new SessionService());
}

void App::init_registries() {
    using namespace rest::registries;
    const std::string& base_configuration{make_base_configuration()};
    RegistryConfigurator::get_instance()->load(base_configuration);
}

void App::init() {
    try {
        init_database();
        init_logger();
        agent_framework::module::ServiceUuid::get_instance();
        init_network_change_notifier();
        init_ssdp_service();
        Context::get_instance()->initialize();
        init_rest_event_service();
        init_rest_session_service();
        psme::rest::eventing::manager::SubscriptionManager::get_instance();
        init_registries();
        init_rest_server();
    }
    catch (const std::exception& e) {
        log_error("app",
                  "Failed to initialize Application: " << e.what());
        cleanup();
        exit(-1);
    }
    catch (...) {
        log_error("app",
                  "Failed to initialize Application: Unknown exception.");
        cleanup();
        exit(-1);
    }
}

void App::run() {
    try {
        if (m_network_change_notifier) {
            m_network_change_notifier->start();
        }
        m_rest_event_service->start();
        m_rest_session_service->start();
        m_rest_server->start();
        m_ssdp_service->start();

        wait_for_termination();
        log_info(LOGUSR, "Stopping PSME Application...");
    }
    catch (std::exception& e) {
        log_error("app", e.what());
    }
    catch (...) {
        log_error("app", "Unknown exception.");
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

void App::statics_cleanup() {
    psme::rest::eventing::manager::SubscriptionManager::get_instance()->clean_up();
    configuration::Configuration::cleanup();
    logger_cpp::LoggerFactory::cleanup();
}

void App::cleanup() {
    if (m_rest_event_service) {
        m_rest_event_service->stop();
        m_rest_event_service.reset();
    }
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
    statics_cleanup();
}
