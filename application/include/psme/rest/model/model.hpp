/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "psme/rest/model/resource_handler.hpp"

#include "agent-framework/generic/singleton.hpp"

#include "logger/logger.hpp"

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

namespace psme {
namespace rest {

class Model : public agent_framework::generic::Singleton<Model> {
public:
    using Component = agent_framework::model::enums::Component;

    /*!
     * @brief Get Health Rollup of a resource by examining it and the resources
     * underneath it in hierarchy
     * @param[in] resource to examine
     * @param[in] filter - if it is not None, the rollup procedure only
     * analyzes subcomponents of the specific Component type
     *
     * @return the health rollup result
     * */
    template <typename Resource>
    agent_framework::module::utils::OptionalField<agent_framework::model::enums::Health> get_health_rollup(const Resource& resource, const Component filter) const;

    /*!
     * @brief Get Health Rollup of a resource by examining it and the resources
     * underneath it in hierarchy
     * @param[in]     resource to examine
     * @param[in,out] health the variable holding the rollup result
     * @param[in]     filter - if it is not None, the rollup procedure only
     * analyzes subcomponents of the specific Component type
     * */
    template <typename Resource>
    void rollup_health(const Resource& resource, agent_framework::module::utils::OptionalField<agent_framework::model::enums::Health>& health, const Component filter = Component::None) const;

    Model();
    virtual ~Model();
private:
    // the maps below are actually using Component as key

    using HandlerPtr = std::shared_ptr<ResourceHandler>;
    using Handlers = std::unordered_map<uint32_t, HandlerPtr>;
    const Handlers m_handlers;

    // map of subcomponent types that may appear for a component
    using Hierarchy = std::unordered_map<uint32_t, std::vector<uint32_t>>;
    const Hierarchy m_hierarchy;
};

template <typename Resource>
agent_framework::module::utils::OptionalField<agent_framework::model::enums::Health> Model::get_health_rollup(const Resource& resource, const Component filter) const {
    agent_framework::module::utils::OptionalField<agent_framework::model::enums::Health> health{};
    rollup_health(resource, health, filter);
    return health;
}

template <typename Resource>
void Model::rollup_health(const Resource& resource, agent_framework::module::utils::OptionalField<agent_framework::model::enums::Health>& health, const Component filter) const {
    const auto this_component = resource.get_component();
    if (m_hierarchy.count(this_component) > 0) {
        for (const auto& component : m_hierarchy.at(this_component)) {
            if (filter == Component::None || filter == component) {
                // rollup the health of child resources
                try {
                    m_handlers.at(component)->health_rollup(resource.get_uuid(), health);
                }
                catch (std::out_of_range& oor) {
                    // a missing handler should *not* happen in production,
                    // nevertheless the exception is caught to avoid crashes
                    log_error("model", "could not find handler for component " << Component(Component::Component_enum(component)).to_string());
                }
            }
        }
    }
    if (filter != Component::None) {
        // do not rollup this resource's health, just its children of specific type.
        return;
    }
    using agent_framework::model::enums::Health;
    // the following code assumes that the Health enum values have a specific order.
    static_assert(Health::OK < Health::Warning, "Unexpected Health values ordering");
    static_assert(Health::Warning < Health::Critical, "Unexpected Health values ordering");
    const auto& resource_health = resource.get_status().get_health();
    if (resource_health.has_value() && health.has_value()) {
        if (resource_health.value() > health.value()) {
            health = resource_health; // return Critical over Warning over OK
        }
    } else if (resource_health.has_value()) {
        health = resource_health;
    }
}

} // namespace rest
} // namespace psme
