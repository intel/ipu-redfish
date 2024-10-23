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
 * @file configuration.hpp
 * @brief Application configuration interface
 * */

#pragma once
#include "json-wrapper/json-wrapper.hpp"

#include <string>

namespace configuration {

/*!
 * @brief Configuration singleton
 *
 * Get configuration settings from a file and provide them as JSON object
 * */
class Configuration {
public:
    /*!
     * @brief Get configuration in JSON C++ object
     *
     * @return  JSON C++ object that contain configuration settings
     * */
    const json::Json& to_json();

    /*!
     * @brief Add file from which to load configuration
     *
     * @param[in]   file_name   Path to file configuration
     * */
    void add_file(const std::string& file_name);

    /*!
     * @brief singleton. Get configuration
     *
     * After first use, it creates configuration object
     *
     * @return Configuration object
     * */
    static Configuration& get_instance();

    /*!
     * @brief Singleton. Configuration cleanup
     * */
    static void cleanup();
private:
    using FileName = const std::string;

    json::Json m_json_configuration{};
    std::string m_configuration_file{};

    Configuration() = default;
    Configuration(const Configuration&) = delete;
    Configuration(Configuration&&) = delete;
    Configuration& operator=(const Configuration&) = delete;
    Configuration& operator=(Configuration&&) = delete;
};

} /* namespace configuration */
