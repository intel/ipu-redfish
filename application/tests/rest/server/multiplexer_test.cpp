/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief Path multiplexer tests
 *
 * @copyright Copyright (c) 2017-2019 Intel Corporation.
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
 * @file multiplexer_test.cpp
 */

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/constants/routes.hpp"
#include "psme/rest/server/multiplexer.hpp"

#include "gtest/gtest.h"

using namespace testing;
using namespace psme::rest::constants;

namespace psme {
namespace rest {
namespace server {

class TestEndpoint : public MethodsHandler {
public:
    explicit TestEndpoint(const std::string& path) : MethodsHandler(path) {}

    ~TestEndpoint();

    virtual void get(const Request& /* request */, Response& /*  response */) override {}

    virtual void patch(const Request& /* request */, Response& /*  response */) override {}

    virtual void post(const Request& /* request */, Response& /*  response */) override {}

    virtual void put(const Request& /* request */, Response& /*  response */) override {}

    virtual void del(const Request& /* request */, Response& /*  response */) override {}
};

TestEndpoint::~TestEndpoint() {}

class MultiplexerTest : public Test {
public:
    MultiplexerTest() {
        m_multiplexer.register_handler(TestEndpoint::UPtr(new TestEndpoint(Routes::REDFISH_PATH)));
        m_multiplexer.register_handler(TestEndpoint::UPtr(new TestEndpoint(Routes::ROOT_PATH)));
        m_multiplexer.register_handler(TestEndpoint::UPtr(new TestEndpoint(Routes::SYSTEMS_COLLECTION_PATH)));
        m_multiplexer.register_handler(TestEndpoint::UPtr(new TestEndpoint(Routes::SYSTEM_PATH)));
        m_multiplexer.register_handler(TestEndpoint::UPtr(new TestEndpoint(Routes::SYSTEM_ETHERNET_INTERFACES_COLLECTION_PATH)));
        m_multiplexer.register_handler(TestEndpoint::UPtr(new TestEndpoint(Routes::SYSTEM_ETHERNET_INTERFACE_PATH)));
    }

    ~MultiplexerTest();

    Multiplexer m_multiplexer{};
};

MultiplexerTest::~MultiplexerTest() {}

TEST_F(MultiplexerTest, AllowableURL) {
    ASSERT_TRUE(m_multiplexer.is_correct_endpoint_url("/redfish"));
    ASSERT_TRUE(m_multiplexer.is_correct_endpoint_url("/redfish/v1"));
    ASSERT_TRUE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/Systems"));
    ASSERT_TRUE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/Systems/100"));
    ASSERT_TRUE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/Systems/100/EthernetInterfaces"));
    ASSERT_TRUE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/Systems/100/EthernetInterfaces/500"));
}

TEST_F(MultiplexerTest, NotAllowableURL) {
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/deadfish/v1"));
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/redfish/v2"));
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/System"));
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/Systemss"));
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/Systems/*"));
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/Systems/-12"));
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/Systems/10^10"));
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/......"));
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/żółć"));
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/Systems/1/2"));
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/Systems/EthernetInterfaces/100"));
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/EthernetInterfaces/500/Systems/100"));
    ASSERT_FALSE(m_multiplexer.is_correct_endpoint_url("/redfish/v1/Systems/100/EthernetInterfaces/500/EthernetInterfaces/500"));
}

TEST_F(MultiplexerTest, TestGetParams) {
    const auto path1 = "/redfish/v1/Systems/3";
    const auto output1 = m_multiplexer.get_params(path1, constants::Routes::SYSTEM_PATH);

    ASSERT_EQ(output1[PathParam::SYSTEM_ID], "3");

    const auto path2 = "/redfish/v1/Systems/2/EthernetInterfaces/4";
    const auto output2 = m_multiplexer.get_params(path2, constants::Routes::SYSTEM_ETHERNET_INTERFACE_PATH);

    ASSERT_EQ(output2[PathParam::NIC_ID], "4");
    ASSERT_EQ(output2[PathParam::SYSTEM_ID], "2");

    std::vector<std::pair<std::string, std::string>> wrong_path_cases = {
        std::make_pair("/redfish/v1/EthernetInterfaces/4/Systems/2", constants::Routes::SYSTEM_ETHERNET_INTERFACE_PATH),
        std::make_pair("/redfish/v1/Systems/2/EthernetInterfaces/4", constants::Routes::SYSTEM_PATH),
        std::make_pair("/redfish/v1/Systems/2", constants::Routes::SYSTEM_ETHERNET_INTERFACE_PATH),
        std::make_pair("/redfish/v1/Systems/abc", constants::Routes::SYSTEM_PATH),
        std::make_pair("/redfish/v1/Systemss/2", constants::Routes::SYSTEM_PATH),
    };
    for (const auto& wrong_path : wrong_path_cases) {
        ASSERT_THROW(m_multiplexer.get_params(wrong_path.first, wrong_path.second),
                     agent_framework::exceptions::InvalidValue);
    }
}

TEST_F(MultiplexerTest, TestGetParamsGamiNothrow) {
    const auto path1 = "/redfish/v1/Systems/3";
    const auto output1 = m_multiplexer.try_get_params(path1, constants::Routes::SYSTEM_PATH);

    ASSERT_EQ(output1[PathParam::SYSTEM_ID], "3");

    const auto path2 = "/redfish/v1/Systems/2/EthernetInterfaces/4";
    const auto output2 = m_multiplexer.try_get_params(path2, constants::Routes::SYSTEM_ETHERNET_INTERFACE_PATH);

    ASSERT_EQ(output2[PathParam::NIC_ID], "4");
    ASSERT_EQ(output2[PathParam::SYSTEM_ID], "2");

    std::vector<std::pair<std::string, std::string>> wrong_path_cases = {
        std::make_pair("/redfish/v1/EthernetInterfaces/4/Systems/2", constants::Routes::SYSTEM_ETHERNET_INTERFACE_PATH),
        std::make_pair("/redfish/v1/Systems/2/EthernetInterfaces/4", constants::Routes::SYSTEM_PATH),
        std::make_pair("/redfish/v1/Systems/2", constants::Routes::SYSTEM_ETHERNET_INTERFACE_PATH),
        std::make_pair("/redfish/v1/Systems/abc", constants::Routes::SYSTEM_PATH),
        std::make_pair("/redfish/v1/Systemss/2", constants::Routes::SYSTEM_PATH),
    };
    Parameters params;
    for (const auto& wrong_path : wrong_path_cases) {
        ASSERT_NO_THROW(params = m_multiplexer.try_get_params(wrong_path.first, wrong_path.second));
        ASSERT_TRUE(params.begin() == params.end());
    }
}

TEST_F(MultiplexerTest, TestBadUrlTemplate) {
    const auto path = "/redfish/v1/BadSystems/3";
    const auto path_template = "/redfish/v1/Systems/{storageServiceId:[0-9]+}";

    ASSERT_THROW(m_multiplexer.get_params(path, path_template), std::logic_error);
}

} // namespace server
} // namespace rest
} // namespace psme
