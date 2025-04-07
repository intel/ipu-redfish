/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/acc_boot_override_handler.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "agent-framework/module/model/system.hpp"
#include "agent-framework/module/model/virtual_media.hpp"
#include "ipu/reserved_memory_json.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/server/error/server_exception.hpp"
#include <filesystem>
#include <fstream>

using namespace psme::ipu;
using namespace psme::ipu::constants;
using namespace psme::rest::error;
using namespace agent_framework::module;
using namespace agent_framework::model;
using namespace agent_framework::model::enums;

static json::Json OVERRIDE_CONFIG_SCHEMA = R"~(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "type": "object",
    "properties": {
        "BootType": {
            "type": "string",
            "enum": [
                "LocalIscsiBoot",
                "LocalIscsiBoot_ISO",
                "DramBoot",
                "InternalUefiShell",
                "Pxe",
                "Http",
                "Other"
            ]
        },
        "State": {
            "type": "string",
            "enum": [
                "ContinuousOverride",
                "OneTimeOverride",
                "OneTimeOverrideSynced",
                "OneTimeOverrideUpdated"
            ]
        },
        "IpVersion": {
            "type": "string"
        },
        "VPortId": {
            "type": "integer"
        },
        "Error": {
            "type": "string"
        }
    },
    "required": ["BootType", "State", "IpVersion", "VPortId", "Error"]
})~"_json;

static json::Json DEFAULT_OVERRIDE_CONFIG_JSON = R"~(
{
    "State": "ContinuousOverride",
    "BootType": "DramBoot",
    "IpVersion": "",
    "VportId": 0,
    "Error": ""
})~"_json;

std::optional<AccBootOverrideHandler::OverrideConfig> AccBootOverrideHandler::read_override_config() {
    json::Json json_config;
    try {
        if (!std::filesystem::exists(constants::ACC_BOOT_OVERRIDE_FILEPATH)) {
            log_debug("ipu", "Boot override file " << constants::ACC_BOOT_OVERRIDE_FILEPATH << " is not present.");
            return {};
        }
        /* try to parse the file */
        std::ifstream boot_override_file(constants::ACC_BOOT_OVERRIDE_FILEPATH);
        json_config = json::Json::parse(boot_override_file);
    }
    catch (const json::Json::parse_error&) {
        log_error("ipu", "Failed to parse boot override file " << constants::ACC_BOOT_OVERRIDE_FILEPATH << ". The file is not JSON-compliant.");
        return {};
    }

    nlohmann::json_schema::json_validator validator;
    validator.set_root_schema(::OVERRIDE_CONFIG_SCHEMA);

    try {
        validator.validate(json_config);
    }
    catch (const std::exception&) {
        log_error("ipu", "The boot override file " << constants::ACC_BOOT_OVERRIDE_FILEPATH << " does not match the schema.");
        return {};
    }

    OverrideConfig override_config;
    override_config.error = json_config[constants::ERROR];
    override_config.state = enums::State::from_string(json_config[constants::STATE]);
    override_config.boot_type = enums::BootType::from_string(json_config[constants::BOOT_TYPE]);

    return override_config;
}

bool AccBootOverrideHandler::has_error(const OverrideConfig& config) {
    return config.error.empty() ? false : (config.error != constants::NONE);
}

BootOverride AccBootOverrideHandler::state_to_redfish(enums::State state) {
    switch (state) {
    case enums::State::OneTimeOverride:
        return BootOverride::Once;
    case enums::State::ContinuousOverride:
        return BootOverride::Continuous;
    default:
        return BootOverride::Disabled;
    }
}

BootOverrideTarget AccBootOverrideHandler::boot_type_to_redfish(enums::BootType boot_type) {
    switch (boot_type) {
    case enums::BootType::InternalUefiShell:
        return BootOverrideTarget::UefiShell;
    case enums::BootType::LocalIscsiBoot:
        return BootOverrideTarget::RemoteDrive;
    case enums::BootType::DramBoot:
        // The user of this function needs to distinguish between
        // MFG (Utilities) and ISO (Cd) boot types based on other inputs,
        // as both of these options are present as DramBoot in the config file.
        return BootOverrideTarget::Utilities;
    default:
        return BootOverrideTarget::None;
    }
}

