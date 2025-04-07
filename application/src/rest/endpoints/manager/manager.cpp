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
    r[Common::ODATA_TYPE] = "#Manager.v1_19_2.Manager";
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

    r[Common::LINKS][Common::ODATA_TYPE] = "#Manager.v1_18_0.Links";
    r[Common::LINKS][Manager::MANAGER_FOR_SERVERS] = json::Json::value_t::array;

    r[Common::ACTIONS] = json::Json::value_t::object;

    r[Common::ADDITIONAL_FIRMWARE_VERSIONS] = json::Json::value_t::object;
    r[Common::ADDITIONAL_FIRMWARE_VERSIONS][Inventory::BOOTLOADER] = json::Json::value_t::null;
    r[Common::ADDITIONAL_FIRMWARE_VERSIONS][Common::OEM][Common::INTEL] = json::Json::value_t::object;
    r[Common::ADDITIONAL_FIRMWARE_VERSIONS][Common::OEM][Common::INTEL][Common::ODATA_TYPE] = "#InteIPUSoftwareInventory.v1_0_0.AdditionalVersions";
    r[Common::ADDITIONAL_FIRMWARE_VERSIONS][Common::OEM][Common::INTEL][Inventory::IMC] = json::Json::value_t::null;
    r[Common::ADDITIONAL_FIRMWARE_VERSIONS][Common::OEM][Common::INTEL][Inventory::RECOVERY] = json::Json::value_t::null;
    r[Common::ADDITIONAL_FIRMWARE_VERSIONS][Common::OEM][Common::INTEL][Inventory::OROM] = json::Json::value_t::null;
    r[Common::OEM][Common::INTEL] = json::Json::value_t::object;
    r[Common::OEM][Common::INTEL][Common::ODATA_TYPE] = "#InteIPUManager.v1_0_0.Manager";
    r[Common::OEM][Common::INTEL][Inventory::BOARD_ID] = json::Json::value_t::null;

    return r;
}

template <agent_framework::model::enums::ManagerInfoType::base_enum MANAGER_TYPE>
bool is_manager_of_type(const agent_framework::model::Manager& m) {
    return MANAGER_TYPE == m.get_manager_type();
}

auto is_rack_manger = is_manager_of_type<agent_framework::model::enums::ManagerInfoType::RackManager>;
auto is_enclosure_manger = is_manager_of_type<agent_framework::model::enums::ManagerInfoType::EnclosureManager>;

void fill_links(const agent_framework::model::Manager& manager, json::Json& r) {

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

    r[Common::ADDITIONAL_FIRMWARE_VERSIONS][Inventory::BOOTLOADER] = manager.get_boot_image_version();
    r[Common::ADDITIONAL_FIRMWARE_VERSIONS][Common::OEM][Common::INTEL][Inventory::IMC] = manager.get_imc_version();
    r[Common::ADDITIONAL_FIRMWARE_VERSIONS][Common::OEM][Common::INTEL][Inventory::RECOVERY] = manager.get_recovery_imc_version();
    r[Common::ADDITIONAL_FIRMWARE_VERSIONS][Common::OEM][Common::INTEL][Inventory::OROM] = manager.get_imc_orom_version();
    r[Common::OEM][Common::INTEL][Inventory::BOARD_ID] = manager.get_board_id_version();

    r[constants::Manager::DATE_TIME] = manager.get_date_time();
    r[constants::Manager::DATE_TIME_LOCAL_OFFSET] = manager.get_date_time_local_offset();

    ::fill_manager_actions(request, manager, r);

    set_response(response, r);
}
