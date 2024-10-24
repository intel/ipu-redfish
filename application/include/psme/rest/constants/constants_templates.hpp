/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2018-2019 Intel Corporation
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
 * @file constants_templates.hpp
 *
 * @brief Definition of resource's IDs as templates of the resource type.
 * */

#pragma once

#include "agent-framework/module/model/model_chassis.hpp"
#include "agent-framework/module/model/model_common.hpp"
#include "agent-framework/module/model/model_compute.hpp"
#include "agent-framework/module/model/model_pnc.hpp"
#include "agent-framework/module/model/model_storage.hpp"

namespace psme {
namespace rest {
namespace constants {

template <typename T>
const char* get_resource_id() {
    return T::need_to_implement_for_T;
}

template <>
const char* get_resource_id<agent_framework::model::Memory>();

template <>
const char* get_resource_id<agent_framework::model::System>();

template <>
const char* get_resource_id<agent_framework::model::Chassis>();

template <>
const char* get_resource_id<agent_framework::model::Drive>();

template <>
const char* get_resource_id<agent_framework::model::StorageSubsystem>();

template <>
const char* get_resource_id<agent_framework::model::Manager>();

template <>
const char* get_resource_id<agent_framework::model::NetworkInterface>();

template <>
const char* get_resource_id<agent_framework::model::NetworkDevice>();

template <>
const char* get_resource_id<agent_framework::model::Task>();

template <>
const char* get_resource_id<agent_framework::model::NetworkDeviceFunction>();

template <>
const char* get_resource_id<agent_framework::model::MetricDefinition>();

template <>
const char* get_resource_id<agent_framework::model::Processor>();

template <>
const char* get_resource_id<agent_framework::model::PcieDevice>();

template <>
const char* get_resource_id<agent_framework::model::PcieFunction>();

template <>
const char* get_resource_id<agent_framework::model::LogService>();

template <>
const char* get_resource_id<agent_framework::model::LogEntry>();

template <>
const char* get_resource_id<agent_framework::model::VirtualMedia>();

} // namespace constants
} // namespace rest
} // namespace psme
