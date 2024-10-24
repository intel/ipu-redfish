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

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/path_builder.hpp"

using namespace psme::rest::constants;
using namespace psme::rest::endpoint;

// "/redfish
const std::string Routes::REDFISH_PATH =
    PathBuilder()
        .append(PathParam::REDFISH)
        .build();

// "/redfish/v1"
const std::string Routes::ROOT_PATH =
    PathBuilder(PathParam::BASE_URL)
        .build();

// "/redfish/v1/odata"
const std::string Routes::ODATA_SERVICE_DOCUMENT =
    PathBuilder(PathParam::BASE_URL)
        .append(PathParam::ODATA)
        .build();

// "/redfish/v1/$metadata"
const std::string Routes::METADATA_ROOT_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(PathParam::METADATA_ROOT)
        .build();

// "/redfish/v1/metadata/{metadata_file:*}"
const std::string Routes::METADATA_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(PathParam::METADATA)
        .append_regex(PathParam::METADATA_FILE, PathParam::XML_REGEX)
        .build();

// "/redfish/v1/UpdateService"
const std::string Routes::UPDATE_SERVICE_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::UPDATE_SERVICE)
        .build();

// "/redfish/v1/UpdateService/Actions/UpdateService.SimpleUpdate"
const std::string Routes::SIMPLE_UPDATE_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::UPDATE_SERVICE)
        .append(Common::ACTIONS)
        .append(UpdateService::UPDATE_SERVICE_SIMPLE_UPDATE)
        .build();

// "/redfish/v1/UpdateService/SimpleUpdateActionInfo"
const std::string Routes::SIMPLE_UPDATE_ACTION_INFO_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::UPDATE_SERVICE)
        .append(UpdateService::SIMPLE_UPDATE_ACTION_INFO)
        .build();

// "/redfish/v1/EventService"
const std::string Routes::EVENT_SERVICE_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::EVENT_SERVICE)
        .build();

// "/redfish/v1/EventService/Subscriptions"
const std::string Routes::SUBSCRIPTION_COLLECTION_PATH =
    PathBuilder(EVENT_SERVICE_PATH)
        .append(EventService::SUBSCRIPTIONS)
        .build();

// "/redfish/v1/EventService/Subscriptions/{subscriptionId:[0-9]+}"
const std::string Routes::SUBSCRIPTION_PATH =
    PathBuilder(SUBSCRIPTION_COLLECTION_PATH)
        .append_regex(PathParam::SUBSCRIPTION_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/SessionService"
const std::string Routes::SESSION_SERVICE_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::SESSION_SERVICE)
        .build();

// "/redfish/v1/SessionService/Sessions"
const std::string Routes::SESSION_COLLECTION_PATH =
    PathBuilder(SESSION_SERVICE_PATH)
        .append(SessionService::SESSIONS)
        .build();

// "/redfish/v1/SessionService/Sessions/{sessionId:[0-9]+}"
const std::string Routes::SESSION_PATH =
    PathBuilder(SESSION_COLLECTION_PATH)
        .append_regex(PathParam::SESSION_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/MessageRegistryFiles"
const std::string Routes::MESSAGE_REGISTRY_FILE_COLLECTION_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::REGISTRIES)
        .build();