BootOverrideTarget AccBootOverrideHandler::detect_dram_boot_type() {
    if (!std::filesystem::exists(constants::ACC_BOOT_OPTION_FILEPATH)) {
        log_info("ipu", "Detected inconsistent boot override setting: boot option file "
                            << constants::ACC_BOOT_OPTION_FILEPATH << " is not present, while boot override file "
                            << constants::ACC_BOOT_OVERRIDE_FILEPATH << " is present.");
        return BootOverrideTarget::None;
    }

    json::Json json;
    try {
        /* try to parse the file */
        std::ifstream boot_option_file(constants::ACC_BOOT_OPTION_FILEPATH);
        json = json::Json::parse(boot_option_file);
    }
    catch (const json::Json::parse_error&) {
        log_error("ipu", "Failed to parse boot option file " << constants::ACC_BOOT_OPTION_FILEPATH << ". The file is not JSON-compliant.");
        return BootOverrideTarget::None;
    }

    // while the ACC_BOOT_OVERRIDE_FILEPATH file may be modified by the users manually
    // (the MEV-TS documentation suggests doing it),
    // ACC_BOOT_OPTION_FILEPATH file is only modified using scripts.
    // Therefore, we don't validate it against a schema

    auto option = enums::BootOption::from_string(json[CURRENT_BOOT_OPTION]);
    switch (option) {
    case enums::BootOption::ram:
        return BootOverrideTarget::Utilities;
    case enums::BootOption::ramdisk:
        return BootOverrideTarget::Cd;
    default: {
        log_info("ipu", "Detected inconsistent boot override setting: boot option file "
                            << constants::ACC_BOOT_OPTION_FILEPATH << " has setting " << json[CURRENT_BOOT_OPTION]
                            << " while " << constants::ACC_BOOT_OVERRIDE_FILEPATH << " is configured to DramBoot");
        return BootOverrideTarget::None;
    }
    }
}

void AccBootOverrideHandler::update_view(const std::optional<OverrideConfig>& config) {
    auto acc = get_manager<System>().get_only_reference();

    if (!config) {
        // lack of config is mapped to Disabled/None/OK
        acc->set_boot_override(BootOverride::Disabled);
        acc->set_boot_override_target(BootOverrideTarget::None);
        auto status = acc->get_status();
        status.set_health(Health::OK);
        acc->set_status(status);
        return;
    }

    if (has_error(*config)) {
        log_info("ipu", "Boot override file " << constants::ACC_BOOT_OVERRIDE_FILEPATH << " reports an error of the last boot override attempt: " << config->error);
        auto status = acc->get_status();
        status.set_health(Health::Critical);
        acc->set_status(status);
    } else {
        log_debug("ipu", "No error reported in boot override file " << constants::ACC_BOOT_OVERRIDE_FILEPATH);
        auto status = acc->get_status();
        status.set_health(Health::OK);
        acc->set_status(status);
    }

    auto state = state_to_redfish(config->state);
    acc->set_boot_override(state);

    if (state == BootOverride::Disabled) {
        // ignore boot type from the config if override is disabled
        acc->set_boot_override_target(BootOverrideTarget::None);
        return;
    }

    auto boot_type = boot_type_to_redfish(config->boot_type);
    if (boot_type == BootOverrideTarget::Utilities) {
        // distinguish MFG vs ISO
        acc->set_boot_override_target(detect_dram_boot_type());
    } else {
        acc->set_boot_override_target(boot_type);
    }
}

void AccBootOverrideHandler::create_memory_reservation_for_iso() {
    log_info("ipu", "Creating memory reservation for ISO");

    bool directory_exists = std::filesystem::exists(RESERVED_MEMORY_DIRECTORY);
    if (!directory_exists) {
        std::error_code ec{};
        std::filesystem::create_directory(RESERVED_MEMORY_DIRECTORY, ec);
        if (ec) {
            throw std::runtime_error(std::string("Could not create directory for: ") + RESERVED_MEMORY_FILEPATH + ": " + ec.message());
        }
        log_info("ipu", RESERVED_MEMORY_DIRECTORY << " has been successfully created.");
    }

    bool exists = std::filesystem::exists(RESERVED_MEMORY_FILEPATH);
    if (!exists) {
        try {
            std::ofstream bmd_override_file(RESERVED_MEMORY_FILEPATH);
            bmd_override_file << RESERVED_MEMORY;
            log_info("ipu", RESERVED_MEMORY_FILEPATH << " file has been successfully created.");
        }
        catch (const std::exception& ex) {
            throw std::runtime_error(std::string("Failed to create ") + RESERVED_MEMORY_FILEPATH + ": " + ex.what());
        }
    } else {
        log_debug("ipu", RESERVED_MEMORY_FILEPATH << " already exists");
    }
}

