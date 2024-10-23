/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2015-2019 Intel Corporation
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
 * @section Task timestamp tests
 * */

#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/model/task.hpp"

#include "gtest/gtest.h"

using namespace agent_framework::model;

class TaskTest : public ::testing::Test {
public:
    virtual ~TaskTest();

    static void SetUpTestSuite();

    static Task default_task;
};

TaskTest::~TaskTest() {}
Task TaskTest::default_task{};

void TaskTest::SetUpTestSuite() {
    default_task.set_name("repaint the submarine");
    default_task.set_state(enums::TaskState::New);
    default_task.set_start_time("2016-07-15T07:01+01:00");
    default_task.set_end_time("2016-07-07T14:45+02:00");
    attribute::Status status{};
    status.set_health(enums::Health::OK);
    default_task.set_status(status);

    attribute::Message message{};
    message.set_message_id("Base.1.0.BadColor");
    message.set_content("Cannot repaint the submarine");
    message.set_related_properties({"Color"});
    message.set_message_args({"ColorValue"});
    message.set_severity(enums::Health::Critical);
    message.set_resolution("The submarine must be yellow!");

    default_task.add_message(message);
}

TEST_F(TaskTest, ConversionFromJsonValuesExist) {
    const Task task = default_task;

    ASSERT_TRUE(task.get_start_time().has_value());
    ASSERT_TRUE(task.get_end_time().has_value());
    ASSERT_TRUE(task.get_name().has_value());
    ASSERT_TRUE(task.get_status().get_health().has_value());
    ASSERT_TRUE(task.get_state().has_value());
    ASSERT_FALSE(task.get_messages().empty());

    attribute::Message message = task.get_messages()[0];
    ASSERT_TRUE(message.get_message_id().has_value());
    ASSERT_TRUE(message.get_content().has_value());
    ASSERT_TRUE(message.get_severity().has_value());
    ASSERT_TRUE(message.get_resolution().has_value());
    ASSERT_FALSE(message.get_related_properties().empty());
    ASSERT_FALSE(message.get_message_args().empty());
}

TEST_F(TaskTest, ConversionFromJsonValuesCorrect) {
    const Task task = default_task;

    ASSERT_EQ(task.get_start_time(), "2016-07-15T07:01+01:00");
    ASSERT_EQ(task.get_end_time(), "2016-07-07T14:45+02:00");
    ASSERT_EQ(task.get_name(), "repaint the submarine");
    attribute::Status expected_status{};
    expected_status.set_health(enums::Health::OK);
    ASSERT_EQ(task.get_status().get_health(), expected_status.get_health());
    ASSERT_EQ(task.get_state(), enums::TaskState::New);

    const attribute::Message message = task.get_messages()[0];
    ASSERT_EQ(message.get_message_id(), "Base.1.0.BadColor");
    ASSERT_EQ(message.get_content(), "Cannot repaint the submarine");
    ASSERT_EQ(message.get_severity(), enums::Health::Critical);
    ASSERT_EQ(message.get_resolution(), "The submarine must be yellow!");
}

TEST_F(TaskTest, StartSetsStartTime) {
    Task task{};
    ASSERT_FALSE(task.get_start_time().has_value());
    ASSERT_FALSE(task.get_end_time().has_value());

    ASSERT_THROW(task.stop(), std::logic_error);

    task.start();
    ASSERT_TRUE(task.get_start_time().has_value());
    ASSERT_FALSE(task.get_end_time().has_value());

    ASSERT_THROW(task.start(), std::logic_error);
}

TEST_F(TaskTest, StopSetsStopTime) {
    Task task{};
    ASSERT_FALSE(task.get_start_time().has_value());
    ASSERT_FALSE(task.get_end_time().has_value());
    ASSERT_THROW(task.stop(), std::logic_error);

    task.start();
    task.stop();

    ASSERT_TRUE(task.get_start_time().has_value());
    ASSERT_TRUE(task.get_end_time().has_value());
    ASSERT_THROW(task.stop(), std::logic_error);
}
