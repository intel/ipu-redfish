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
 * @file manager_utils.cpp
 * @brief Utilities used by managers.
 * */

#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "agent-framework/module/chassis_components.hpp"
#include "agent-framework/module/common_components.hpp"
#include "agent-framework/module/compute_components.hpp"
#include "agent-framework/module/storage_components.hpp"

namespace agent_framework {
namespace module {

// template specializations for common components

template <>
GenericManager<model::Manager>&
get_manager<model::Manager>() { return CommonComponents::get_instance()->get_module_manager(); }

template <>
GenericManager<model::Task>& get_manager<model::Task>() { return CommonComponents::get_instance()->get_task_manager(); }

template <>
GenericManager<model::Metric>&
get_manager<model::Metric>() { return CommonComponents::get_instance()->get_metric_manager(); }

template <>
GenericManager<model::MetricDefinition>&
get_manager<model::MetricDefinition>() { return CommonComponents::get_instance()->get_metric_definition_manager(); }

template <>
GenericManager<model::Chassis>&
get_manager<model::Chassis>() { return CommonComponents::get_instance()->get_chassis_manager(); }

template <>
GenericManager<model::Drive>&
get_manager<model::Drive>() { return CommonComponents::get_instance()->get_drive_manager(); }

template <>
GenericManager<model::System>&
get_manager<model::System>() { return CommonComponents::get_instance()->get_system_manager(); }

template <>
GenericManager<model::StorageSubsystem>&
get_manager<model::StorageSubsystem>() { return CommonComponents::get_instance()->get_storage_subsystem_manager(); }

template <>
GenericManager<model::NetworkInterface>&
get_manager<model::NetworkInterface>() { return CommonComponents::get_instance()->get_network_interface_manager(); }

template <>
GenericManager<model::PcieDevice>& get_manager<model::PcieDevice>() {
    return CommonComponents::get_instance()->get_pcie_device_manager();
}

template <>
GenericManager<model::PcieFunction>& get_manager<model::PcieFunction>() {
    return CommonComponents::get_instance()->get_pcie_function_manager();
}

template <>
GenericManager<model::VirtualMedia>& get_manager<model::VirtualMedia>() {
    return CommonComponents::get_instance()->get_virtual_media_manager();
}

// template specializations for compute components

template <>
GenericManager<model::Processor>&
get_manager<model::Processor>() { return CommonComponents::get_instance()->get_processor_manager(); }

template <>
GenericManager<model::Memory>&
get_manager<model::Memory>() { return ComputeComponents::get_instance()->get_memory_manager(); }

template <>
GenericManager<model::StorageController>&
get_manager<model::StorageController>() { return ComputeComponents::get_instance()->get_storage_controller_manager(); }

template <>
GenericManager<model::NetworkDevice>&
get_manager<model::NetworkDevice>() { return ComputeComponents::get_instance()->get_network_device_manager(); }

template <>
GenericManager<model::NetworkDeviceFunction>&
get_manager<model::NetworkDeviceFunction>() { return ComputeComponents::get_instance()->get_network_device_function_manager(); }

template <>
GenericManager<model::TrustedModule>&
get_manager<model::TrustedModule>() { return ComputeComponents::get_instance()->get_trusted_module_manager(); }

template <>
GenericManager<model::Volume>&
get_manager<model::Volume>() { return StorageComponents::get_instance()->get_volume_manager(); }

template <>
GenericManager<model::LogService>&
get_manager<model::LogService>() { return CommonComponents::get_instance()->get_log_service_manager(); }

template <>
GenericManager<model::LogEntry>&
get_manager<model::LogEntry>() { return CommonComponents::get_instance()->get_log_entry_manager(); }

} // namespace module
} // namespace agent_framework
