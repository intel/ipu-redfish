/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief find function tests
 *
 * @copyright Copyright (c) 2018-2019 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file find_test.cpp
 */

#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/constants/routes.hpp"
#include "psme/rest/model/find.hpp"
#include "psme/rest/model/try_find.hpp"
#include "psme/rest/server/multiplexer.hpp"

#include "gtest/gtest.h"

using namespace testing;
using namespace psme::rest::constants;

namespace psme {
namespace rest {
namespace server {

using SystemManager = agent_framework::module::GenericManager<agent_framework::model::System>;
using ProcessorManager = agent_framework::module::GenericManager<agent_framework::model::Processor>;

class TestEndpointForFindApi : public MethodsHandler {
public:
    explicit TestEndpointForFindApi(const std::string& path) : MethodsHandler(path) {}
    ~TestEndpointForFindApi();
    virtual void get(const Request&, Response&) override {}
    virtual void patch(const Request&, Response&) override {}
    virtual void post(const Request&, Response&) override {}
    virtual void put(const Request&, Response&) override {}
    virtual void del(const Request&, Response&) override {}
};

TestEndpointForFindApi::~TestEndpointForFindApi() {}

class FindTest : public Test {
public:
    FindTest() : m_systems(agent_framework::module::get_manager<agent_framework::model::System>()),
                 m_processors(agent_framework::module::get_manager<agent_framework::model::Processor>()) {

        m_multiplexer.register_handler(TestEndpointForFindApi::UPtr(new TestEndpointForFindApi(Routes::REDFISH_PATH)));
        m_multiplexer.register_handler(TestEndpointForFindApi::UPtr(new TestEndpointForFindApi(Routes::ROOT_PATH)));
        m_multiplexer.register_handler(TestEndpointForFindApi::UPtr(new TestEndpointForFindApi(Routes::SYSTEMS_COLLECTION_PATH)));
        m_multiplexer.register_handler(TestEndpointForFindApi::UPtr(new TestEndpointForFindApi(Routes::SYSTEM_PATH)));
        m_multiplexer.register_handler(TestEndpointForFindApi::UPtr(new TestEndpointForFindApi(Routes::PROCESSORS_COLLECTION_PATH)));
        m_multiplexer.register_handler(TestEndpointForFindApi::UPtr(new TestEndpointForFindApi(Routes::PROCESSOR_PATH)));
    }
    ~FindTest();

    void SetUp() override {
        auto S1 = agent_framework::model::System("");
        S1.set_uuid("system_S1");
        S1.set_id(1);
        m_systems.add_entry(S1);

        auto S1Proc1 = agent_framework::model::Processor("system_S1");
        S1Proc1.set_uuid("S1_processor_P1");
        S1Proc1.set_id(1);
        m_processors.add_entry(S1Proc1);

        auto S1Proc2 = agent_framework::model::Processor("system_S1");
        S1Proc2.set_uuid("S1_processor_P2");
        S1Proc2.set_id(2);
        m_processors.add_entry(S1Proc2);

        auto S2 = agent_framework::model::System("");
        S2.set_uuid("system_S2");
        S2.set_id(2);
        m_systems.add_entry(S2);

        auto S2Proc1 = agent_framework::model::Processor("system_S2");
        S2Proc1.set_uuid("S2_processor_P1");
        S2Proc1.set_id(1);
        m_processors.add_entry(S2Proc1);

        auto S2Proc2 = agent_framework::model::Processor("system_S2");
        S2Proc2.set_uuid("S2_processor_P2");
        S2Proc2.set_id(2);
        m_processors.add_entry(S2Proc2);
    }

    void TearDown() override {
        m_systems.clear_entries();
        m_processors.clear_entries();
    }

    Multiplexer m_multiplexer{};
    SystemManager& m_systems;
    ProcessorManager& m_processors;
};

FindTest::~FindTest() {}

TEST_F(FindTest, TestFindDirectly_ThrowAfterNotFound) {
    const auto path = "/redfish/v1/Systems/2/Processors/1";
    const auto params = m_multiplexer.get_params(path, Routes::PROCESSOR_PATH);

    ASSERT_THROW(model::find<agent_framework::model::Manager>(params).get_uuid(), agent_framework::exceptions::NotFound);
}

TEST_F(FindTest, TestFindDirectly_NoThrowAfterFound) {
    const auto path = "/redfish/v1/Systems/2/Processors/1";
    const auto params = m_multiplexer.get_params(path, Routes::PROCESSOR_PATH);

    ASSERT_NO_THROW(model::find<agent_framework::model::Processor>(params).get_uuid());
    ASSERT_NO_THROW(
        (model::find<agent_framework::model::System, agent_framework::model::Processor>(params).get_uuid()));
}

TEST_F(FindTest, TestFindIndirectly) {
    const auto path = "/redfish/v1/Systems/2/Processors/2";
    const auto params = m_multiplexer.get_params(path, Routes::PROCESSOR_PATH);

    ASSERT_EQ("system_S2", model::find<agent_framework::model::System>(params).get_uuid());
    ASSERT_EQ("S2_processor_P2",
              (model::find<agent_framework::model::System, agent_framework::model::Processor>(params).get_uuid()));
}

TEST_F(FindTest, TestFindNoexceptDirectly_NoThrowAfterNotFound) {
    const auto path = "/redfish/v1/Systems/2/Processors/1";
    const auto params = m_multiplexer.get_params(path, Routes::PROCESSOR_PATH);

    ASSERT_NO_THROW(model::try_find<agent_framework::model::Manager>(params).get_uuid());
}

TEST_F(FindTest, TestFindNoexceptDirectly_NoThrowAfterFound) {
    const auto path = "/redfish/v1/Systems/2/Processors/1";
    const auto params = m_multiplexer.get_params(path, Routes::PROCESSOR_PATH);

    ASSERT_NO_THROW(model::try_find<agent_framework::model::Processor>(params).get_uuid());
    ASSERT_NO_THROW(
        (model::try_find<agent_framework::model::System, agent_framework::model::Processor>(params).get_uuid()));
}

TEST_F(FindTest, TestFindNoexceptIndirectly) {
    const auto path = "/redfish/v1/Systems/2/Processors/2";
    const auto params = m_multiplexer.get_params(path, Routes::PROCESSOR_PATH);

    auto opt_uuid = model::try_find<agent_framework::model::System>(params).get_uuid();
    ASSERT_EQ("system_S2", opt_uuid.value());
    opt_uuid =
        model::try_find<agent_framework::model::System, agent_framework::model::Processor>(params).get_uuid();
    ASSERT_EQ("S2_processor_P2", opt_uuid.value());
}

TEST_F(FindTest, TestFindNoexceptToBool) {
    const auto true_path = "/redfish/v1/Systems/2/Processors/2";
    const auto false_path = "/redfish/v1/Systems/2/Processors/3";
    const auto true_params = m_multiplexer.get_params(true_path, Routes::PROCESSOR_PATH);
    const auto false_params = m_multiplexer.get_params(false_path, Routes::PROCESSOR_PATH);

    ASSERT_EQ(true, (model::try_find<agent_framework::model::System, agent_framework::model::Processor>(true_params)));
    ASSERT_EQ(false, (model::try_find<agent_framework::model::System, agent_framework::model::Processor>(false_params)));
}

} // namespace server
} // namespace rest
} // namespace psme
