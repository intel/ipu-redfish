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
 * */

#include "configuration/utils.hpp"
#include "logger/logger_factory.hpp"

bool configuration::string_to_json(const std::string& json_string,
                                   json::Json& root) {
    try {
        root = json::Json::parse(json_string);
    }
    catch (const std::exception& e) {
        log_error("validator", e.what());
        return false;
    }

    return true;
}

std::string
configuration::json_value_to_string(const json::Json& value) {
    std::string output;
    output = value.dump();
    return output;
}
