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

#include "agent-framework/module/model/task.hpp"
#include "psme/rest/server/response.hpp"

namespace psme {
namespace rest {
namespace endpoint {
namespace task_service_utils {

server::Response call_task_get(const Uuid& task_uuid);

agent_framework::model::Task::Messages build_created_message();

agent_framework::model::Task::Messages build_success_message();

} // namespace task_service_utils
} // namespace endpoint
} // namespace rest
} // namespace psme
