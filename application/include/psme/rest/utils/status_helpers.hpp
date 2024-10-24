/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2016-2019 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * */

#pragma once

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/model/model.hpp"

namespace psme {
namespace rest {
namespace endpoint {

/*!
 * @brief Derives rollup health from subtree of resources
 *
 */
template <typename T>
class HealthRollup {
public:
    using Component = agent_framework::model::enums::Component;

    HealthRollup() {}

    /*!
     * @brief Computes Health rollup starting from resource
     *
     * @param[in] resource resource for which health rollup is to be computed
     * @param[in] filter Optional parameter that limits rollup computation to given component type
     * @return Health Rollup optional
     */
    agent_framework::module::utils::OptionalField<agent_framework::model::enums::Health> get(
        const T& resource,
        const Component filter = Component::None) {
        return psme::rest::Model::get_instance()->get_health_rollup(resource, filter);
    }
};

/*!
 * @brief Get resource status (state and health) and add it to GET reply JSON
 *
 * @param resource a Resource
 * @param v the json value sent as a GET response
 */
template <typename T>
void status_to_json(const T& resource, json::Json& v, bool calculate_rollup = true) {
    using namespace psme::rest;
    auto& status = resource.get_status();
    v[constants::Common::STATUS][constants::Common::STATE] = status.get_state().to_string();
    v[constants::Common::STATUS][constants::Common::HEALTH] = status.get_health();
    if (calculate_rollup) {
        v[constants::Common::STATUS][constants::Common::HEALTH_ROLLUP] = HealthRollup<T>().get(resource);
    }
}

} // namespace endpoint
} // namespace rest
} // namespace psme
