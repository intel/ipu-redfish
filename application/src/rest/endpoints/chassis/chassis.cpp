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

#include "psme/rest/endpoints/chassis/chassis.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/utils/status_helpers.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;

namespace {

json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#Chassis.Chassis";
    r[Common::ODATA_ID] = json::Json::value_t::null;
    r[Common::ODATA_TYPE] = "#Chassis.v1_7_0.Chassis";
    r[Common::ID] = json::Json::value_t::null;
    r[Chassis::CHASSIS_TYPE] = json::Json::value_t::null;
    r[Common::NAME] = "Chassis";
    r[Common::DESCRIPTION] = "Chassis description";

    r[Common::MANUFACTURER] = json::Json::value_t::null;
    r[Common::MODEL] = json::Json::value_t::null;
    r[Common::SKU] = json::Json::value_t::null;
    r[Common::SERIAL_NUMBER] = json::Json::value_t::null;
    r[Common::PART_NUMBER] = json::Json::value_t::null;
    r[Common::ASSET_TAG] = json::Json::value_t::null;
    r[Chassis::INDICATOR_LED] = json::Json::value_t::null;
    r[Chassis::NETWORK_ADAPTERS] = json::Json::value_t::null;

    // Power and Thermal links are created only for Zone and Sled Chassis

    r[Common::STATUS][Common::STATE] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Json::value_t::null;

    r[Common::LINKS][Common::ODATA_TYPE] = "#Chassis.v1_7_0.Links";
    r[Common::LINKS][Common::MANAGED_BY] = json::Json::value_t::array;
    r[Common::LINKS][Chassis::CONTAINS] = json::Json::value_t::array;
    r[Common::LINKS][Chassis::COMPUTER_SYSTEMS] = json::Json::value_t::array;
    r[Common::LINKS][Chassis::MANAGERS_IN_CHASSIS] = json::Json::value_t::array;
    r[Common::LINKS][Chassis::DRIVES] = json::Json::value_t::array;
    r[Common::LINKS][Chassis::PCIE_DEVICES] = json::Json::value_t::array;
    r[Common::LINKS][Chassis::STORAGE] = json::Json::value_t::array;

    r[Common::ACTIONS] = json::Json::value_t::object;

    return r;
}

void fill_chassis_link(json::Json& r, const std::string& chassis_uuid) {
    r[Common::ODATA_ID] = endpoint::utils::get_component_url(agent_framework::model::enums::Component::Chassis, chassis_uuid);
}

void fill_chassis_links(json::Json& r, const std::vector<std::string>& uuids,
                        OptionalField<std::string> omit_uuid = {}) {

    for (const auto& uuid : uuids) {
        if (!omit_uuid.has_value() || uuid != omit_uuid.value()) {
            json::Json link = json::Json();
            link[Common::ODATA_ID] =
                endpoint::utils::get_component_url(agent_framework::model::enums::Component::Chassis, uuid);
            r.push_back(std::move(link));
        }
    }
}

template <agent_framework::model::enums::ChassisType::base_enum CHASSIS_TYPE>
bool is_chassis_of_type(const agent_framework::model::Chassis& ch) {
    return CHASSIS_TYPE == ch.get_type();
}

void fill_containing_links(const agent_framework::model::Chassis& chassis, json::Json& r) {

    auto is_rack = is_chassis_of_type<agent_framework::model::enums::ChassisType::Rack>;
    auto is_zone = is_chassis_of_type<agent_framework::model::enums::ChassisType::Zone>;
    auto is_drawer = is_chassis_of_type<agent_framework::model::enums::ChassisType::Drawer>;
    auto is_enclosure = is_chassis_of_type<agent_framework::model::enums::ChassisType::Enclosure>;
    auto is_child_chassis = [&is_rack, &is_drawer, &is_zone](const agent_framework::model::Chassis& c) {
        return !is_rack(c) && !is_drawer(c) && !is_zone(c);
    };
    auto& chassis_manager = agent_framework::module::get_manager<agent_framework::model::Chassis>();

    try {
        if (is_rack(chassis)) {
            /* This is the top chassis and it contains all other chassis, does not contain itself */
            auto uuids = chassis_manager.get_keys();
            fill_chassis_links(r[Common::LINKS][Chassis::CONTAINS], uuids, chassis.get_uuid());
        } else if (is_zone(chassis)) {
            /* Zones are contained by the rack, and does not contain any other chassis */
            auto uuids = chassis_manager.get_keys(is_rack);
            if (uuids.size() > 0) {
                fill_chassis_link(r[Common::LINKS][Common::CONTAINED_BY], uuids.front());
            }
        } else if (is_drawer(chassis)) {
            /* Drawers contain everything but racks, zones and other drawers */
            auto uuids = chassis_manager.get_keys(is_child_chassis);
            fill_chassis_links(r[Common::LINKS][Chassis::CONTAINS], uuids);

            /* Drawers may be contained by racks */
            auto racks = chassis_manager.get_keys(is_rack);
            if (racks.size() > 0) {
                fill_chassis_link(r[Common::LINKS][Common::CONTAINED_BY], racks.front());
            }
        } else if (is_enclosure(chassis)) {
            /* Enclosures may contain everything but racks, zones and drawers, does not contain itself */
            auto uuids = chassis_manager.get_keys(is_child_chassis);
            fill_chassis_links(r[Common::LINKS][Chassis::CONTAINS], uuids, chassis.get_uuid());
        } else {
            /* Other cases -> chassis is to be contained by the drawer */
            auto uuids = chassis_manager.get_keys(is_drawer);
            if (uuids.size() > 0) {
                fill_chassis_link(r[Common::LINKS][Common::CONTAINED_BY], uuids.front());
            }
        }
    }
    catch (const std::exception& e) {
        log_error("rest", "Exception caught during filling Chassis"
                          "links Contains and ContainedBy: "
                              << e.what());
    }
}

