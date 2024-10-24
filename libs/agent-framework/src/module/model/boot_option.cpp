/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "agent-framework/module/model/boot_option.hpp"

using namespace agent_framework::model;

const enums::Component BootOption::component = enums::Component::BootOption;

BootOption::BootOption(const std::string& parent_uuid, enums::Component parent_type) : Resource{parent_uuid, parent_type} {}

BootOption::~BootOption() {}
