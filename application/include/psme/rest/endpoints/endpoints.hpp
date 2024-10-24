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
 * */

#pragma once

#include "event_service.hpp"
#include "event_subscription.hpp"
#include "event_subscriptions_collection.hpp"
#include "message_registry.hpp"
#include "message_registry_file.hpp"
#include "message_registry_file_collection.hpp"
#include "metadata.hpp"
#include "metadata_root.hpp"
#include "odata_service_document.hpp"
#include "psme/rest/endpoints/manager/log_entry.hpp"
#include "psme/rest/endpoints/manager/log_entry_collection.hpp"
#include "psme/rest/endpoints/manager/log_service.hpp"
#include "psme/rest/endpoints/manager/log_services_collection.hpp"
#include "psme/rest/endpoints/manager/manager.hpp"
#include "psme/rest/endpoints/manager/manager_collection.hpp"
#include "psme/rest/endpoints/manager/manager_network_interface.hpp"
#include "psme/rest/endpoints/manager/manager_network_interface_collection.hpp"
#include "psme/rest/endpoints/manager/manager_reset.hpp"
#include "psme/rest/endpoints/manager/network_protocol.hpp"
#include "psme/rest/endpoints/system/virtual_media.hpp"
#include "psme/rest/endpoints/system/virtual_media_collection.hpp"
#include "psme/rest/endpoints/system/virtual_media_eject.hpp"
#include "psme/rest/endpoints/system/virtual_media_insert.hpp"
#include "psme/rest/endpoints/task_service/monitor.hpp"
#include "psme/rest/endpoints/task_service/task.hpp"
#include "psme/rest/endpoints/task_service/task_collection.hpp"
#include "psme/rest/endpoints/task_service/task_service.hpp"
#include "redfish.hpp"
#include "root.hpp"
#include "session.hpp"
#include "session_collection.hpp"
#include "session_service.hpp"
#include "simple_update.hpp"
#include "simple_update_action_info.hpp"
#include "update_service.hpp"

#include "system/memory.hpp"
#include "system/memory_collection.hpp"
#include "system/memory_metrics.hpp"
#include "system/network_device_function.hpp"
#include "system/network_device_functions_collection.hpp"
#include "system/network_interface.hpp"
#include "system/network_interfaces_collection.hpp"
#include "system/processor.hpp"
#include "system/processor_metrics.hpp"
#include "system/processors_collection.hpp"
#include "system/storage_subsystem.hpp"
#include "system/storage_subsystems_collection.hpp"
#include "system/system.hpp"
#include "system/system_network_interface.hpp"
#include "system/system_network_interface_collection.hpp"
#include "system/system_reset.hpp"
#include "system/systems_collection.hpp"

#include "chassis/chassis.hpp"
#include "chassis/chassis_collection.hpp"
#include "chassis/drive.hpp"
#include "chassis/drives_collection.hpp"
#include "chassis/network_adapter.hpp"
#include "chassis/network_adapter_collection.hpp"
#include "chassis/pcie_device.hpp"
#include "chassis/pcie_devices_collection.hpp"
#include "chassis/pcie_function.hpp"
#include "chassis/pcie_functions_collection.hpp"

#include "telemetry/metric_definition.hpp"
#include "telemetry/metric_definitions_collection.hpp"
#include "telemetry/metric_report_definition.hpp"
#include "telemetry/metric_report_definitions_collection.hpp"
#include "telemetry/telemetry_service.hpp"
#include "telemetry/trigger.hpp"
#include "telemetry/triggers_collection.hpp"

#include "account_service/account.hpp"
#include "account_service/account_collection.hpp"
#include "account_service/account_service.hpp"
#include "account_service/role.hpp"
#include "account_service/role_collection.hpp"
