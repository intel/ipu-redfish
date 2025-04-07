/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/virtual_media_eject_handler.hpp"
#include "agent-framework/database/database_entity.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "agent-framework/module/model/virtual_media.hpp"
#include "ipu/ipu_constants.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/server/error/error_factory.hpp"

#include <filesystem>

using namespace agent_framework::module;
using namespace agent_framework::model;
using namespace agent_framework::database;
using namespace psme::rest::error;

namespace psme {
namespace ipu {

VirtualMediaEjectHandler::VirtualMediaEjectHandler() : m_virtual_media(get_virtual_media_resource()) {
}

void VirtualMediaEjectHandler::remove_image() {
    if (!m_virtual_media->get_inserted()) {
        throw ServerException(ErrorFactory::create_action_not_supported_error(psme::rest::constants::Common::VIRTUAL_MEDIA_EJECT, "The virtual media is not inserted so it cannot be ejected."));
    }

    std::filesystem::remove(psme::ipu::constants::IMAGE_SYMLINK);
    std::filesystem::remove(psme::ipu::constants::IMAGE_PATH);
    log_info("ipu", "The virtual media image has been successfully removed");
}

void VirtualMediaEjectHandler::update_virtual_media() {
    m_virtual_media->set_image_name({});
    m_virtual_media->set_inserted(false);

    DatabaseEntity<psme::ipu::constants::VIRTUAL_MEDIA_ENTITY_NAME> entity(m_virtual_media->get_uuid());
    entity.del(psme::ipu::constants::DOWNLOADED_IMAGE_NAME);
}

GenericManager<VirtualMedia>::Reference VirtualMediaEjectHandler::get_virtual_media_resource() {
    return get_manager<VirtualMedia>().get_only_reference();
}

} // namespace ipu
} // namespace psme
