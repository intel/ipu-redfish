/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "psme/rest/model/model.hpp"
#include "psme/rest/model/resource_handler.hpp"

#include "agent-framework/module/managers/utils/manager_utils.hpp"

namespace psme {
namespace rest {

template <typename Resource>
class Handler : public ResourceHandler {
public:
    Handler() = default;

    virtual ~Handler() {}
    /*!
     * @brief Rolls up the health of all resources of the type handled by this Handler under a specific parent
     * @param[in]     parent_uuid - the uuid of the parent resource
     * @param[in,out] health the variable holding the rollup result
     * */
    virtual void health_rollup(const std::string& parent_uuid, agent_framework::module::utils::OptionalField<agent_framework::model::enums::Health>& health) const;
};

template <typename Resource>
void Handler<Resource>::health_rollup(const std::string& parent_uuid, agent_framework::module::utils::OptionalField<agent_framework::model::enums::Health>& health) const {
    const auto resources = agent_framework::module::get_manager<Resource>().get_entries(parent_uuid);
    for (const auto& resource : resources) {
        Model::get_instance()->rollup_health(resource, health);
    }
}

} // namespace rest
} // namespace psme
