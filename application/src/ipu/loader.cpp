/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/loader.hpp"
#include "agent-framework/database/database_entity.hpp"
#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/enum/compute.hpp"
#include "agent-framework/module/model/manager.hpp"
#include "ipu/firmware_build_getter.hpp"
#include "ipu/ipu_constants.hpp"
#include "ipu/ipu_update_handler.hpp"

// TODO: fix cyclic dependency
#include "model/handlers/id_policy.hpp"

#include "agent-framework/module/common_components.hpp"

using namespace psme::ipu;

using namespace agent_framework::model;
using namespace agent_framework::module;
using namespace agent_framework::database;
using namespace agent_framework::model::enums;
using namespace psme::rest::model::handler;
using agent_framework::module::CommonComponents;

void Loader::load() {
    System acc{};
    acc.set_name("ACC");
    acc.set_description("Acceleration Compute Complex");
    acc.set_system_type(SystemType::DPU);
    acc.set_status({State::Enabled, Health::OK});
    acc.add_boot_override_supported(BootOverride::Once);
    acc.add_boot_override_supported(BootOverride::Continuous);
    acc.add_boot_override_target_supported(BootOverrideTarget::Cd);
    acc.add_boot_override_target_supported(BootOverrideTarget::RemoteDrive);
    acc.add_boot_override_target_supported(BootOverrideTarget::Utilities);
    acc.add_boot_override_target_supported(BootOverrideTarget::UefiShell);
    acc.set_boot_override_mode(BootOverrideMode::UEFI);
    // set initial values in case the boot override config file doesn't exist
    acc.set_boot_override(BootOverride::Disabled);
    acc.set_boot_override_target(BootOverrideTarget::None);
    acc.set_unique_key(acc.get_name());
    acc.make_persistent_uuid();

    IdPolicy<Component::System, NumberingZone::SHARED> system_id_policy;
    acc.set_id(system_id_policy.get_id(acc.get_uuid(), ""));

    Manager imc{};
    imc.set_name("IMC");
    imc.set_description("Integrated Management Complex");
    imc.set_status({State::Enabled, Health::OK});
    imc.set_unique_key(imc.get_name());
    imc.make_persistent_uuid();
    imc.set_firmware_version(FirmwareBuildGetter().value());

    InventoryVersion inventory = IpuUpdateHandler().get_component_info();
    imc.set_board_id_version(inventory.board_id_version);
    imc.set_boot_image_version(inventory.boot_image_version);
    imc.set_imc_version(inventory.imc_version);
    imc.set_imc_orom_version(inventory.imc_orom_version);
    imc.set_recovery_imc_version(inventory.recovery_imc_version);
    acc.set_bios_version(inventory.acc_bios_version);

    IdPolicy<Component::Manager, NumberingZone::SHARED> manager_id_policy;
    imc.set_id(manager_id_policy.get_id(imc.get_uuid(), ""));

    imc.set_allowed_reset_actions(Manager::AllowedResetActions{ResetType::ForceRestart});

    acc.set_manager(imc.get_uuid());

    get_manager<System>().add_entry(acc);
    get_manager<Manager>().add_entry(imc);

    VirtualMedia media(acc.get_uuid());
    media.set_name("VirtualMedia");
    media.set_description("Virtual Media");
    media.set_unique_key(media.get_name());
    media.make_persistent_uuid();
    OptionalField<std::string> image_name;
    DatabaseEntity<psme::ipu::constants::VIRTUAL_MEDIA_ENTITY_NAME> db(media.get_uuid());
    try {
        image_name = db.get(psme::ipu::constants::DOWNLOADED_IMAGE_NAME);
        if (std::filesystem::exists(psme::ipu::constants::IMAGE_PATH)) {
            log_debug("ipu", "Detected previously inserted virtual media image: " << image_name);
            media.set_image_name(image_name);
            media.set_inserted(true);
        } else {
            log_info("ipu", "Previously inserted virtual media image is missing in the filesystem");
            db.del(psme::ipu::constants::DOWNLOADED_IMAGE_NAME);
        }
    }
    catch (const std::out_of_range&) {
        log_debug("ipu", "There is no ACC image name in the database");
    }
    IdPolicy<Component::VirtualMedia, NumberingZone::PARENT_SPACE> media_id_policy;
    media.set_id(media_id_policy.get_id(media.get_uuid(), acc.get_uuid()));
    get_manager<agent_framework::model::VirtualMedia>().add_entry(media);
}
