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
 *
 * */

#include "agent-framework/module/compute_components.hpp"
#include "agent-framework/module/managers/generic_manager.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "agent-framework/module/model/attributes/status.hpp"
#include "agent-framework/module/model/manager.hpp"
#include "agent-framework/module/model/memory.hpp"
#include "agent-framework/module/model/processor.hpp"
#include "agent-framework/module/model/system.hpp"
#include "psme/rest/utils/status_helpers.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace agent_framework;
using namespace agent_framework::model;
using namespace agent_framework::module;
using namespace enums;

namespace psme {
namespace rest {
namespace endpoint {

using SystemManager = module::GenericManager<System>;
using MemoryManager = module::GenericManager<Memory>;
using ProcessorManager = module::GenericManager<Processor>;

class RollupTest : public ::testing::Test {
public:
    RollupTest() : systems(get_manager<System>()),
                   // yeah, I know this is not correct english
                   memories(get_manager<Memory>()),
                   processors(get_manager<Processor>()) {}

    ~RollupTest();

    void SetUp() {
    }

    void TearDown() {
        systems.clear_entries();
        memories.clear_entries();
        processors.clear_entries();
    }
protected:
    template <typename T>
    T addResource(const std::string& parent_uuid, const std::string& uuid, OptionalField<enums::Health> health) {
        auto r = T(parent_uuid);
        r.set_uuid(uuid);
        attribute::Status status;
        status.set_health(health);
        r.set_status(status);

        auto& manager = module::get_manager<T>();
        manager.add_entry(r);
        return manager.get_entry(uuid);
    }

    SystemManager& systems;
    MemoryManager& memories;
    ProcessorManager& processors;
};

RollupTest::~RollupTest() {}

using namespace testing;

TEST_F(RollupTest, SingleResourceNoChild) {
    auto system1 = addResource<System>("", "system1", Health::OK);
    auto system2 = addResource<System>("", "system2", Health::Critical);
    auto system3 = addResource<System>("", "system3", {});
    EXPECT_EQ(Health::OK, HealthRollup<System>().get(system1));
    EXPECT_EQ(Health::Critical, HealthRollup<System>().get(system2));
    EXPECT_FALSE(HealthRollup<System>().get(system3).has_value());
}

TEST_F(RollupTest, HealthyParentWithHealthyChild) {
    auto system1 = addResource<System>("", "system1", Health::OK);
    auto memory1 = addResource<Memory>("system1", "memory1", Health::OK);
    EXPECT_EQ(Health::OK, HealthRollup<Memory>().get(memory1));
    EXPECT_EQ(Health::OK, HealthRollup<System>().get(system1));
}

TEST_F(RollupTest, HealthyParentWithUnHealthyChild) {
    auto system1 = addResource<System>("", "system1", Health::OK);
    auto memory1 = addResource<Memory>("system1", "memory1", Health::Critical);
    EXPECT_EQ(Health::Critical, HealthRollup<Memory>().get(memory1));
    EXPECT_EQ(Health::Critical, HealthRollup<System>().get(system1));
}

TEST_F(RollupTest, UnhealthyParentWithHealthyChild) {
    auto system1 = addResource<System>("", "system1", Health::Warning);
    auto memory1 = addResource<Memory>("system1", "memory1", Health::OK);
    EXPECT_EQ(Health::OK, HealthRollup<Memory>().get(memory1));
    EXPECT_EQ(Health::Warning, HealthRollup<System>().get(system1));
}

TEST_F(RollupTest, HealthyParentWithUnknownChildHealth) {
    auto system1 = addResource<System>("", "system1", Health::OK);
    auto memory1 = addResource<Memory>("system1", "memory1", {});
    EXPECT_FALSE(HealthRollup<Memory>().get(memory1).has_value());
    EXPECT_EQ(Health::OK, HealthRollup<System>().get(system1));
}

TEST_F(RollupTest, UnhealthyParentWithUnknownChildHealth) {
    auto system1 = addResource<System>("", "system1", Health::Warning);
    auto memory1 = addResource<Memory>("system1", "memory1", {});
    EXPECT_FALSE(HealthRollup<Memory>().get(memory1).has_value());
    EXPECT_EQ(Health::Warning, HealthRollup<System>().get(system1));
}

TEST_F(RollupTest, ParentWithUnknownHealthWithUnknownChildHealth) {
    auto system1 = addResource<System>("", "system1", {});
    auto memory1 = addResource<Memory>("system1", "memory1", {});
    EXPECT_FALSE(HealthRollup<Memory>().get(memory1).has_value());
    EXPECT_FALSE(HealthRollup<System>().get(system1).has_value());
}

TEST_F(RollupTest, IgnoreOtherComponentsWhenFilterSpecified) {
    auto system1 = addResource<System>("", "system1", Health::Critical);
    addResource<Memory>("system1", "memory1", Health::Warning);
    addResource<Memory>("system1", "memory2", Health::OK);
    addResource<Memory>("system1", "memory3", {});
    addResource<Processor>("system1", "processor1", Health::Critical);

    // only rolled up health of dimm. ignore everything else (also System itself)
    EXPECT_EQ(Health::Warning, HealthRollup<System>().get(system1, agent_framework::model::enums::Component::Memory));
}

} // namespace endpoint
} // namespace rest
} // namespace psme
