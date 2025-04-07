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
 * @file array.cpp
 * @brief Arrays specific data
 * */

#include "agent-framework/module/model/attributes/model_attributes.hpp"
#include "agent-framework/module/model/log_entry.hpp"
#include "agent-framework/module/model/metric.hpp"
#include "agent-framework/module/utils/is_framework_enum.hpp"
#include "agent-framework/module/utils/is_framework_object.hpp"

using namespace agent_framework::model;
using namespace agent_framework::model::attribute;

template <typename T>
Array<T>::Array() {}

template <typename T>
Array<T>::~Array() {}

namespace agent_framework {
namespace model {
namespace attribute {

/* Primitives */
template class Array<std::string>;
template class Array<std::uint32_t>;
template class Array<double>;

/* Model attributes */
template class Array<NetworkService>;
template class Array<Ipv4Address>;
template class Array<Ipv6Address>;
template class Array<PciDevice>;
template class Array<Message>;
template class Array<Identifier>;
template class Array<Location>;
template class Array<Region>;
template class Array<Wildcard>;
template class Array<CalculationParameters>;
template class Array<CapacitySource>;
template class Array<ReplicaInfo>;

/* Model classes */
template class Array<Metric>;
template class Array<LogEntry>;

/* Enums */
template class Array<enums::GraphicalConsoleSupportedType>;
template class Array<enums::SerialConsoleSupportedType>;
template class Array<enums::CommandShellSupportedType>;
template class Array<enums::BootOverride>;
template class Array<enums::BootOverrideTarget>;
template class Array<enums::MemoryType>;
template class Array<enums::Media>;
template class Array<enums::MemoryModuleType>;
template class Array<enums::DeviceType>;
template class Array<enums::MemoryMode>;
template class Array<enums::TransportProtocol>;
template class Array<enums::ResetType>;
template class Array<enums::VolumeType>;
template class Array<enums::EncryptionType>;
template class Array<enums::AccessCapability>;

} // namespace attribute
} // namespace model
} // namespace agent_framework
