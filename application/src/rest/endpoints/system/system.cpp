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

#include "psme/rest/validators/schemas/system.hpp"
#include "context.hpp"
#include "psme/rest/endpoints/system/system.hpp"
#include "psme/rest/endpoints/task_service/monitor_content_builder.hpp"
#include "psme/rest/endpoints/task_service/task_service_utils.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/utils/status_helpers.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "utils/conversion.hpp"

#include <set>

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::endpoint::utils;
using namespace psme::rest::validators;
using namespace agent_framework::module;
using namespace agent_framework::model;

namespace {

json::Json make_prototype() {
    json::Json response(json::Json::value_t::object);

    response[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#ComputerSystem.ComputerSystem";
    response[Common::ODATA_ID] = json::Json::value_t::null;
    response[Common::ODATA_TYPE] = "#ComputerSystem.v1_22_1.ComputerSystem";
    response[Common::ID] = json::Json::value_t::null;
    response[Common::NAME] = "Computer System";
    response[Common::DESCRIPTION] = "Computer System description";
    response[System::SYSTEM_TYPE] = json::Json::value_t::null;
    response[Common::ASSET_TAG] = json::Json::value_t::null;
    response[Common::MANUFACTURER] = json::Json::value_t::null;
    response[Common::MODEL] = json::Json::value_t::null;
    response[Common::SKU] = json::Json::value_t::null;
    response[Common::SERIAL_NUMBER] = json::Json::value_t::null;
    response[Common::PART_NUMBER] = json::Json::value_t::null;
    response[Common::UUID] = json::Json::value_t::null;
    response[System::HOST_NAME] = json::Json::value_t::null;
    response[Common::STATUS][Common::STATE] = json::Json::value_t::null;
    response[Common::STATUS][Common::HEALTH] = json::Json::value_t::null;
    response[Common::STATUS][Common::HEALTH_ROLLUP] = json::Json::value_t::null;
    response[System::INDICATOR_LED] = json::Json::value_t::null;
    response[System::POWER_STATE] = json::Json::value_t::null;
    response[System::BIOS_VERSION] = json::Json::value_t::null;

    json::Json boot = json::Json();
    boot[Common::ODATA_TYPE] = "#ComputerSystem.v1_1_0.Boot";
    boot[System::BOOT_SOURCE_OVERRIDE_ENABLED] = json::Json::value_t::null;
    boot[System::BOOT_SOURCE_OVERRIDE_TARGET] = json::Json::value_t::null;
    boot[System::BOOT_SOURCE_OVERRIDE_TARGET_ALLOWABLE_VALUES] = json::Json::value_t::array;
    boot[System::BOOT_SOURCE_OVERRIDE_MODE] = json::Json::value_t::null;
    boot[System::BOOT_SOURCE_OVERRIDE_MODE_ALLOWABLE_VALUES] = json::Json::value_t::array;

    response[System::BOOT] = std::move(boot);

    json::Json processors = json::Json();
    processors[System::COUNT] = json::Json::value_t::null;
    processors[Common::MODEL] = json::Json::value_t::null;
    processors[Common::STATUS][Common::STATE] = json::Json::value_t::null;
    processors[Common::STATUS][Common::HEALTH] = json::Json::value_t::null;
    processors[Common::STATUS][Common::HEALTH_ROLLUP] = json::Json::value_t::null;
    response[System::PROCESSOR_SUMMARY] = std::move(processors);

    json::Json memory = json::Json();
    memory[System::TOTAL_SYSTEM_MEMORY_GIB] = json::Json::value_t::null;
    memory[Common::STATUS][Common::STATE] = json::Json::value_t::null;
    memory[Common::STATUS][Common::HEALTH] = json::Json::value_t::null;
    memory[Common::STATUS][Common::HEALTH_ROLLUP] = json::Json::value_t::null;
    response[System::MEMORY_SUMMARY] = std::move(memory);

    response[System::PROCESSORS] = json::Json::value_t::null;
    response[System::ETHERNET_INTERFACES] = json::Json::value_t::null;
    response[System::STORAGE] = json::Json::value_t::null;
    response[System::MEMORY] = json::Json::value_t::null;
    response[Chassis::PCIE_DEVICES] = json::Json::value_t::array;
    response[PcieDevice::PCIE_FUNCTIONS] = json::Json::value_t::array;
    response[System::TRUSTED_MODULES] = json::Json::value_t::array;

    response[Common::LINKS][Common::ODATA_TYPE] = "#ComputerSystem.v1_2_0.Links";
    response[Common::LINKS][Common::CHASSIS] = json::Json::value_t::array;
    response[Common::LINKS][System::ENDPOINTS] = json::Json::value_t::array;
    response[Common::LINKS][Common::MANAGED_BY] = json::Json::value_t::array;

    response[Common::ACTIONS] = json::Json::value_t::object;
    response[Common::VIRTUAL_MEDIA] = json::Json::value_t::object;

    return response;
}

void add_reset_action(const server::Request& request, json::Json& response) {
    json::Json reset = json::Json();

    reset[Common::TARGET] = psme::rest::endpoint::PathBuilder(request)
                                .append(constants::Common::ACTIONS)
                                .append(constants::System::COMPUTER_SYSTEM_RESET)
                                .build();

    json::Json allowable_reset_types = json::Json::value_t::array;
    for (const auto& reset_type : endpoint::System::get_allowed_reset_types()) {
        allowable_reset_types.push_back(reset_type.to_string());
    }
    reset[Common::ALLOWABLE_RESET_TYPES] = std::move(allowable_reset_types);

    response[Common::ACTIONS][System::HASH_COMPUTER_SYSTEM_RESET] = std::move(reset);
}

void add_processors_summary(const agent_framework::model::System& system, json::Json& response) {
    const agent_framework::model::enums::State ENABLED = agent_framework::model::enums::State::Enabled;

    // PROCESSORS SUMMARY
    auto procs = agent_framework::module::get_manager<agent_framework::model::Processor>()
                     .get_keys(system.get_uuid());

    response[constants::System::PROCESSOR_SUMMARY][Common::MODEL] = json::Json::value_t::null;
    auto processors_count = static_cast<std::uint32_t>(procs.size());
    if (processors_count > 0) {
        const auto& proc = agent_framework::module::get_manager<agent_framework::model::Processor>()
                               .get_entry(procs.front());
        response[constants::System::PROCESSOR_SUMMARY][Common::MODEL] = proc.get_model_name();
    }

    response[constants::System::PROCESSOR_SUMMARY][Common::STATUS][Common::STATE] =
        procs.empty() ? json::Json(nullptr) : json::Json(ENABLED.to_string());
    auto summarized_proc_health = psme::rest::endpoint::HealthRollup<agent_framework::model::System>()
                                      .get(system, agent_framework::model::enums::Component::Processor);

    response[constants::System::PROCESSOR_SUMMARY][Common::STATUS][Common::HEALTH] = summarized_proc_health;
    response[constants::System::PROCESSOR_SUMMARY][Common::STATUS][Common::HEALTH_ROLLUP] = summarized_proc_health;

    auto is_processor_present = [](const agent_framework::model::Processor& p) {
        return (p.get_status().get_state() == agent_framework::model::enums::State::Absent) ? 0 : 1;
    };
    auto available_processors =
        psme::rest::endpoint::utils::query_entries<agent_framework::model::Processor, std::uint32_t>(
            system.get_uuid(), is_processor_present);
    response[constants::System::PROCESSOR_SUMMARY][constants::System::COUNT] =
        std::accumulate(available_processors.begin(), available_processors.end(), 0u);
}

void add_memory_summary(const agent_framework::model::System& system, json::Json& response) {
    const agent_framework::model::enums::State ENABLED = agent_framework::model::enums::State::Enabled;

    auto memory_modules = agent_framework::module::get_manager<agent_framework::model::Memory>()
                              .get_keys(system.get_uuid());

    OptionalField<std::uint32_t> total_size_mb{};
    auto get_memory_size = [](const agent_framework::model::Memory& m) { return m.get_capacity_mib(); };
    auto memory_sizes = psme::rest::endpoint::utils::query_entries<agent_framework::model::Memory, OptionalField<std::uint32_t>>(system.get_uuid(), get_memory_size);

    for (const auto& mem_size : memory_sizes) {
        total_size_mb = total_size_mb + mem_size;
    }

    response[constants::System::MEMORY_SUMMARY][constants::System::TOTAL_SYSTEM_MEMORY_GIB] =
        (total_size_mb.has_value() ? json::Json(endpoint::utils::round(
                                         ::utils::convert_byte_units(total_size_mb, ::utils::BytePrefix::Mibi, ::utils::BytePrefix::Gibi), 2))
                                   : json::Json(json::Json::value_t::null));

    response[constants::System::MEMORY_SUMMARY][Common::STATUS][Common::STATE] =
        memory_modules.empty() ? json::Json(nullptr) : json::Json(ENABLED.to_string());
    auto summarized_memory_health = psme::rest::endpoint::HealthRollup<agent_framework::model::System>()
                                        .get(system, agent_framework::model::enums::Component::Memory);

    response[constants::System::MEMORY_SUMMARY][Common::STATUS][Common::HEALTH] = summarized_memory_health;
    response[constants::System::MEMORY_SUMMARY][Common::STATUS][Common::HEALTH_ROLLUP] = summarized_memory_health;
}

void add_system_relations(const agent_framework::model::System& system, json::Json& response) {
    // PCIe Devices
    // The manager for this system is also the manager for all the PCIe devices
    const auto& manager_uuid = system.get_parent_uuid();
    for (const auto& device_uuid :
         agent_framework::module::get_manager<agent_framework::model::PcieDevice>().get_keys(manager_uuid)) {
        json::Json device_link = json::Json();
        device_link[Common::ODATA_ID] = endpoint::utils::get_component_url(
            agent_framework::model::enums::Component::PcieDevice, device_uuid);
        response[Chassis::PCIE_DEVICES].push_back(std::move(device_link));
    }

    // PCIe Functions
    // Per the architect's decision, we do not fill the PCIeFunctions array

    // Endpoints
    // Per the architect's decision, we do not fill the Endpoints array
}

void fill_tpm_data(const agent_framework::model::System& system, json::Json& r) {
    const auto& tpms =
        agent_framework::module::get_manager<agent_framework::model::TrustedModule>().get_entries(system.get_uuid());

    std::set<agent_framework::model::enums::InterfaceType> available_interface_types{};
    for (const auto& trusted_module : tpms) {
        json::Json tpm = json::Json();
        endpoint::status_to_json(trusted_module, tpm);

        tpm[Common::ODATA_TYPE] = "#ComputerSystem.v1_3_0.TrustedModules";
        tpm[Common::FIRMWARE_VERSION] = trusted_module.get_firmware_version();
        tpm[System::INTERFACE_TYPE] = trusted_module.get_interface_type();
        tpm[System::FIRMWARE_VERSION_2] = json::Json::value_t::null;
        r[System::TRUSTED_MODULES].push_back(std::move(tpm));

        if (trusted_module.get_interface_type().has_value()) {
            available_interface_types.insert(trusted_module.get_interface_type().value());
        }
    }
}

void make_children_links(const server::Request& req, json::Json& response) {
    using psme::rest::endpoint::PathBuilder;
    response[constants::System::PROCESSORS][Common::ODATA_ID] =
        PathBuilder(req).append(constants::System::PROCESSORS).build();
    response[constants::System::ETHERNET_INTERFACES][Common::ODATA_ID] =
        PathBuilder(req).append(constants::System::ETHERNET_INTERFACES).build();
    response[constants::System::STORAGE][Common::ODATA_ID] =
        PathBuilder(req).append(constants::System::STORAGE).build();
    response[constants::System::MEMORY][Common::ODATA_ID] =
        PathBuilder(req).append(constants::System::MEMORY).build();
}

void make_parent_links(const agent_framework::model::System& system, json::Json& response) {
    // find manager of this system
    try {
        if (system.get_manager().has_value()) {
            json::Json managed_by = json::Json();
            managed_by[Common::ODATA_ID] = psme::rest::endpoint::utils::get_component_url(
                agent_framework::model::enums::Component::Manager, system.get_manager());
            response[Common::LINKS][Common::MANAGED_BY].push_back(managed_by);
        }
    }
    catch (const agent_framework::exceptions::InvalidUuid&) {
    }

    // find chassis containing this system
    try {
        if (system.get_chassis().has_value()) {
            json::Json chassis = json::Json();
            chassis[Common::ODATA_ID] =
                psme::rest::endpoint::utils::get_component_url(agent_framework::model::enums::Component::Chassis,
                                                               system.get_chassis());
            response[Common::LINKS][constants::Common::CHASSIS].push_back(chassis);
        }
    }
    catch (const agent_framework::exceptions::InvalidUuid&) {
    }
}
} // namespace

endpoint::System::System(const std::string& path) : EndpointBase(path) {}

endpoint::System::~System() {}

const endpoint::System::AllowableResetTypes& endpoint::System::get_allowed_reset_types() {
    static endpoint::System::AllowableResetTypes allowed_reset_types{
        agent_framework::model::enums::ResetType::GracefulShutdown,
        agent_framework::model::enums::ResetType::GracefulRestart};
    return allowed_reset_types;
}

void endpoint::System::get(const server::Request& request, server::Response& response) {
    auto r = make_prototype();
    r[Common::ODATA_ID] = PathBuilder(request).build();

    auto system = psme::rest::model::find<agent_framework::model::System>(request.params).get();

    make_parent_links(system, r);

    r[constants::Common::ODATA_ID] = PathBuilder(request).build();
    r[constants::Common::ID] = request.params[PathParam::SYSTEM_ID];
    utils::fill_name_and_description(system, r);
    r[constants::System::SYSTEM_TYPE] = system.get_system_type();
    r[Common::MANUFACTURER] = system.get_fru_info().get_manufacturer();
    r[Common::MODEL] = system.get_fru_info().get_model_number();
    r[Common::SERIAL_NUMBER] = system.get_fru_info().get_serial_number();
    r[Common::PART_NUMBER] = system.get_fru_info().get_part_number();
    r[constants::Common::UUID] = system.get_guid();

    endpoint::status_to_json(system, r);

    r[constants::System::POWER_STATE] = system.get_power_state();
    r[constants::System::BIOS_VERSION] = system.get_bios_version();
    r[constants::Common::SKU] = system.get_sku();
    r[constants::Common::ASSET_TAG] = system.get_asset_tag();
    r[constants::System::INDICATOR_LED] = system.get_indicator_led();
    r[constants::System::BOOT][constants::System::BOOT_SOURCE_OVERRIDE_TARGET] =
        system.get_boot_override_target();

    r[constants::System::BOOT][constants::System::BOOT_SOURCE_OVERRIDE_ENABLED] =
        system.get_boot_override();

    r[constants::System::BOOT][constants::System::BOOT_SOURCE_OVERRIDE_MODE] =
        system.get_boot_override_mode();

    for (const auto& allowable : system.get_boot_override_supported()) {
        r[constants::System::BOOT][constants::System::BOOT_SOURCE_OVERRIDE_TARGET_ALLOWABLE_VALUES]
            .push_back(allowable.to_string());
    }

    for (const auto& allowable : system.get_boot_mode_supported()) {
        r[constants::System::BOOT][constants::System::BOOT_SOURCE_OVERRIDE_MODE_ALLOWABLE_VALUES]
            .push_back(allowable.to_string());
    }

    add_processors_summary(system, r);
    add_memory_summary(system, r);
    add_system_relations(system, r);
    make_children_links(request, r);
    fill_tpm_data(system, r);

    auto system_type = system.get_system_type();
    if (system_type == agent_framework::model::enums::SystemType::Physical ||
        system_type == agent_framework::model::enums::SystemType::DPU) {
        add_reset_action(request, r);
    }

    r[constants::Common::VIRTUAL_MEDIA][Common::ODATA_ID] =
        PathBuilder(request).append(constants::Common::VIRTUAL_MEDIA).build();

    set_response(response, r);
}

void endpoint::System::patch(const server::Request& request, server::Response& response) {
    auto system = psme::rest::model::find<agent_framework::model::System>(request.params).get();
    const auto& json = JsonValidator::validate_request_body<schema::SystemPatchSchema>(request);
    const auto& json_boot = json[constants::System::BOOT];

    if (!(json_boot.contains(constants::System::BOOT_SOURCE_OVERRIDE_ENABLED) ||
          json_boot.contains(constants::System::BOOT_SOURCE_OVERRIDE_TARGET))) {
        response.set_status(server::status_2XX::NO_CONTENT);
        return;
    }

    if (json_boot.contains(constants::System::BOOT_SOURCE_OVERRIDE_TARGET)) {
        const auto& boot_source_override_target = json_boot[constants::System::BOOT_SOURCE_OVERRIDE_TARGET];
        const auto& boot_source_override_target_enum = enums::BootOverrideTarget::from_string(boot_source_override_target);

        if (!JsonValidator::validate_allowable_values(system.get_boot_override_supported().get_array(),
                                                      boot_source_override_target_enum)) {
            throw error::ServerException(
                error::ErrorFactory::create_error_for_not_allowable_value(
                    "Requested boot source override is not supported.",
                    constants::System::BOOT_SOURCE_OVERRIDE_TARGET, boot_source_override_target,
                    endpoint::PathBuilder(constants::System::BOOT)
                        .append(constants::System::BOOT_SOURCE_OVERRIDE_TARGET_ALLOWABLE_VALUES)
                        .build()));
        }
    }

    Context::get_instance()->service->set_acc_boot_options(json);

    response.set_status(server::status_2XX::NO_CONTENT);
}
