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

#include "psme/rest/endpoints/chassis/pcie_device.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/utils/status_helpers.hpp"

using namespace psme::rest;
using namespace psme::rest::endpoint;
using namespace psme::rest::constants;

namespace {
json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#PCIeDevice.PCIeDevice";
    r[Common::ODATA_ID] = json::Json::value_t::null;
    r[Common::ODATA_TYPE] = "#PCIeDevice.v1_0_0.PCIeDevice";
    r[Common::NAME] = "PCIe Device";

    r[Common::ASSET_TAG] = json::Json::value_t::null;
    r[Common::DESCRIPTION] = "PCIe Device Description";
    // DeviceType is only filled when it's available, because it's non-nullable
    r[constants::Common::FIRMWARE_VERSION] = json::Json::value_t::null;
    r[Common::ID] = json::Json::value_t::null;

    r[Common::LINKS][Common::CHASSIS] = json::Json::value_t::array;
    r[Common::LINKS][constants::PcieDevice::PCIE_FUNCTIONS] = json::Json::value_t::array;

    r[Common::MANUFACTURER] = json::Json::value_t::null;
    r[Common::MODEL] = json::Json::value_t::null;
    r[Common::PART_NUMBER] = json::Json::value_t::null;
    r[Common::SKU] = json::Json::value_t::null;
    r[Common::SERIAL_NUMBER] = json::Json::value_t::null;

    r[Common::STATUS][Common::STATE] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Json::value_t::null;

    return r;
}

void fill_links(const agent_framework::model::PcieDevice& device, json::Json& json) {
    if (device.get_chassis().has_value()) {
        try {
            auto chassis_id = agent_framework::module::get_manager<agent_framework::model::Chassis>()
                                  .get_entry(device.get_chassis().value())
                                  .get_id();

            // fill Chassis link
            json::Json chassis_link = json::Json();
            chassis_link[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL)
                                                 .append(Common::CHASSIS)
                                                 .append(chassis_id)
                                                 .build();
            json[Common::LINKS][Common::CHASSIS].push_back(std::move(chassis_link));

            // fill Functions link
            for (const auto function_id : agent_framework::module::get_manager<agent_framework::model::PcieFunction>().get_ids(
                     device.get_uuid())) {
                json::Json function_link = json::Json();
                function_link[Common::ODATA_ID] = endpoint::PathBuilder(constants::PathParam::BASE_URL)
                                                      .append(constants::Common::CHASSIS)
                                                      .append(chassis_id)
                                                      .append(constants::Chassis::PCIE_DEVICES)
                                                      .append(device.get_id())
                                                      .append(constants::PathParam::FUNCTIONS)
                                                      .append(function_id)
                                                      .build();
                json[Common::LINKS][constants::PcieDevice::PCIE_FUNCTIONS].push_back(std::move(function_link));
            }
        }
        catch (const agent_framework::exceptions::InvalidUuid&) {
            log_error("rest", "Device " + device.get_uuid() + " has chassis " + device.get_chassis().value() + " which does not exist as a resource");
        }
    }
}

agent_framework::model::PcieDevice find_pci_device(const server::Request& req) {

    // the devices are under the same manager as the chassis from the URL
    auto chassis_manager_uuid = psme::rest::model::find<agent_framework::model::Chassis>(
                                    req.params)
                                    .get_one()
                                    ->get_parent_uuid();

    auto chassis_manager_id = agent_framework::module::get_manager<agent_framework::model::Manager>().get_entry(chassis_manager_uuid).get_id();

    auto params_copy = req.params;
    params_copy[constants::PathParam::MANAGER_ID] = std::to_string(chassis_manager_id);

    return psme::rest::model::find<agent_framework::model::Manager, agent_framework::model::PcieDevice>(
               params_copy)
        .get();
}

} // namespace

endpoint::PcieDevice::PcieDevice(const std::string& path) : EndpointBase(path) {}

endpoint::PcieDevice::~PcieDevice() {}

void endpoint::PcieDevice::get(const server::Request& req, server::Response& res) {
    auto json = ::make_prototype();

    json[Common::ODATA_ID] = PathBuilder(req).build();

    const auto device = find_pci_device(req);

    json[Common::ASSET_TAG] = device.get_asset_tag();
    if (device.get_device_class().has_value()) {
        json[constants::PcieDevice::DEVICE_TYPE] = device.get_device_class();
    }
    json[constants::Common::FIRMWARE_VERSION] = device.get_firmware_version();
    json[Common::ID] = req.params[PathParam::DEVICE_ID];
    json[Common::SKU] = device.get_sku();

    fill_links(device, json);

    const auto& fru = device.get_fru_info();
    json[Common::MANUFACTURER] = fru.get_manufacturer();
    json[Common::MODEL] = fru.get_model_number();
    json[Common::PART_NUMBER] = fru.get_part_number();
    json[Common::SERIAL_NUMBER] = fru.get_serial_number();

    endpoint::status_to_json(device, json);

    set_response(res, json);
}
