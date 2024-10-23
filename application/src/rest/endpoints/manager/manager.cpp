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

#include "agent-framework/module/model/manager.hpp"
#include "agent-framework/module/service_uuid.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/manager/manager.hpp"
#include "psme/rest/utils/status_helpers.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;

namespace {

json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#Manager.Manager";
    r[Common::ODATA_ID] = json::Json::value_t::null;
    r[Common::ODATA_TYPE] = "#Manager.v1_19_1.Manager";
    r[Common::ID] = json::Json::value_t::null;
    r[Common::NAME] = "Manager";
    r[Common::DESCRIPTION] = "Manager description";
    r[Manager::MANAGER_TYPE] = json::Json::value_t::null;
    r[Manager::POWER_STATE] = agent_framework::model::enums::ManagerPowerState(
                                  agent_framework::model::enums::ManagerPowerState::On)
                                  .to_string();
    r[Common::MODEL] = json::Json::value_t::null;
    r[Common::FIRMWARE_VERSION] = json::Json::value_t::null;
    r[Common::UUID] = json::Json::value_t::null;

    r[Manager::DATE_TIME] = json::Json::value_t::null;
    r[Manager::DATE_TIME_LOCAL_OFFSET] = json::Json::value_t::null;

    r[Common::STATUS][Common::STATE] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Json::value_t::null;

    r[Common::REDUNDANCY] = json::Json::value_t::array;
    r[Manager::ETHERNET_INTERFACES] = json::Json::value_t::null;
    r[Common::LOG_SERVICES] = json::Json::value_t::null;
    r[Manager::NETWORK_PROTOCOL] = json::Json::value_t::object;

    /*
     * @TODO for BVT, cannot be read right now, will be used later
     *
     * r[Manager::GRAPHICAL_CONSOLE] = json::Json::value_t::null;
     * r[Manager::SERIAL_CONSOLE] = json::Json::value_t::null;
     * r[Manager::COMMAND_SHELL] = json::Json::value_t::null;
     *
     * */

    r[Common::LINKS][Common::ODATA_TYPE] = "#Manager.v1_18_0.Links";
    r[Common::LINKS][Manager::MANAGER_FOR_CHASSIS] = json::Json::value_t::array;
    r[Common::LINKS][Manager::MANAGER_FOR_SERVERS] = json::Json::value_t::array;
    r[Common::LINKS][Manager::MANAGER_FOR_SWITCHES] = json::Json::value_t::array;

    r[Common::ACTIONS] = json::Json::value_t::object;

    return r;
}

template <agent_framework::model::enums::ManagerInfoType::base_enum MANAGER_TYPE>
bool is_manager_of_type(const agent_framework::model::Manager& m) {
    return MANAGER_TYPE == m.get_manager_type();
}

auto is_rack_manger = is_manager_of_type<agent_framework::model::enums::ManagerInfoType::RackManager>;
auto is_enclosure_manger = is_manager_of_type<agent_framework::model::enums::ManagerInfoType::EnclosureManager>;

void fill_links(const agent_framework::model::Manager& manager, json::Json& r) {
    // find all chassis managed by this manager
    auto managed_chassis_uuids = agent_framework::module::get_manager<agent_framework::model::Chassis>()
                                     .get_keys(manager.get_uuid(), [](const agent_framework::model::Chassis& ch) {
                                         // Link only those chassis which are manageable.
                                         return ch.get_is_managed();
                                     });
    for (const auto& chassis_uuid : managed_chassis_uuids) {
        json::Json chassis_link = json::Json();
        chassis_link[Common::ODATA_ID] =
            endpoint::utils::get_component_url(agent_framework::model::enums::Component::Chassis, chassis_uuid);
        r[Common::LINKS][constants::Manager::MANAGER_FOR_CHASSIS].push_back(chassis_link);
    }

    // find all systems managed by this manager
    auto managed_system_uuids = agent_framework::module::get_manager<agent_framework::model::System>()
                                    .get_keys([&](const auto& s) {
                                        return s.get_manager() == manager.get_uuid();
                                    });
    for (const auto& system_uuid : managed_system_uuids) {
        json::Json system_link = json::Json();
        system_link[Common::ODATA_ID] =
            endpoint::utils::get_component_url(agent_framework::model::enums::Component::System, system_uuid);
        r[Common::LINKS][constants::Manager::MANAGER_FOR_SERVERS].push_back(system_link);
    }
}

