/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/acc_boot_option_setter.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "agent-framework/module/model/virtual_media.hpp"
#include "ipu/ipu_constants.hpp"
#include "ipu/reserved_memory_json.hpp"
#include "psme/rest/constants/system.hpp"
#include <filesystem>
#include <fstream>
#include <system_error>

using namespace agent_framework::model;
using namespace agent_framework::model::enums;
using namespace psme::rest::constants;
using namespace psme::rest;

namespace psme {
namespace ipu {

void AccBootOptionSetter::run(const json::Json& redfish_config) {
    json::Json file_content(json::Json::value_t::object);

    // TODO: add a lock mechanism and revise file check
    try {
        /* try to parse existing file */
        std::ifstream boot_option_file(constants::ACC_BOOT_OPTION_FILEPATH);
        file_content = json::Json::parse(boot_option_file);
    }
    catch (const std::exception& e) {
        log_error("ipu", "Failed to parse " << constants::ACC_BOOT_OPTION_FILEPATH << " " << e.what() << ". Trying to make a new file");
        make_file_content(file_content);
    }

    update_file_content(redfish_config, file_content);

    if (file_content.contains(constants::STATE) &&
        file_content.contains(constants::BOOT_TYPE)) {
        auto state = enums::State::from_string(file_content[constants::STATE]);
        auto boot_type = enums::BootType::from_string(file_content[constants::BOOT_TYPE]);

        if (boot_type == enums::BootType::DramBoot &&
            (state == enums::State::OneTimeOverride || state == enums::State::ContinuousOverride)) {
            prepare_acc_iso_dram_boot();
        }
    } else {
        prepare_acc_iscsi_boot();
    }

    try {
        std::ofstream boot_option_file(constants::ACC_BOOT_OPTION_FILEPATH);
        boot_option_file << file_content.dump(2);
    }
    catch (const std::exception& ex) {
        throw std::runtime_error(std::string("Cannot create boot option file ") + constants::ACC_BOOT_OPTION_FILEPATH + " " + ex.what());
    }
}

void AccBootOptionSetter::make_file_content(json::Json& file_config) {
    file_config[constants::ERROR] = "";
    file_config[constants::IP_VERSION] = "";
    file_config[constants::VPORT_ID] = 0;
}

void AccBootOptionSetter::update_file_content(const json::Json& redfish_config, json::Json& file_config) {
    const json::Json& json_boot = redfish_config[System::BOOT];

    if (json_boot.contains(System::BOOT_SOURCE_OVERRIDE_TARGET)) {
        update_boot_type(json_boot, file_config);
    }

    update_error(json_boot, file_config);

    if (json_boot.contains(System::BOOT_SOURCE_OVERRIDE_ENABLED)) {
        update_boot_state(json_boot, file_config);
    }
}

void AccBootOptionSetter::update_boot_state(const json::Json& redfish_config, json::Json& file_config) {
    auto& redfish_boot_override = redfish_config[System::BOOT_SOURCE_OVERRIDE_ENABLED];
    switch (BootOverride::from_string(redfish_boot_override)) {
    case BootOverride::Disabled:
        file_config.erase(constants::STATE);
        break;
    case BootOverride::Once:
        file_config[constants::STATE] = enums::State(enums::State::OneTimeOverride).to_string();
        break;
    case BootOverride::Continuous:
        file_config[constants::STATE] = enums::State(enums::State::ContinuousOverride).to_string();
        break;
    default:
        file_config.erase(constants::STATE);
        break;
    }
}

void AccBootOptionSetter::update_boot_type(const json::Json& redfish_config, json::Json& file_config) {
    auto& redfish_boot_override_target = redfish_config[System::BOOT_SOURCE_OVERRIDE_TARGET];

    switch (BootOverrideTarget::from_string(redfish_boot_override_target)) {
    case BootOverrideTarget::Cd:
        file_config[constants::BOOT_TYPE] = enums::BootType(enums::BootType::DramBoot).to_string();
        break;
    default:
        file_config.erase(constants::BOOT_TYPE);
        break;
    }
}

void AccBootOptionSetter::update_error(const json::Json& redfish_config, json::Json& file_config) {
    (void)redfish_config;
    file_config[constants::ERROR] = "";
}

void AccBootOptionSetter::prepare_acc_iso_dram_boot() {
    using namespace psme::ipu::constants;

#if defined __aarch64__
    int script_result = system("/usr/bin/scripts/set_acc_kernel_cmdline.sh -f -b ramdisk");
    if (script_result) {
        throw std::runtime_error("\"set_acc_kernel_cmdline.sh\" script failed.");
    }
#endif

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

    auto& virtual_media_manager = agent_framework::module::get_manager<VirtualMedia>();
    auto uuids = virtual_media_manager.get_keys();

    if (uuids.empty()) {
        throw std::runtime_error("Cannot locate virtual media resource");
    }

    auto virtual_media = virtual_media_manager.get_entry_reference(uuids[0]);

    if (!virtual_media->get_inserted()) {
        return;
    }

    directory_exists = std::filesystem::exists(IMAGE_SYMLINK_DIRECTORY);
    if (!directory_exists) {
        std::error_code ec{};
        std::filesystem::create_directory(IMAGE_SYMLINK_DIRECTORY, ec);
        if (ec) {
            throw std::runtime_error(std::string("Could not create directory for: ") + IMAGE_SYMLINK + ": " + ec.message());
        }
        log_info("ipu", IMAGE_SYMLINK_DIRECTORY << " has been successfully created.");
    }

    bool sym_exist = std::filesystem::is_symlink(IMAGE_SYMLINK);
    if (!sym_exist) {
        // only try to create the symlink if it didn't exist before
        std::error_code ec{};
        std::filesystem::create_symlink(IMAGE_PATH, IMAGE_SYMLINK, ec);
        if (ec) {
            throw std::runtime_error("The virtual media image symlink creation failed: " + ec.message());
        }
        log_info("ipu", "The virtual media image symlink has been successfully created.");
    } else {
        log_debug("ipu", "The virtual media image symlink already exists.");
    }
}

void AccBootOptionSetter::prepare_acc_iscsi_boot() {
    using namespace psme::ipu::constants;

#if defined __aarch64__
    int script_result = system("/usr/bin/imc-scripts/clear_bmd_config");
    if (script_result) {
        throw std::runtime_error("\"clear_bmd_config\" script failed.");
    }
    script_result = system("/usr/bin/imc-scripts/clear_acc_boot_options");
    if (script_result) {
        throw std::runtime_error("\"clear_acc_boot_options\" script failed.");
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
        log_debug("ipu", RESERVED_MEMORY_FILEPATH << " is not present");
    }

    bool sym_exist = std::filesystem::is_symlink(IMAGE_SYMLINK);
    if (!sym_exist) {
        // only try to remove the symlink if it exists
        log_debug("ipu", "The virtual media image symlink is not present.");
        return;
    }
    bool rem_res = std::filesystem::remove(IMAGE_SYMLINK);
    if (!rem_res) {
        throw std::runtime_error("The virtual media image symlink removal failed");
    }
    log_info("ipu", "The virtual media image symlink has been successfully removed.");
}

} // namespace ipu
} // namespace psme