void AccBootOverrideHandler::clear_memory_reservation_for_iso() {
    log_info("ipu", "Removing memory reservation for ISO");

#ifdef INTEL_IPU
    int script_result = system("/usr/bin/imc-scripts/clear_bmd_config");
    if (script_result) {
        throw std::runtime_error("\"clear_bmd_config\" script failed.");
    }
#endif

    bool exists = std::filesystem::exists(RESERVED_MEMORY_FILEPATH);
    if (exists) {
        try {
            std::filesystem::remove(RESERVED_MEMORY_FILEPATH);
            log_info("ipu", RESERVED_MEMORY_FILEPATH << " file has been removed.");
        }
        catch (const std::exception& ex) {
            throw std::runtime_error(std::string("Failed to remove ") + RESERVED_MEMORY_FILEPATH + ": " + ex.what());
        }
    } else {
        log_info("ipu", "Attempted to remove memory reservation for ISO, but " << RESERVED_MEMORY_FILEPATH << " is not present");
    }
}

void AccBootOverrideHandler::update_memory_reservation_for_iso(const std::optional<OverrideConfig>& config) {
    auto acc = get_manager<System>().get_only();

    // only clear the reservation if previously there was an override set to Cd
    // and now it was disabled or changed to something else
    if (acc.get_boot_override_target() != BootOverrideTarget::Cd) {
        return;
    }

    if (acc.get_boot_override() == BootOverride::Disabled) {
        return;
    }

    if (!config) {
        return;
    }

    auto override_state = state_to_redfish(config->state);
    auto boot_type = boot_type_to_redfish(config->boot_type);
    if (boot_type == BootOverrideTarget::Utilities) {
        // distinguish MFG vs ISO
        boot_type = detect_dram_boot_type();
    }
    if (boot_type != BootOverrideTarget::Cd || override_state == BootOverride::Disabled) {
        clear_memory_reservation_for_iso();
    }
}

bool AccBootOverrideHandler::view_is_outdated(const std::optional<OverrideConfig>& config) {
    auto acc = get_manager<System>().get_only();

    if (!config) {
        if ((acc.get_boot_override() != BootOverride::Disabled) ||
            (acc.get_boot_override_target() != BootOverrideTarget::None) ||
            (acc.get_status().get_health() != Health::OK)) {
            return true;
        }
        return false;
    }

    if ((has_error(*config) && (acc.get_status().get_health() == Health::OK)) ||
        (!has_error(*config) && (acc.get_status().get_health() == Health::Critical))) {
        return true;
    }

    auto override_state = state_to_redfish(config->state);
    if (acc.get_boot_override() != override_state) {
        return true;
    }

    BootOverrideTarget boot_type{BootOverrideTarget::None};
    if (override_state == BootOverride::Disabled) {
        // ignore boot type from the config if override is disabled
        boot_type = BootOverrideTarget::None;
    } else {
        boot_type = boot_type_to_redfish(config->boot_type);
        if (boot_type == BootOverrideTarget::Utilities) {
            // distinguish MFG vs ISO
            boot_type = detect_dram_boot_type();
        }
    }
    if (acc.get_boot_override_target() != boot_type) {
        return true;
    }

    return false;
}