void fill_manager_actions(const server::Request& request,
                          const agent_framework::model::Manager& manager,
                          json::Json& r) {

    json::Json reset = json::Json();
    reset[Common::TARGET] =
        endpoint::PathBuilder(request)
            .append(constants::Common::ACTIONS)
            .append(constants::Manager::MANAGER_RESET)
            .build();

    reset[Common::ALLOWABLE_RESET_TYPES] = json::Json::value_t::array;
    for (const auto& reset_type : manager.get_allowed_reset_actions()) {
        reset[Common::ALLOWABLE_RESET_TYPES].push_back(reset_type.to_string());
    }
    r[Common::ACTIONS][Manager::HASH_MANAGER_RESET] = std::move(reset);
}

} // namespace

/* TODO for BVT, cannot be read right now, will be used later
template <typename T>
json::Json console_to_json(const T& console)
{
    if (console.get_enabled().has_value()) {
        json::Json r = json::Json();
        r[constants::Manager::SERVICE_ENABLED] = console.get_enabled();

        r[constants::Manager::MAX_CONCURRENT_SESSIONS] = 0;
        if(console.get_max_sessions().has_value()) {
            r[constants::Manager::MAX_CONCURRENT_SESSIONS] = console.get_max_sessions();
        }

        json::Json array_json = json::Json::value_t::array;;
        const auto& supported = console.get_types_supported();
        for (const auto& entry : supported) {
            array_json.push_back(std::move(entry.to_string()));
        }
        r[constants::Manager::CONNECT_TYPES_SUPPORTED] = std::move(array_json);
        return r;
    }
    else {
        return json::Json::value_t::null;
    }
}*/

endpoint::Manager::Manager(const std::string& path) : EndpointBase(path) {}

endpoint::Manager::~Manager() {}

void endpoint::Manager::get(const server::Request& request, server::Response& response) {
    using namespace agent_framework::model::enums;

    auto r = make_prototype();
    auto manager = psme::rest::model::find<agent_framework::model::Manager>(request.params).get();

    r[Common::ODATA_ID] = PathBuilder(request).build();
    r[Common::ID] = request.params[PathParam::MANAGER_ID];
    utils::fill_name_and_description(manager, r);

    psme::rest::endpoint::status_to_json(manager, r);
    fill_links(manager, r);

    if (is_rack_manger(manager) || is_enclosure_manger(manager)) {
        r[constants::Manager::SERVICE_ENTRY_POINT_UUID] =
            agent_framework::module::ServiceUuid::get_instance()->get_service_uuid();
    }

    r[constants::Manager::MANAGER_TYPE] = manager.get_manager_type();
    r[Common::MODEL] = manager.get_manager_model();

    r[Common::FIRMWARE_VERSION] = manager.get_firmware_version();
    r[Common::UUID] = manager.get_guid();

    r[constants::Manager::DATE_TIME] = manager.get_date_time();
    r[constants::Manager::DATE_TIME_LOCAL_OFFSET] = manager.get_date_time_local_offset();

    /*
     * @TODO for BVT, cannot be read right now, will be used later
     *
     * r[constants::Manager::GRAPHICAL_CONSOLE] = console_to_json(manager.get_graphical_console());
     * r[constants::Manager::SERIAL_CONSOLE] = console_to_json(manager.get_serial_console());
     * r[constants::Manager::COMMAND_SHELL] = console_to_json(manager.get_command_shell());
     *
     * */

    r[constants::Manager::NETWORK_PROTOCOL][Common::ODATA_ID] =
        PathBuilder(request).append(constants::Manager::NETWORK_PROTOCOL).build();
    r[constants::Manager::ETHERNET_INTERFACES][Common::ODATA_ID] =
        PathBuilder(request).append(constants::Manager::ETHERNET_INTERFACES).build();
    r[constants::Common::LOG_SERVICES][Common::ODATA_ID] =
        PathBuilder(request).append(constants::Common::LOG_SERVICES).build();

    ::fill_manager_actions(request, manager, r);

    set_response(response, r);
}
