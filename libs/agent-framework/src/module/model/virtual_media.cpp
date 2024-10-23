/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "agent-framework/module/model/virtual_media.hpp"

using namespace agent_framework::model;

const enums::Component VirtualMedia::component = enums::Component::VirtualMedia;

VirtualMedia::VirtualMedia(const std::string& parent_uuid, enums::Component parent_type) : Resource{parent_uuid, parent_type} {}

VirtualMedia::~VirtualMedia() {}