void fill_links(const agent_framework::model::Chassis& chassis, json::Json& r) {
    // find manager of this chassis
    try {
        if (chassis.get_is_managed()) {
            json::Json managed_by = json::Json();
            managed_by[Common::ODATA_ID] = psme::rest::endpoint::utils::get_component_url(
                agent_framework::model::enums::Component::Manager, chassis.get_parent_uuid());
            r[Common::LINKS][Common::MANAGED_BY].push_back(managed_by);
        }
    }
    catch (const agent_framework::exceptions::InvalidUuid&) {
    }

    // find managers located in this chassis
    auto managers_in_chassis_uuids = agent_framework::module::get_manager<agent_framework::model::Manager>()
                                         .get_keys([&chassis](const agent_framework::model::Manager& manager) {
                                             return manager.get_location() == chassis.get_uuid();
                                         });
    for (const auto& manager_uuid : managers_in_chassis_uuids) {
        json::Json link = json::Json();
        link[Common::ODATA_ID] = psme::rest::endpoint::utils::get_component_url(
            agent_framework::model::enums::Component::Manager, manager_uuid);
        r[Common::LINKS][Chassis::MANAGERS_IN_CHASSIS].push_back(std::move(link));
    }

    // systems and storage subsystems in chassis
    auto& system_manager = agent_framework::module::get_manager<agent_framework::model::System>();
    auto& storage_manager = agent_framework::module::get_manager<agent_framework::model::StorageSubsystem>();
    auto system_uuids = system_manager.get_keys();
    for (const auto& system_uuid : system_uuids) {
        auto system = system_manager.get_entry(system_uuid);
        if (system.get_chassis() == chassis.get_uuid()) {
            json::Json link = json::Json();
            link[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL)
                                         .append(constants::Root::SYSTEMS)
                                         .append(system.get_id())
                                         .build();
            r[Common::LINKS][constants::Chassis::COMPUTER_SYSTEMS].push_back(std::move(link));

            for (const auto storage_id : storage_manager.get_ids(system.get_uuid())) {
                json::Json storage_link = json::Json();
                storage_link[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL)
                                                     .append(constants::Root::SYSTEMS)
                                                     .append(system.get_id())
                                                     .append(constants::System::STORAGE)
                                                     .append(storage_id)
                                                     .build();
                r[Common::LINKS][constants::Chassis::STORAGE].push_back(std::move(storage_link));
            }
        }
    }

    // drives in chassis
    auto drive_ids = agent_framework::module::get_manager<agent_framework::model::Drive>().get_ids(chassis.get_uuid());
    for (const auto& drive_id : drive_ids) {
        json::Json link = json::Json();
        link[Common::ODATA_ID] = endpoint::PathBuilder(constants::PathParam::BASE_URL)
                                     .append(Common::CHASSIS)
                                     .append(chassis.get_id())
                                     .append(constants::Chassis::DRIVES)
                                     .append(drive_id)
                                     .build();
        r[Common::LINKS][Chassis::DRIVES].push_back(std::move(link));
    }

    // PCIe devices in chassis
    // the devices are under the same manager as the chassis from the URL
    auto chassis_manager_uuid = chassis.get_parent_uuid();
    auto pcie_device_ids = agent_framework::module::get_manager<agent_framework::model::PcieDevice>().get_ids(chassis_manager_uuid);

    for (const auto& pcie_device_id : pcie_device_ids) {
        json::Json link = json::Json();
        link[Common::ODATA_ID] = endpoint::PathBuilder(constants::PathParam::BASE_URL)
                                     .append(Common::CHASSIS)
                                     .append(chassis.get_id())
                                     .append(constants::Chassis::PCIE_DEVICES)
                                     .append(pcie_device_id)
                                     .build();
        r[Common::LINKS][Chassis::PCIE_DEVICES].push_back(std::move(link));
    }
}
} // namespace

endpoint::Chassis::Chassis(const std::string& path) : EndpointBase(path) {}

endpoint::Chassis::~Chassis() {}

void endpoint::Chassis::get(const server::Request& request, server::Response& response) {
    auto chassis = psme::rest::model::find<agent_framework::model::Chassis>(request.params).get();

    auto r = make_prototype();

    psme::rest::endpoint::status_to_json(chassis, r);
    r[Common::ODATA_ID] = PathBuilder(request).build();
    r[Common::ID] = request.params[PathParam::CHASSIS_ID];

    fill_links(chassis, r);
    fill_containing_links(chassis, r);

    if (agent_framework::model::enums::ChassisType::Rack == chassis.get_type()) {
        r[Common::UUID] = chassis.get_uuid();
    }

    r[constants::Chassis::NETWORK_ADAPTERS][Common::ODATA_ID] =
        PathBuilder(request).append(constants::Chassis::NETWORK_ADAPTERS).build();
    r[constants::Chassis::CHASSIS_TYPE] = chassis.get_type().to_string();
    r[Common::MANUFACTURER] = chassis.get_fru_info().get_manufacturer();
    r[Common::MODEL] = chassis.get_fru_info().get_model_number();
    r[Common::SERIAL_NUMBER] = chassis.get_fru_info().get_serial_number();
    r[Common::PART_NUMBER] = chassis.get_fru_info().get_part_number();
    r[Common::ASSET_TAG] = chassis.get_asset_tag();
    r[Common::SKU] = chassis.get_sku();
    r[constants::Chassis::INDICATOR_LED] = chassis.get_indicator_led();

    set_response(response, r);
}
