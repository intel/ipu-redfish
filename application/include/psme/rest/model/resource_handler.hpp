/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/utils/optional_field.hpp"

namespace psme {
namespace rest {

class ResourceHandler {
public:
    ResourceHandler() = default;
    virtual ~ResourceHandler();

    virtual void health_rollup(const std::string& parent_uuid, agent_framework::module::utils::OptionalField<agent_framework::model::enums::Health>& health) const = 0;
};

} // namespace rest
} // namespace psme
