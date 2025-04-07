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
    response[Common::ODATA_TYPE] = "#ComputerSystem.v1_23_0.ComputerSystem";
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
    boot[Common::ODATA_TYPE] = "#ComputerSystem.v1_14_0.Boot";
    boot[System::BOOT_SOURCE_OVERRIDE_ENABLED] = json::Json::value_t::null;
    boot[System::BOOT_SOURCE_OVERRIDE_ENABLED_ALLOWABLE_VALUES] = json::Json::value_t::array;
    boot[System::BOOT_SOURCE_OVERRIDE_TARGET] = json::Json::value_t::null;
    boot[System::BOOT_SOURCE_OVERRIDE_TARGET_ALLOWABLE_VALUES] = json::Json::value_t::array;
    boot[System::BOOT_SOURCE_OVERRIDE_MODE] = json::Json::value_t::null;

    response[System::BOOT] = std::move(boot);

    response[Common::LINKS][Common::ODATA_TYPE] = "#ComputerSystem.v1_19_0.Links";
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

    for (const auto& allowable : system.get_boot_override_target_supported()) {
        r[constants::System::BOOT][constants::System::BOOT_SOURCE_OVERRIDE_TARGET_ALLOWABLE_VALUES]
            .push_back(allowable.to_string());
    }

    for (const auto& allowable : system.get_boot_override_supported()) {
        r[constants::System::BOOT][constants::System::BOOT_SOURCE_OVERRIDE_ENABLED_ALLOWABLE_VALUES]
            .push_back(allowable.to_string());
    }

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
    if (json_boot.contains(constants::System::BOOT_SOURCE_OVERRIDE_ENABLED)) {
        const auto& override = enums::BootOverride::from_string(json_boot[constants::System::BOOT_SOURCE_OVERRIDE_ENABLED]);

        if (!JsonValidator::validate_allowable_values(system.get_boot_override_supported().get_array(),
                                                      override)) {
            throw error::ServerException(
                error::ErrorFactory::create_error_for_not_allowable_value(
                    "Requested boot override is not supported.",
                    constants::System::BOOT_SOURCE_OVERRIDE_ENABLED, override.to_string(),
                    endpoint::PathBuilder(constants::System::BOOT)
                        .append(constants::System::BOOT_SOURCE_OVERRIDE_ENABLED_ALLOWABLE_VALUES)
                        .build()));
        }
    }

    if (json_boot.contains(constants::System::BOOT_SOURCE_OVERRIDE_TARGET)) {
        const auto& target = enums::BootOverrideTarget::from_string(json_boot[constants::System::BOOT_SOURCE_OVERRIDE_TARGET]);

        if (!JsonValidator::validate_allowable_values(system.get_boot_override_target_supported().get_array(),
                                                      target)) {
            throw error::ServerException(
                error::ErrorFactory::create_error_for_not_allowable_value(
                    "Requested boot source override is not supported.",
                    constants::System::BOOT_SOURCE_OVERRIDE_TARGET, target.to_string(),
                    endpoint::PathBuilder(constants::System::BOOT)
                        .append(constants::System::BOOT_SOURCE_OVERRIDE_TARGET_ALLOWABLE_VALUES)
                        .build()));
        }
    }

    OptionalField<enums::BootOverride> override{};
    OptionalField<enums::BootOverrideTarget> target{};

    if (json_boot.contains(constants::System::BOOT_SOURCE_OVERRIDE_ENABLED)) {
        override = enums::BootOverride::from_string(json_boot[constants::System::BOOT_SOURCE_OVERRIDE_ENABLED]);
    }

    if (json_boot.contains(constants::System::BOOT_SOURCE_OVERRIDE_TARGET)) {
        target = enums::BootOverrideTarget::from_string(json_boot[constants::System::BOOT_SOURCE_OVERRIDE_TARGET]);
    }

    Context::get_instance()->service->set_acc_boot_override(std::move(override), std::move(target));

    response.set_status(server::status_2XX::NO_CONTENT);
}
