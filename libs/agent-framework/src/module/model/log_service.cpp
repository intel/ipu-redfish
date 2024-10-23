/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief Log service model
 *
 * @copyright Copyright (c) 2019 Intel Corporation.
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
 * @file log_service.cpp
 */

#include "agent-framework/module/model/log_service.hpp"

using namespace agent_framework::model;

const enums::Component LogService::component = enums::Component::LogService;

LogService::LogService(const std::string& parent_uuid, enums::Component parent_type) : Resource{parent_uuid, parent_type} {
    set_status(attribute::Status(enums::State::Enabled, enums::Health::OK));
}

LogService::~LogService() {}