// "/redfish/v1/Registries/{MessageRegistryFileId: [0-9]+}"
const std::string Routes::MESSAGE_REGISTRY_FILE_PATH =
    PathBuilder(MESSAGE_REGISTRY_FILE_COLLECTION_PATH)
        .append_regex(PathParam::MESSAGE_REGISTRY_FILE_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/TaskService"
const std::string Routes::TASK_SERVICE_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(TaskService::TASK_SERVICE)
        .build();

// "/redfish/v1/TaskService/Tasks"
const std::string Routes::TASK_COLLECTION_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(TaskService::TASK_SERVICE)
        .append(Root::TASKS)
        .build();

// "/redfish/v1/TaskService/Tasks/{TaskId:[0-9]+}"
const std::string Routes::TASK_PATH =
    PathBuilder(TASK_COLLECTION_PATH)
        .append_regex(PathParam::TASK_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/TaskService/TaskMonitors/{TaskId:[0-9]+}"
const std::string Routes::MONITOR_PATH =
    PathBuilder(TASK_SERVICE_PATH)
        .append(Monitor::TASK_MONITORS)
        .append_regex(PathParam::TASK_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Chassis"
const std::string Routes::CHASSIS_COLLECTION_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Common::CHASSIS)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}"
const std::string Routes::CHASSIS_PATH =
    PathBuilder(CHASSIS_COLLECTION_PATH)
        .append_regex(PathParam::CHASSIS_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Drives"
const std::string Routes::DRIVES_COLLECTION_PATH =
    PathBuilder(CHASSIS_PATH)
        .append(Chassis::DRIVES)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/Drives/{driveId:[0-9]+}"
const std::string Routes::DRIVE_PATH =
    PathBuilder(DRIVES_COLLECTION_PATH)
        .append_regex(PathParam::DRIVE_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices"
const std::string Routes::PCIE_DEVICES_COLLECTION_PATH =
    PathBuilder(CHASSIS_PATH)
        .append(Chassis::PCIE_DEVICES)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices/{deviceId:[0-9]+}"
const std::string Routes::PCIE_DEVICE_PATH =
    PathBuilder(PCIE_DEVICES_COLLECTION_PATH)
        .append_regex(PathParam::DEVICE_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+/PCIeDevices/{deviceId:[0-9]+/Functions"
const std::string Routes::PCIE_FUNCTIONS_COLLECTION_PATH =
    PathBuilder(PCIE_DEVICE_PATH)
        .append(PathParam::FUNCTIONS)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices/{deviceId:[0-9]+}/Functions/{functionId:[0-9]+}"
const std::string Routes::PCIE_FUNCTION_PATH =
    PathBuilder(PCIE_FUNCTIONS_COLLECTION_PATH)
        .append_regex(PathParam::FUNCTION_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/NetworkAdapters"
const std::string Routes::NETWORK_ADAPTERS_COLLECTION_PATH =
    PathBuilder(CHASSIS_PATH)
        .append(Chassis::NETWORK_ADAPTERS)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/NetworkAdapters/{networkAdapterId:[0-9]+}"
const std::string Routes::NETWORK_ADAPTER_PATH =
    PathBuilder(NETWORK_ADAPTERS_COLLECTION_PATH)
        .append_regex(PathParam::NETWORK_ADAPTER_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/NetworkAdapters/{networkAdapterId:[0-9]+}/NetworkDeviceFunctions"
const std::string Routes::CHASSIS_NETWORK_DEVICE_FUNCTIONS_COLLECTION_PATH =
    PathBuilder(NETWORK_ADAPTER_PATH)
        .append(NetworkInterface::NETWORK_DEVICE_FUNCTIONS)
        .build();

// "/redfish/v1/Chassis/{chassisId:[0-9]+}/NetworkAdapters/{networkAdapterId:[0-9]+}/NetworkDeviceFunctions/{networkDeviceFunctionId:[0-9]+}"
const std::string Routes::CHASSIS_NETWORK_DEVICE_FUNCTION_PATH =
    PathBuilder(CHASSIS_NETWORK_DEVICE_FUNCTIONS_COLLECTION_PATH)
        .append_regex(PathParam::NETWORK_DEVICE_FUNCTION_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Managers"
const std::string Routes::MANAGER_COLLECTION_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::MANAGERS)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}"
const std::string Routes::MANAGER_PATH =
    PathBuilder(MANAGER_COLLECTION_PATH)
        .append_regex(PathParam::MANAGER_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/Actions/Manager.Reset"
const std::string Routes::MANAGER_RESET_PATH =
    PathBuilder(MANAGER_PATH)
        .append(Common::ACTIONS)
        .append(Manager::MANAGER_RESET)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/EthernetInterfaces"
const std::string Routes::MANAGER_NETWORK_INTERFACE_COLLECTION_PATH =
    PathBuilder(MANAGER_PATH)
        .append(Manager::ETHERNET_INTERFACES)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/EthernetInterfaces/{nicId:[0-9]+}"
const std::string Routes::MANAGER_NETWORK_INTERFACE_PATH =
    PathBuilder(MANAGER_NETWORK_INTERFACE_COLLECTION_PATH)
        .append_regex(PathParam::NIC_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/NetworkProtocol"
const std::string Routes::NETWORK_PROTOCOL_PATH =
    PathBuilder(MANAGER_PATH)
        .append(Manager::NETWORK_PROTOCOL)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices"
const std::string Routes::MANAGER_LOG_SERVICES_COLLECTION_PATH =
    PathBuilder(MANAGER_PATH)
        .append(Common::LOG_SERVICES)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices/{logServiceId:[0-9]+}"
const std::string Routes::MANAGER_LOG_SERVICE_PATH =
    PathBuilder(MANAGER_LOG_SERVICES_COLLECTION_PATH)
        .append_regex(PathParam::LOG_SERVICE_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices/{logServiceId:[0-9]+}/Entries"
const std::string Routes::MANAGER_LOG_ENTRY_COLLECTION_PATH =
    PathBuilder(MANAGER_LOG_SERVICE_PATH)
        .append(LogService::ENTRIES)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices/{logServiceId:[0-9]+}/Entries/{entryId:[0-9]+}"
const std::string Routes::MANAGER_LOG_ENTRY_PATH =
    PathBuilder(MANAGER_LOG_ENTRY_COLLECTION_PATH)
        .append_regex(PathParam::LOG_ENTRY_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Systems"
const std::string Routes::SYSTEMS_COLLECTION_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::SYSTEMS)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}"
const std::string Routes::SYSTEM_PATH =
    PathBuilder(SYSTEMS_COLLECTION_PATH)
        .append_regex(PathParam::SYSTEM_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/Actions/ComputerSystem.Reset"
const std::string Routes::SYSTEM_RESET_PATH =
    PathBuilder(SYSTEM_PATH)
        .append(Common::ACTIONS)
        .append(System::COMPUTER_SYSTEM_RESET)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/Storage"
const std::string Routes::STORAGE_SUBSYSTEMS_COLLECTION_PATH =
    PathBuilder(SYSTEM_PATH)
        .append(System::STORAGE)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/Storage/{storageId:[0-9]+}"
const std::string Routes::STORAGE_SUBSYSTEM_PATH =
    PathBuilder(STORAGE_SUBSYSTEMS_COLLECTION_PATH)
        .append_regex(PathParam::STORAGE_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/EthernetInterfaces"
const std::string Routes::SYSTEM_ETHERNET_INTERFACES_COLLECTION_PATH =
    PathBuilder(SYSTEM_PATH)
        .append(System::ETHERNET_INTERFACES)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/EthernetInterfaces/{nicId:[0-9]+}"
const std::string Routes::SYSTEM_ETHERNET_INTERFACE_PATH =
    PathBuilder(SYSTEM_ETHERNET_INTERFACES_COLLECTION_PATH)
        .append_regex(PathParam::NIC_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/Memory"
const std::string Routes::MEMORY_COLLECTION_PATH =
    PathBuilder(SYSTEM_PATH)
        .append(System::MEMORY)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/Memory/{memoryId:[0-9]+}"
const std::string Routes::MEMORY_PATH =
    PathBuilder(MEMORY_COLLECTION_PATH)
        .append_regex(PathParam::MEMORY_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/Memory/{memoryId:[0-9]+}/Metrics"
const std::string Routes::MEMORY_METRICS_PATH =
    PathBuilder(MEMORY_PATH)
        .append(Common::METRICS)
        .build();

//  "/redfish/v1/Systems/{systemId:[0-9]+}/Processors"
const std::string Routes::PROCESSORS_COLLECTION_PATH =
    PathBuilder(SYSTEM_PATH)
        .append(System::PROCESSORS)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/Processors/{processorId:[0-9]+}"
const std::string Routes::PROCESSOR_PATH =
    PathBuilder(PROCESSORS_COLLECTION_PATH)
        .append_regex(PathParam::PROCESSOR_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/Processors/{processorId:[0-9]+}/Metrics"
const std::string Routes::PROCESSORS_METRICS_PATH =
    PathBuilder(PROCESSOR_PATH)
        .append(Common::METRICS)
        .build();

// "redfish/v1/Systems/{systemId:[0-9]+}/VirtualMedia"
const std::string Routes::VIRTUAL_MEDIA_COLLECTION_PATH =
    PathBuilder(SYSTEM_PATH)
        .append(Common::VIRTUAL_MEDIA)
        .build();

// "redfish/v1/Systems/{systemId:[0-9]+}/VirtualMedia/{virtualMediaId:[0-9]+}"
const std::string Routes::VIRTUAL_MEDIA_PATH =
    PathBuilder(VIRTUAL_MEDIA_COLLECTION_PATH)
        .append_regex(PathParam::VIRTUAL_MEDIA_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/VirtualMedia/{virtualMediaId:[0-9]+}/Actions/VirtualMedia.InsertMedia"
const std::string Routes::VIRTUAL_MEDIA_INSERT_PATH =
    PathBuilder(Routes::VIRTUAL_MEDIA_PATH)
        .append(Common::ACTIONS)
        .append(Common::VIRTUAL_MEDIA_INSERT)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/VirtualMedia/{virtualMediaId:[0-9]+}/Actions/VirtualMedia.EjectMedia"
const std::string Routes::VIRTUAL_MEDIA_EJECT_PATH =
    PathBuilder(Routes::VIRTUAL_MEDIA_PATH)
        .append(Common::ACTIONS)
        .append(Common::VIRTUAL_MEDIA_EJECT)
        .build();

// "/redfish/v1/TelemetryService"
const std::string Routes::TELEMETRY_SERVICE_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::TELEMETRY_SERVICE)
        .build();

// "/redfish/v1/TelemetryService/MetricDefinitions"
const std::string Routes::METRIC_DEFINITIONS_COLLECTION_PATH =
    PathBuilder(TELEMETRY_SERVICE_PATH)
        .append(TelemetryService::METRIC_DEFINITIONS)
        .build();

// "/redfish/v1/TelemetryService/MetricDefinitions/{metricDefinitionId}"
const std::string Routes::METRIC_DEFINITION_PATH =
    PathBuilder(METRIC_DEFINITIONS_COLLECTION_PATH)
        .append_regex(PathParam::METRIC_DEFINITION_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/TelemetryService/MetricReportDefinitions"
const std::string Routes::METRIC_REPORT_DEFINITIONS_COLLECTION_PATH =
    PathBuilder(TELEMETRY_SERVICE_PATH)
        .append(TelemetryService::METRIC_REPORT_DEFINITIONS)
        .build();

// "/redfish/v1/TelemetryService/MetricReportDefinitions/{metricReportDefinitionId}"
const std::string Routes::METRIC_REPORT_DEFINITION_PATH =
    PathBuilder(METRIC_REPORT_DEFINITIONS_COLLECTION_PATH)
        .append_regex(PathParam::METRIC_REPORT_DEFINITION_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/TelemetryService/Triggers"
const std::string Routes::TRIGGERS_COLLECTION_PATH =
    PathBuilder(TELEMETRY_SERVICE_PATH)
        .append(TelemetryService::TRIGGERS)
        .build();

// "/redfish/v1/TelemetryService/Triggers/{trigger}"
const std::string Routes::TRIGGER_PATH =
    PathBuilder(TRIGGERS_COLLECTION_PATH)
        .append_regex(PathParam::TRIGGER_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/AccountService"
const std::string Routes::ACCOUNT_SERVICE_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::ACCOUNT_SERVICE)
        .build();

// "/redfish/v1/AccountService/Accounts"
const std::string Routes::ACCOUNTS_COLLECTION_PATH =
    PathBuilder(ACCOUNT_SERVICE_PATH)
        .append(AccountService::ACCOUNTS)
        .build();

// "/redfish/v1/AccountService/Accounts/{accountId}
const std::string Routes::ACCOUNT_PATH =
    PathBuilder(ACCOUNTS_COLLECTION_PATH)
        .append_regex(PathParam::ACCOUNT_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/AccountService/Roles"
const std::string Routes::ROLES_COLLECTION_PATH =
    PathBuilder(ACCOUNT_SERVICE_PATH)
        .append(AccountService::ROLES)
        .build();

// "/redfish/v1/AccountService/Roles/{rolesId}"
const std::string Routes::ROLE_PATH =
    PathBuilder(ROLES_COLLECTION_PATH)
        .append_regex(PathParam::ROLE_ID, PathParam::STRING_ID_REGEX)
        .build();
