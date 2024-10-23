/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/loader.hpp"
#include "agent-framework/database/database_entity.hpp"
#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/model/manager.hpp"
#include "ipu/acc_boot_option_handler.hpp"
#include "ipu/firmware_build_getter.hpp"
#include "ipu/ipu_constants.hpp"

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
    acc.set_system_type(agent_framework::model::enums::SystemType::DPU);
    acc.set_unique_key(acc.get_name());
    acc.make_persistent_uuid();

    IdPolicy<Component::System, NumberingZone::SHARED> system_id_policy;
    acc.set_id(system_id_policy.get_id(acc.get_uuid(), ""));

    Manager imc{};
    imc.set_name("IMC");
    imc.set_description("Integrated Management Complex");
    imc.set_unique_key(imc.get_name());
    imc.make_persistent_uuid();
    imc.set_firmware_version(FirmwareBuildGetter().value());

    IdPolicy<Component::Manager, NumberingZone::SHARED> manager_id_policy;
    imc.set_id(manager_id_policy.get_id(imc.get_uuid(), ""));

    imc.set_allowed_reset_actions(Manager::AllowedResetActions{agent_framework::model::enums::ResetType::ForceRestart});

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

    AccBootOptionHandler handler;
    handler.init();
}
