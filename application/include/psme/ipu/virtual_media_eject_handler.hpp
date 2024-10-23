/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/managers/generic_manager.hpp"
#include "agent-framework/module/model/virtual_media.hpp"
#include <string>

namespace psme {
namespace ipu {

/*!
 * @brief Virtual Media Eject Handler declaration
 */
class VirtualMediaEjectHandler {
public:
    VirtualMediaEjectHandler();
    ~VirtualMediaEjectHandler() = default;
    void remove_image();
    void update_virtual_media();
private:
    agent_framework::module::GenericManager<agent_framework::model::VirtualMedia>::Reference get_virtual_media_resource();
private:
    agent_framework::module::GenericManager<agent_framework::model::VirtualMedia>::Reference m_virtual_media;
};

} // namespace ipu
} // namespace psme
