/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
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
 *
 * @brief Task model interface
 * @file task.hpp
 * */

#pragma once

#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/model/attributes/array.hpp"
#include "agent-framework/module/model/resource.hpp"

#include <ctime>
#include <functional>
#include <vector>

namespace agent_framework {
namespace model {

class Task : public Resource {
public:
    using Messages = attribute::Array<attribute::Message>;

    explicit Task(const std::string& parent_uuid = {}, enums::Component parent_type = enums::Component::None);

    ~Task();

    /* Enable copy and move semantics */
    Task(const Task&) = default;

    Task(Task&&) = default;

    Task& operator=(const Task&) = default;

    Task& operator=(Task&&) = default;

    friend auto operator<=>(const Task& lhs, const Task& rhs) = default;

    /*!
     * Get task component name enum
     *
     * @return Task component name enum
     * */
    static enums::Component get_component() {
        return Task::component;
    }

    /*!
     * Set task start time timestamp with current time value
     *
     * This function provides the only interface for setting the start time
     * timestamp. If the start or end time timestamps are already set, then
     * and std::logic_error is thrown.
     * */
    void start();

    /*!
     * Set task end time timestamp with current time value
     *
     * This function provides the only interface for setting the end time
     * timestamp. If the start time timestamp is not set or end time timestamp
     * is already set, and std::logic_error is thrown.
     * */
    void stop();

    /*!
     * Get start time timestamp
     *
     * @return Start time timestamp formatted according to redfish metadata
     * */
    const OptionalField<std::string>& get_start_time() const {
        return m_start_time;
    }

    /*!
     * Get end time timestamp
     *
     * @return End time timestamp formatted according to redfish metadata
     * */
    const OptionalField<std::string>& get_end_time() const {
        return m_end_time;
    }

    /*!
     * Get task state
     *
     * @return TaskState enum representing task state
     * */
    const OptionalField<enums::TaskState>& get_state() const {
        return m_state;
    }

    /*!
     * Set task state
     *
     * @param[in] state TaskState enum representing task state
     * */
    void set_state(const OptionalField<enums::TaskState>& state) {
        m_state = state;
    }

    /*!
     * Get messages associated with a task
     *
     * @return Array object containing the messages
     * */
    const Messages& get_messages() const {
        return m_messages;
    }

    /*!
     * Set messages associated with a task
     *
     * @param[in] messages Array object containing task messages
     * */
    void set_messages(const Messages& messages) {
        m_messages = messages;
    }

    /*!
     * Clear all messages.
     * */
    void clear_messages() {
        m_messages = Messages();
    }

    /*!
     * Add message to a task.
     *
     * @param[in] message Message to be added to the task object.
     * */
    void add_message(const attribute::Message& message) {
        m_messages.add_entry(message);
    }

    void set_start_time(const OptionalField<std::string>& start_time) {
        m_start_time = start_time;
    }

    void set_end_time(const OptionalField<std::string>& end_time) {
        m_end_time = end_time;
    }
private:
    OptionalField<std::string> m_start_time{};
    OptionalField<std::string> m_end_time{};
    OptionalField<enums::TaskState> m_state{enums::TaskState::New};
    Messages m_messages{};

    static const enums::Component component;
};

} // namespace model
} // namespace agent_framework
