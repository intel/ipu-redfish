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
 * @file configuration.cpp
 *
 * @brief Application configuration implementation
 * */

#include "configuration/configuration.hpp"
#include "logger/logger_factory.hpp"

#include <fstream>
#include <stdexcept>
#include <string>

using namespace configuration;

static Configuration* g_configuration = nullptr;

const json::Json& Configuration::to_json() {
    if (m_configuration_file.empty()) {
        log_error("configuration", "No configuration file added");
        return m_json_configuration;
    }
    try {
        std::ifstream configuration_file(m_configuration_file);
        m_json_configuration = json::Json::parse(configuration_file);
    }
    catch (const std::exception& e) {
        log_error("configuration", e.what());
        std::exit(-1);
    }

    return m_json_configuration;
}

void Configuration::add_file(const std::string& file_name) {
    m_configuration_file = file_name;
    log_debug("configuration", "Added file " << file_name);
}

Configuration& Configuration::get_instance() {
    if (nullptr == g_configuration) {
        g_configuration = new Configuration();
    }
    return *g_configuration;
}

void Configuration::cleanup() {
    log_debug("configuration", "Cleanup");
    delete g_configuration;
    g_configuration = nullptr;
}
