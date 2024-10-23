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

#include "psme/rest/endpoints/chassis/pcie_function.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "psme/rest/utils/status_helpers.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace agent_framework::module;

namespace {
json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#PCIeFunction.PCIeFunction";
    r[Common::ODATA_ID] = json::Json::value_t::null;
    r[Common::ODATA_TYPE] = "#PCIeFunction.v1_0_0.PCIeFunction";
    r[Common::NAME] = "PCIe Function";

    r[constants::PcieFunction::CLASS_CODE] = json::Json::value_t::null;
    r[Common::DESCRIPTION] = "PCIe Function description";
    r[constants::PcieFunction::DEVICE_ID] = json::Json::value_t::null;
    r[constants::PcieFunction::FUNCTION_ID] = json::Json::value_t::null;
    // FunctionType and DeviceClass are only filled when it's available, because they're non-nullable
    r[Common::ID] = json::Json::value_t::null;
    r[constants::PcieFunction::REVISION_ID] = json::Json::value_t::null;

    r[Common::STATUS][Common::STATE] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Json::value_t::null;

    r[constants::PcieFunction::SUBSYSTEM_ID] = json::Json::value_t::null;
    r[constants::PcieFunction::SUBSYSTEM_VENDOR_ID] = json::Json::value_t::null;
    r[constants::PcieFunction::VENDOR_ID] = json::Json::value_t::null;

    r[Common::LINKS][constants::Chassis::DRIVES] = json::Json::value_t::array;
    r[Common::LINKS][constants::PcieFunction::PCIE_DEVICE] = json::Json::value_t::object;
    r[Common::LINKS][constants::StorageSubsystem::STORAGE_CONTROLLERS] = json::Json::value_t::array;
    r[Common::LINKS][constants::System::ETHERNET_INTERFACES] = json::Json::value_t::array;

    return r;
}

void fill_links(const std::string& device_uuid, json::Json& json) {

    json[Common::LINKS][constants::PcieFunction::PCIE_DEVICE][Common::ODATA_ID] =
        endpoint::utils::get_component_url(agent_framework::model::enums::Component::PcieDevice, device_uuid);
}

agent_framework::model::PcieDevice find_pci_device(const server::Request& req) {

    // the devices are under the same manager as the chassis from the URL
    auto chassis_manager_uuid = psme::rest::model::find<agent_framework::model::Chassis>(
                                    req.params)
                                    .get_one()
                                    ->get_parent_uuid();

    auto chassis_manager_id = agent_framework::module::get_manager<agent_framework::model::Manager>().get_entry(
                                                                                                         chassis_manager_uuid)
                                  .get_id();

    auto params_copy = req.params;
    params_copy[constants::PathParam::MANAGER_ID] = std::to_string(chassis_manager_id);

    return psme::rest::model::find<agent_framework::model::Manager, agent_framework::model::PcieDevice>(
               params_copy)
        .get();
}

} // namespace

endpoint::PcieFunction::PcieFunction(const std::string& path) : EndpointBase(path) {}

endpoint::PcieFunction::~PcieFunction() {}

void endpoint::PcieFunction::get(const server::Request& req, server::Response& res) {
    auto json = ::make_prototype();

    json[Common::ODATA_ID] = PathBuilder(req).build();

    const auto device = find_pci_device(req);

    const auto function = psme::rest::model::find<agent_framework::model::PcieDevice, agent_framework::model::PcieFunction>(
                              req.params)
                              .get();

    json[Common::ID] = req.params[PathParam::FUNCTION_ID];

    fill_links(device.get_uuid(), json);

    json[constants::PcieFunction::CLASS_CODE] = function.get_pci_class_code();
    json[constants::PcieFunction::DEVICE_ID] = function.get_pci_device_id();
    try {
        if (function.get_function_id().has_value()) {
            json[constants::PcieFunction::FUNCTION_ID] = std::stoi(function.get_function_id());
        }
    }
    catch (const std::exception& ex) {
        log_warning("rest", "Invalid function id type:" << ex.what());
    }
    json[constants::PcieFunction::REVISION_ID] = function.get_pci_revision_id();
    if (function.get_device_class().has_value()) {
        json[constants::PcieFunction::DEVICE_CLASS] = function.get_device_class();
    }
    if (function.get_function_type().has_value()) {
        json[constants::PcieFunction::FUNCTION_TYPE] = function.get_function_type();
    }

    endpoint::status_to_json(function, json);
    json[Common::STATUS][Common::HEALTH_ROLLUP] = function.get_status().get_health();

    json[constants::PcieFunction::SUBSYSTEM_ID] = function.get_pci_subsystem_id();
    json[constants::PcieFunction::SUBSYSTEM_VENDOR_ID] = function.get_pci_subsystem_vendor_id();
    json[constants::PcieFunction::VENDOR_ID] = function.get_pci_vendor_id();

    set_response(res, json);
}
