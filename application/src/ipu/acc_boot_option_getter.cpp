/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/acc_boot_option_getter.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "agent-framework/module/model/system.hpp"
#include "ipu/acc_boot_option_getter.hpp"
#include "ipu/ipu_constants.hpp"
#include "psme/rest/constants/system.hpp"
#include <fstream>

using namespace agent_framework::module;
using namespace agent_framework::model;
using namespace agent_framework::model::enums;
using namespace psme::rest;

namespace psme {
namespace ipu {

void AccBootOptionGetter::run() {
    json::Json file_content(json::Json::value_t::object);

    // TODO: add a lock mechanism and revise file check
    try {
        if (!std::filesystem::exists(constants::ACC_BOOT_OPTION_FILEPATH)) {
            log_debug("ipu", "Boot option file " << constants::ACC_BOOT_OPTION_FILEPATH << " is not present.");
            return;
        }
        /* try to parse file */
        std::ifstream boot_option_file(constants::ACC_BOOT_OPTION_FILEPATH);
        file_content = json::Json::parse(boot_option_file);
    }
    catch (const json::Json::parse_error&) {
        log_error("ipu", "Failed to parse boot option file " << constants::ACC_BOOT_OPTION_FILEPATH << ". The file is not JSON-compliant.");
        return;
    }

    get_boot_type(file_content);

    get_boot_state(file_content);
}

void AccBootOptionGetter::get_boot_state(const json::Json& file_config) {
    auto& system_manager = get_manager<System>();
    auto uuids = system_manager.get_keys();

    if (uuids.empty()) {
        return;
    }

    auto system = system_manager.get_entry_reference(uuids[0]);

    if (!file_config.contains(constants::STATE)) {
        system->set_boot_override(BootOverride::Disabled);
        return;
    }

    auto& state = file_config[constants::STATE];

    switch (enums::State::from_string(state)) {
    case enums::State::OneTimeOverride:
        system->set_boot_override(BootOverride::Once);
        break;
    case enums::State::ContinuousOverride:
        system->set_boot_override(BootOverride::Continuous);
        break;
    default:
        system->set_boot_override(BootOverride::Disabled);
        break;
    }
}

void AccBootOptionGetter::get_boot_type(const json::Json& file_config) {
    auto& system_manager = get_manager<System>();
    auto uuids = system_manager.get_keys();

    if (uuids.empty()) {
        return;
    }

    auto system = system_manager.get_entry_reference(uuids[0]);

    if (!file_config.contains(constants::BOOT_TYPE)) {
        system->set_boot_override_target(BootOverrideTarget::None);
        return;
    }

    const auto& boot_type = file_config[constants::BOOT_TYPE];

    switch (enums::BootType::from_string(boot_type)) {
    case enums::BootType::DramBoot:
        system->set_boot_override_target(BootOverrideTarget::Cd);
        break;
    default:
        system->set_boot_override_target(BootOverrideTarget::None);
        break;
    }
}

void AccBootOptionGetter::get_error(const json::Json& file_config) {
    (void)file_config;
}

} // namespace ipu
} // namespace psme
