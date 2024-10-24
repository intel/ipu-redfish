/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/acc_boot_option_handler.hpp"
#include "agent-framework/module/enum/compute.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "agent-framework/module/model/system.hpp"

using namespace agent_framework::module;
using namespace agent_framework::model;

namespace psme {
namespace ipu {

void AccBootOptionHandler::init() {
    auto& system_manager = get_manager<System>();
    auto uuids = system_manager.get_keys();

    if (uuids.empty()) {
        return;
    }

    auto system = system_manager.get_entry_reference(uuids[0]);

    system->add_boot_override_supported(enums::BootOverrideTarget::None);
    system->add_boot_override_supported(enums::BootOverrideTarget::Cd);

    system->add_boot_mode_supported(enums::BootOverrideMode::UEFI);

    system->set_boot_override_mode(enums::BootOverrideMode::UEFI);

    // set initial values in case the config file doesn't exist
    system->set_boot_override(enums::BootOverride::Disabled);
    system->set_boot_override_target(enums::BootOverrideTarget::None);

    m_getter.run();
}

void AccBootOptionHandler::set(const json::Json& redfish_config) {
    m_setter.run(redfish_config);
    /* Notify service thread about changes */
}

void AccBootOptionHandler::get() {
    m_getter.run();
}

} // namespace ipu
} // namespace psme