void AccBootOverrideHandler::check_request_validity(const OptionalField<BootOverride>& override,
                                                    const OptionalField<BootOverrideTarget>& target) {
    // if JSON was empty, the request is avalid no-op
    if (!target.has_value() && !override.has_value()) {
        return;
    }

    auto acc = get_manager<System>().get_only();

    // if only the target was provided, the override already needs to already have been
    // set to Once (for Cd target) or Continuous (for all possible targets)
    if (target.has_value() && !override.has_value()) {
        if (acc.get_boot_override() == BootOverride::Disabled) {
            throw ServerException(ErrorFactory::create_invalid_payload_error("Cannot set BootOverrideTarget if BootOverride is Disabled."));
        }
        if ((acc.get_boot_override() == BootOverride::Once) && (target != BootOverrideTarget::Cd)) {
            throw ServerException(ErrorFactory::create_invalid_payload_error("If BootOverride is Once, only Cd BootOverrideTarget is allowed."));
        }
        return;
    }

    // if only the override was provided, the target already needs to already have been
    // set to Cd (for Once) or any not None for Continuous
    if (override.has_value() && !target.has_value()) {
        if (acc.get_boot_override_target() == BootOverrideTarget::None) {
            throw ServerException(ErrorFactory::create_invalid_payload_error("Cannot set BootOverride if BootOverrideTarget is None."));
        }
        if ((acc.get_boot_override_target() != BootOverrideTarget::Cd) && (override == BootOverride::Once)) {
            throw ServerException(ErrorFactory::create_invalid_payload_error("BootOverride Once can only be set for BootOverrideTarget Cd."));
        }
        return;
    }

    // if both have values, the only valid combination for Once is with Cd
    if ((override == BootOverride::Once) && (target != BootOverrideTarget::Cd)) {
        throw ServerException(ErrorFactory::create_invalid_payload_error("BootOverride Once can only be set for BootOverrideTarget Cd."));
    }
}

void AccBootOverrideHandler::remove_iso_symlink() {
    bool rem_res = std::filesystem::remove(IMAGE_SYMLINK);
    if (!rem_res) {
        throw std::runtime_error("The virtual media image symlink removal failed");
    }
    log_info("ipu", "The virtual media image symlink has been successfully removed.");
}

void AccBootOverrideHandler::ensure_iso_symlink_presence() {
    auto virtual_media = agent_framework::module::get_manager<VirtualMedia>().get_only();

    if (!virtual_media.get_inserted()) {
        return;
    }

    bool directory_exists = std::filesystem::exists(IMAGE_SYMLINK_DIRECTORY);
    if (!directory_exists) {
        std::error_code ec{};
        std::filesystem::create_directory(IMAGE_SYMLINK_DIRECTORY, ec);
        if (ec) {
            throw std::runtime_error(std::string("Could not create directory for: ") + IMAGE_SYMLINK + ": " + ec.message());
        }
        log_info("ipu", IMAGE_SYMLINK_DIRECTORY << " has been successfully created.");
    }

    std::error_code ec{};
    std::filesystem::create_symlink(IMAGE_PATH, IMAGE_SYMLINK, ec);
    if (ec) {
        throw std::runtime_error("The virtual media image symlink creation failed: " + ec.message());
    }
    log_info("ipu", "The virtual media image symlink has been successfully created.");
}

void AccBootOverrideHandler::read_initial_state() {
    auto config = read_override_config();

    update_view(config);
}

void AccBootOverrideHandler::check_for_updates() {
    auto config = read_override_config();

    if (view_is_outdated(config)) {
        log_info("ipu", "Detected new state in " << constants::ACC_BOOT_OVERRIDE_FILEPATH << ". Updating.");
        update_memory_reservation_for_iso(config);
        update_view(config);
    }
}

