/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "psme/rest/constants/constants_templates.hpp"

#include "psme/rest/constants/constants.hpp"

namespace psme {
namespace rest {
namespace constants {

template <>
const char* get_resource_id<agent_framework::model::System>() {
    return PathParam::SYSTEM_ID;
}

template <>
const char* get_resource_id<agent_framework::model::Manager>() {
    return PathParam::MANAGER_ID;
}

template <>
const char* get_resource_id<agent_framework::model::Task>() {
    return PathParam::TASK_ID;
}

template <>
const char* get_resource_id<agent_framework::model::VirtualMedia>() {
    return PathParam::VIRTUAL_MEDIA_ID;
}

} // namespace constants
} // namespace rest
} // namespace psme
