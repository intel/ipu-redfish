/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief Definition of NetworkDeviceFunction model class
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
 * @file network_device_function.cpp
 */

#include "agent-framework/module/model/network_device_function.hpp"

using namespace agent_framework::model;

const enums::Component NetworkDeviceFunction::component = enums::Component::NetworkDeviceFunction;

NetworkDeviceFunction::NetworkDeviceFunction(const std::string& parent_uuid, enums::Component parent_type) : Resource{parent_uuid, parent_type} {}

NetworkDeviceFunction::~NetworkDeviceFunction() {}