void AccBootOverrideHandler::set(OptionalField<BootOverride> override,
                                 OptionalField<BootOverrideTarget> target) {
    check_request_validity(override, target);

    if (!override.has_value() && !target.has_value()) {
        // no-op
        return;
    }

    json::Json override_config;
    if (!std::filesystem::exists(constants::ACC_BOOT_OVERRIDE_FILEPATH)) {
        log_debug("ipu", "Boot override file " << constants::ACC_BOOT_OVERRIDE_FILEPATH << " is not present.");
        override_config = DEFAULT_OVERRIDE_CONFIG_JSON;
    } else {
        try {
            /* try to parse the file */
            std::ifstream boot_override_file(constants::ACC_BOOT_OVERRIDE_FILEPATH);
            override_config = json::Json::parse(boot_override_file);
        }
        catch (const json::Json::parse_error&) {
            log_error("ipu", "Failed to parse boot option file " << constants::ACC_BOOT_OVERRIDE_FILEPATH << ". The file is not JSON-compliant.");
            override_config = DEFAULT_OVERRIDE_CONFIG_JSON;
        }
    }

    auto acc = get_manager<System>().get_only_reference();
    if (override_config[ERROR] != "" && override_config[ERROR] != NONE) {
        log_info("ipu", "Clearing Critical status from ACC system");
        auto status = acc->get_status();
        status.set_health(Health::OK);
        acc->set_status(status);
        override_config[ERROR] = "";
    }

    bool has_iso_memory_reservation = (acc->get_boot_override_target() == BootOverrideTarget::Cd) && (acc->get_boot_override() != BootOverride::Disabled);
    bool iso_symlink_exists = std::filesystem::exists(IMAGE_SYMLINK);

    if (override.has_value()) {
        if (override == BootOverride::Once) {
            override_config[STATE] = enums::State(enums::State::OneTimeOverride).to_string();
        } else {
            override_config[STATE] = enums::State(enums::State::ContinuousOverride).to_string();
        }
        log_info("ipu", "Setting override state to " << override_config[STATE]);
        acc->set_boot_override(override);
    }

    if (target.has_value()) {
        switch (*target) {
        case BootOverrideTarget::Cd: {
#ifdef INTEL_IPU
            int script_result = system("/usr/bin/scripts/set_acc_kernel_cmdline.sh -f -b ramdisk");
            if (script_result) {
                throw std::runtime_error("\"set_acc_kernel_cmdline.sh\" script failed.");
            }
#endif

            if (!has_iso_memory_reservation) {
                create_memory_reservation_for_iso();
            }
            if (!iso_symlink_exists) {
                ensure_iso_symlink_presence();
            }

            override_config[BOOT_TYPE] = enums::BootType(enums::BootType::DramBoot).to_string();
            break;
        }
        case BootOverrideTarget::RemoteDrive: {
            if (has_iso_memory_reservation) {
                clear_memory_reservation_for_iso();
            }
#ifdef INTEL_IPU
            int script_result = system("/usr/bin/scripts/set_acc_kernel_cmdline.sh -f -b iscsi");
            if (script_result) {
                throw std::runtime_error("\"set_acc_kernel_cmdline.sh\" script failed.");
            }
#endif
            if (iso_symlink_exists) {
                remove_iso_symlink();
            }

            override_config[BOOT_TYPE] = enums::BootType(enums::BootType::LocalIscsiBoot).to_string();
            break;
        }
        case BootOverrideTarget::Utilities: {
#ifdef INTEL_IPU
            int script_result = system("/usr/bin/scripts/set_acc_kernel_cmdline.sh -f -b ram");
            if (script_result) {
                throw std::runtime_error("\"set_acc_kernel_cmdline.sh\" script failed.");
            }
#endif

            if (has_iso_memory_reservation) {
                clear_memory_reservation_for_iso();
            }
            if (iso_symlink_exists) {
                remove_iso_symlink();
            }

            override_config[BOOT_TYPE] = enums::BootType(enums::BootType::DramBoot).to_string();
            break;
        }
        case BootOverrideTarget::UefiShell: {
            if (has_iso_memory_reservation) {
                clear_memory_reservation_for_iso();
            }
            if (iso_symlink_exists) {
                remove_iso_symlink();
            }

            override_config[BOOT_TYPE] = enums::BootType(enums::BootType::InternalUefiShell).to_string();
            break;
        }
        default:
            break;
        }
        log_info("ipu", "Setting boot type to " << override_config[BOOT_TYPE]);
        acc->set_boot_override_target(target);
    }

    try {
        std::ofstream boot_option_file(constants::ACC_BOOT_OVERRIDE_FILEPATH);
        boot_option_file << override_config.dump(2);
    }
    catch (const std::exception& ex) {
        throw std::runtime_error(std::string("Could not update boot override file ") + constants::ACC_BOOT_OVERRIDE_FILEPATH + " " + ex.what());
    }
}
