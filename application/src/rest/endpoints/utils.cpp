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

#include "psme/rest/endpoints/utils.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/model/find.hpp"
#include "psme/rest/server/multiplexer.hpp"
#include "psme/rest/server/parameters.hpp"
#include "psme/rest/server/utils.hpp"

#include "agent-framework/module/model/model_chassis.hpp"
#include "agent-framework/module/model/model_compute.hpp"
#include "agent-framework/module/model/model_storage.hpp"

#include <cmath>
#include <iterator>
#include <regex>

using namespace psme::rest::constants::Common;
using namespace psme::rest::constants::PathParam;
using namespace agent_framework::module;
using namespace agent_framework::model;
using namespace agent_framework::model::enums;

namespace psme {
namespace rest {
namespace endpoint {
namespace utils {

std::uint64_t id_to_uint64(const std::string& id_as_string) {
    if (!std::all_of(id_as_string.begin(), id_as_string.end(), ::isdigit)) {
        // A string containing non-digit characters is NOT supposed to reach this method,
        // because the rest server only allows ID strings of the form [0-9]+. So, we log an error.
        log_error("rest", "String " + id_as_string +
                              " which contains not-digit characters was received for conversion to REST ID!");

        throw agent_framework::exceptions::NotFound(
            "ID '" + id_as_string + "' could not be converted to REST ID, because it's not a number.");
    }

    std::uint64_t id{};
    try {
        id = std::stoull(id_as_string, NULL);
    }
    catch (const std::out_of_range&) {
        throw agent_framework::exceptions::NotFound(
            "ID '" + id_as_string + "' could not be converted to REST ID, because it's too big.");
    }
    catch (const std::invalid_argument&) {
        // Occures when ID is empty string
        throw agent_framework::exceptions::NotFound(
            "ID '" + id_as_string + "' could not be converted to REST ID, because it's not a proper ID.");
    }
    return id;
}

namespace {
template <typename M>
void build_parent_path(endpoint::PathBuilder& path, const std::string& uuid, const std::string& collection_literal) {
    path.append(collection_literal).append(agent_framework::module::get_manager<M>().uuid_to_rest_id(uuid));
}

template <typename M>
void build_child_path(endpoint::PathBuilder& path, const std::string& uuid, const std::string& collection_literal) {
    const auto& resource = agent_framework::module::get_manager<M>().get_entry(uuid);
    get_component_url_recursive(path, resource.get_parent_type(), resource.get_parent_uuid());
    path.append(collection_literal).append(resource.get_id());
}
} // namespace

void get_component_url_recursive(endpoint::PathBuilder& path, enums::Component type, const std::string& uuid) {
    using Component = enums::Component;
    using namespace psme::rest;
    using namespace agent_framework;

    switch (type) {
    case Component::Manager:
        build_parent_path<agent_framework::model::Manager>(path, uuid, constants::Root::MANAGERS);
        break;
    case Component::Task:
        path.append(constants::TaskService::TASK_SERVICE).append(constants::TaskService::TASKS).append(module::get_manager<agent_framework::model::Task>().uuid_to_rest_id(uuid));
        break;
    case Component::System:
        build_parent_path<agent_framework::model::System>(path, uuid, constants::Root::SYSTEMS);
        break;
    case Component::VirtualMedia:
        build_child_path<agent_framework::model::VirtualMedia>(path, uuid, constants::Common::VIRTUAL_MEDIA);
        break;
    default:
        THROW(agent_framework::exceptions::NotFound, "rest",
              "Could not get URL for component type: " + std::string(type.to_string()));
        break;
    }
}

std::string get_component_url(enums::Component type, const std::string& uuid) {
    endpoint::PathBuilder path_builder(constants::Routes::ROOT_PATH);
    get_component_url_recursive(path_builder, type, uuid);
    return path_builder.build();
}

std::string get_task_monitor_url(const std::string& uuid) {
    endpoint::PathBuilder path(constants::Routes::TASK_SERVICE_PATH);
    path.append(constants::Monitor::TASK_MONITORS)
        .append(agent_framework::module::get_manager<agent_framework::model::Task>().uuid_to_rest_id(uuid));
    return path.build();
}

void set_location_header(const server::Request& request, server::Response& response, const std::string& path) {
    const std::string& absolute_location_path = psme::rest::server::build_location_header(request, path);
    response.set_header(LOCATION, absolute_location_path);
}

} // namespace utils
} // namespace endpoint
} // namespace rest
} // namespace psme
