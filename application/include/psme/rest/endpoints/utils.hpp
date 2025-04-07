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
 * */

#pragma once

#include "path_builder.hpp"
#include "psme/rest/server/parameters.hpp"
#include "psme/rest/server/request.hpp"
#include "psme/rest/server/response.hpp"

#include "agent-framework/module/chassis_components.hpp"
#include "agent-framework/module/common_components.hpp"
#include "agent-framework/module/compute_components.hpp"
#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/managers/generic_manager.hpp"
#include "agent-framework/module/model/chassis.hpp"
#include "agent-framework/module/model/manager.hpp"
#include "agent-framework/module/model/resource.hpp"

#include <vector>

namespace psme {
namespace rest {
namespace endpoint {
namespace utils {

/*!
 * @brief a method converting a string to uint64_t specialized for REST ids: if
 * the string is incorrect, the method throws an InvalidParameters exception
 *
 * @param id_as_string taken out from request url
 *
 * @return id as a number
 */
std::uint64_t id_to_uint64(const std::string& id_as_string);

/*!
 * @brief finds the part of the URL after "/redfish/v1" using
 * recursion
 *
 * @param path_builder The PathBuilder that carries the url under construction
 *
 * @param type component's type
 *
 * @param uuid component's uuid
 */
void get_component_url_recursive(psme::rest::endpoint::PathBuilder& path_builder,
                                 agent_framework::model::enums::Component type,
                                 const std::string& uuid);

/*!
 * @brief finds the URL of the given component
 *
 * @param type component's type
 *
 * @param uuid component's uuid
 *
 * @return component's REST URL
 */
std::string get_component_url(agent_framework::model::enums::Component type, const std::string& uuid);

/*!
 * @brief finds the URL of a task monitor for a given task
 *
 * @param uuid task's uuid
 *
 * @return task monitor's REST URL
 */
std::string get_task_monitor_url(const std::string& uuid);

/*!
 * @brief Sets component location in response header
 *
 * @param req Request - is used to determine IP address, port and protocol
 *
 * @param res Response - location header is stored in this response
 *
 * @param path component's REST URL
 */
void set_location_header(const server::Request& req, server::Response& res, const std::string& path);

/*!
 * @brief Adds value to json if a value is present
 * @tparam T Type of value
 * @param json Reference to json to add value
 * @param key Key of json object for value
 * @param value Optional value to add.
 */
template <typename T>
void add_to_json_if_has_value(json::Json& json, const std::string& key, const OptionalField<T>& value) {
    if (value.has_value()) {
        json[key] = value;
    }
}

/*!
 * @brief Fill JSON with resource name and description (if applicable)
 * @param[in] resource Resource
 * @param[in,out] json JSON value sent as a GET response
 */
template <typename T>
void fill_name_and_description(const T& resource, json::Json& json) {
    add_to_json_if_has_value(json, constants::Common::NAME, resource.get_name());
    add_to_json_if_has_value(json, constants::Common::DESCRIPTION, resource.get_description());
}

} // namespace utils
} // namespace endpoint
} // namespace rest
} // namespace psme
