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

#include "psme/rest/endpoints/endpoint_builder.hpp"
#include "psme/rest/constants/routes.hpp"
#include "psme/rest/endpoints/endpoints.hpp"
#include "psme/rest/server/multiplexer.hpp"
#include "psme/rest/server/utils.hpp"

using namespace psme::rest;
using namespace psme::rest::endpoint;
using namespace psme::rest::server;

EndpointBuilder::~EndpointBuilder() {}

void EndpointBuilder::build_endpoints() {
    auto& mp = *(psme::rest::server::Multiplexer::get_instance());
    mp.use_before([this](const Request&, Response& res) {
        res.set_header(ContentType::CONTENT_TYPE, ContentType::JSON);
    });

    // "/redfish
    mp.register_handler(Redfish::UPtr(new Redfish(constants::Routes::REDFISH_PATH)));

    // "/redfish/v1"
    mp.register_handler(Root::UPtr(new Root(constants::Routes::ROOT_PATH)));

    // "/redfish/v1/odata"
    mp.register_handler(Root::UPtr(new OdataServiceDocument(constants::Routes::ODATA_SERVICE_DOCUMENT)));

    // "/redfish/v1/$metadata"
    mp.register_handler(MetadataRoot::UPtr(new MetadataRoot(constants::Routes::METADATA_ROOT_PATH)));

    // "/redfish/v1/metadata/{metadata_file:*}"
    mp.register_handler(Metadata::UPtr(new Metadata(constants::Routes::METADATA_PATH)));

    // "/redfish/v1/UpdateService"
    mp.register_handler(UpdateService::UPtr(new UpdateService(constants::Routes::UPDATE_SERVICE_PATH)));

    // "/redfish/v1/UpdateService/SimpleUpdateActionInfo"
    mp.register_handler(
        SimpleUpdateActionInfo::UPtr(new SimpleUpdateActionInfo(constants::Routes::SIMPLE_UPDATE_ACTION_INFO_PATH)));

    // "/redfish/v1/UpdateService/Actions/SimpleUpdate"
    mp.register_handler(SimpleUpdate::UPtr(new SimpleUpdate(constants::Routes::SIMPLE_UPDATE_PATH)));

    // "/redfish/v1/EventService"
    mp.register_handler(EventService::UPtr(new EventService(constants::Routes::EVENT_SERVICE_PATH)));

    // "/redfish/v1/EventService/Subscriptions"
    mp.register_handler(
        SubscriptionCollection::UPtr(new SubscriptionCollection(constants::Routes::SUBSCRIPTION_COLLECTION_PATH)));

    // "/redfish/v1/EventService/Subscriptions/{subscriptionId:[0-9]+}"
    mp.register_handler(Subscription::UPtr(new Subscription(constants::Routes::SUBSCRIPTION_PATH)));

    // "/redfish/v1/SessionService"
    mp.register_handler(SessionService::UPtr(new SessionService(constants::Routes::SESSION_SERVICE_PATH)));

    // "/redfish/v1/SessionService/Sessions"
    mp.register_handler(SessionCollection::UPtr(new SessionCollection(constants::Routes::SESSION_COLLECTION_PATH)));

    // "/redfish/v1/SessionService/Sessions/{sessionId:[0-9]+}"
    mp.register_handler(Session::UPtr(new Session(constants::Routes::SESSION_PATH)));

    // "/redfish/v1/Registries"
    mp.register_handler(MessageRegistryFileCollection::UPtr(
        new MessageRegistryFileCollection(constants::Routes::MESSAGE_REGISTRY_FILE_COLLECTION_PATH)));

    // "/redfish/v1/Registries/{MessageRegistryFileId: [0-9]+}"
    mp.register_handler(MessageRegistryFile::UPtr(
        new MessageRegistryFile(constants::Routes::MESSAGE_REGISTRY_FILE_PATH)));

    // "/redfish/v1/TaskService"
    mp.register_handler(TaskService::UPtr(new TaskService(constants::Routes::TASK_SERVICE_PATH)));

    // "/redfish/v1/TaskService/Tasks"
    mp.register_handler(TaskCollection::UPtr(new TaskCollection(constants::Routes::TASK_COLLECTION_PATH)));

    // "/redfish/v1/Task/Service/Tasks/{taskId:[0-9]+}"
    mp.register_handler(Task::UPtr(new Task(constants::Routes::TASK_PATH)));

    // "/redfish/v1/Task/Service/Tasks/{taskId:[0-9]+}/Monitor"
    mp.register_handler(Monitor::UPtr(new Monitor(constants::Routes::MONITOR_PATH)));

    // "/redfish/v1/Chassis"
    mp.register_handler(ChassisCollection::UPtr(new ChassisCollection(constants::Routes::CHASSIS_COLLECTION_PATH)));

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}"
    mp.register_handler(Chassis::UPtr(new Chassis(constants::Routes::CHASSIS_PATH)));

#ifndef NDEBUG
    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/Drives"
    mp.register_handler(Chassis::UPtr(new DrivesCollection(constants::Routes::DRIVES_COLLECTION_PATH)));
#endif

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/Drives/{driveId:[0-9]+}"
    mp.register_handler(Chassis::UPtr(new Drive(constants::Routes::DRIVE_PATH)));

#ifndef NDEBUG
    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices"
    mp.register_handler(PcieDevicesCollection::UPtr(
        new PcieDevicesCollection(constants::Routes::PCIE_DEVICES_COLLECTION_PATH)));
#endif

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices/{deviceId:[0-9]+}"
    mp.register_handler(PcieDevice::UPtr(new PcieDevice(constants::Routes::PCIE_DEVICE_PATH)));

#ifndef NDEBUG
    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices/{deviceId:[0-9]+}/Functions"
    mp.register_handler(PcieFunctionsCollection::UPtr(
        new PcieFunctionsCollection(constants::Routes::PCIE_FUNCTIONS_COLLECTION_PATH)));
#endif

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/PCIeDevices/{deviceId:[0-9]+}/Functions/{functionId:[0-9]+}"
    mp.register_handler(PcieFunction::UPtr(new PcieFunction(constants::Routes::PCIE_FUNCTION_PATH)));

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/NetworkAdapters"
    mp.register_handler(NetworkAdapterCollection::UPtr(new NetworkAdapterCollection(
        constants::Routes::NETWORK_ADAPTERS_COLLECTION_PATH)));

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/NetworkAdapters/{networkAdapterId:[0-9]+}"
    mp.register_handler(NetworkAdapter::UPtr(new NetworkAdapter(
        constants::Routes::NETWORK_ADAPTER_PATH)));

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/NetworkAdapters/{networkAdapterId:[0-9]+}/NetworkDeviceFunctions"
    mp.register_handler(NetworkDeviceFunctionsCollection::UPtr(new NetworkDeviceFunctionsCollection(
        constants::Routes::CHASSIS_NETWORK_DEVICE_FUNCTIONS_COLLECTION_PATH)));

    // "/redfish/v1/Chassis/{chassisId:[0-9]+}/NetworkAdapters/{networkAdapterId:[0-9]+}/NetworkDeviceFunctions/{networkDeviceFunctionId:[0-9]+}"
    mp.register_handler(NetworkDeviceFunction::UPtr(new NetworkDeviceFunction(
        constants::Routes::CHASSIS_NETWORK_DEVICE_FUNCTION_PATH)));

    // "/redfish/v1/Managers"
    mp.register_handler(ManagerCollection::UPtr(new ManagerCollection(constants::Routes::MANAGER_COLLECTION_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}"
    mp.register_handler(Manager::UPtr(new Manager(constants::Routes::MANAGER_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/Actions/Manager.Reset"
    mp.register_handler(ManagerReset::UPtr(new ManagerReset(constants::Routes::MANAGER_RESET_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/EthernetInterfaces"
    mp.register_handler(ManagerNetworkInterfaceCollection::UPtr(
        new ManagerNetworkInterfaceCollection(constants::Routes::MANAGER_NETWORK_INTERFACE_COLLECTION_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/EthernetInterfaces/{nicId:[0-9]+}"
    mp.register_handler(ManagerNetworkInterface::UPtr(
        new ManagerNetworkInterface(constants::Routes::MANAGER_NETWORK_INTERFACE_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/NetworkProtocol"
    mp.register_handler(NetworkProtocol::UPtr(new NetworkProtocol(constants::Routes::NETWORK_PROTOCOL_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices"
    mp.register_handler(LogServicesCollection::UPtr(
        new LogServicesCollection(constants::Routes::MANAGER_LOG_SERVICES_COLLECTION_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices/{logServiceId:[0-9]+}"
    mp.register_handler(LogService::UPtr(
        new LogService(constants::Routes::MANAGER_LOG_SERVICE_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices/{logServiceId:[0-9]+}/Entries"
    mp.register_handler(LogEntryCollection::UPtr(
        new LogEntryCollection(constants::Routes::MANAGER_LOG_ENTRY_COLLECTION_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/LogServices/{logServiceId:[0-9]+}/Entries/{entryId:[0-9]+}"
    mp.register_handler(LogEntry::UPtr(
        new LogEntry(constants::Routes::MANAGER_LOG_ENTRY_PATH)));

    // "/redfish/v1/Systems"
    mp.register_handler(SystemsCollection::UPtr(new SystemsCollection(constants::Routes::SYSTEMS_COLLECTION_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}"
    mp.register_handler(System::UPtr(new System(constants::Routes::SYSTEM_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/Actions/ComputerSystem.Reset"
    mp.register_handler(SystemReset::UPtr(new SystemReset(constants::Routes::SYSTEM_RESET_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/Storage"
    mp.register_handler(StorageSubsystemsCollection::UPtr(
        new StorageSubsystemsCollection(constants::Routes::STORAGE_SUBSYSTEMS_COLLECTION_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/Storage/{storageId:[0-9]+}"
    mp.register_handler(StorageSubsystem::UPtr(new StorageSubsystem(constants::Routes::STORAGE_SUBSYSTEM_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/EthernetInterfaces"
    mp.register_handler(SystemNetworkInterfaceCollection::UPtr(
        new SystemNetworkInterfaceCollection(constants::Routes::SYSTEM_ETHERNET_INTERFACES_COLLECTION_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/EthernetInterfaces/{nicId:[0-9]+}"
    mp.register_handler(
        SystemNetworkInterface::UPtr(new SystemNetworkInterface(constants::Routes::SYSTEM_ETHERNET_INTERFACE_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/Memory"
    mp.register_handler(MemoryCollection::UPtr(new MemoryCollection(constants::Routes::MEMORY_COLLECTION_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/Memory/{memoryId:[0-9]+}"
    mp.register_handler(Memory::UPtr(new Memory(constants::Routes::MEMORY_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/Memory/{memoryId:[0-9]+}/Metrics"
    mp.register_handler(MemoryMetrics::UPtr(new MemoryMetrics(constants::Routes::MEMORY_METRICS_PATH)));

    //  "/redfish/v1/Systems/{systemId:[0-9]+}/Processors"
    mp.register_handler(
        ProcessorsCollection::UPtr(new ProcessorsCollection(constants::Routes::PROCESSORS_COLLECTION_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/Processors/{processorId:[0-9]+}"
    mp.register_handler(Processor::UPtr(new Processor(constants::Routes::PROCESSOR_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/Processors/{processorId:[0-9]+}/Metrics"
    mp.register_handler(ProcessorMetrics::UPtr(new ProcessorMetrics(constants::Routes::PROCESSORS_METRICS_PATH)));

    // "redfish/v1/Systems/{systemId:[0-9]+}+/VirtualMedia"
    mp.register_handler(VirtualMediaCollection::UPtr(
        new VirtualMediaCollection(constants::Routes::VIRTUAL_MEDIA_COLLECTION_PATH)));

    // "redfish/v1/Systems/{systemId:[0-9]+}+/VirtualMedia/{virtualMediaId:[0-9]+}"
    mp.register_handler(VirtualMedia::UPtr(
        new VirtualMedia(constants::Routes::VIRTUAL_MEDIA_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/VirtualMedia/{virtualMediaId:[0-9]+}/Actions/VirtualMedia.InsertMedia"
    mp.register_handler(VirtualMediaInsert::UPtr(new VirtualMediaInsert(constants::Routes::VIRTUAL_MEDIA_INSERT_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/VirtualMedia/{virtualMediaId:[0-9]+}/Actions/VirtualMedia.EjectMedia"
    mp.register_handler(VirtualMediaInsert::UPtr(new VirtualMediaEject(constants::Routes::VIRTUAL_MEDIA_EJECT_PATH)));

    // "/redfish/v1/TelemetryService"
    mp.register_handler(TelemetryService::UPtr(new TelemetryService(constants::Routes::TELEMETRY_SERVICE_PATH)));

    // "/redfish/v1/TelemetryService/MetricDefinitions"
    mp.register_handler(MetricDefinitionsCollection::UPtr(
        new MetricDefinitionsCollection(constants::Routes::METRIC_DEFINITIONS_COLLECTION_PATH)));

    // "/redfish/v1/TelemetryService/MetricDefinitions/{metricDefinitionId}"
    mp.register_handler(MetricDefinition::UPtr(new MetricDefinition(constants::Routes::METRIC_DEFINITION_PATH)));

    // "/redfish/v1/AccountService"
    mp.register_handler(AccountService::UPtr(new AccountService(constants::Routes::ACCOUNT_SERVICE_PATH)));

    // "/redfish/v1/AccountService/Accounts"
    mp.register_handler(AccountCollection::UPtr(new AccountCollection(constants::Routes::ACCOUNTS_COLLECTION_PATH)));

    // "/redfish/v1/AccountService/Accounts/{accountId}"
    mp.register_handler(Account::UPtr(new Account(constants::Routes::ACCOUNT_PATH)));

    // "/redfish/v1/AccountService/Roles"
    mp.register_handler(RoleCollection::UPtr(new RoleCollection(constants::Routes::ROLES_COLLECTION_PATH)));

    // "/redfish/v1/AccountService/Roles/{rolesId}"
    mp.register_handler(Role::UPtr(new Role(constants::Routes::ROLE_PATH)));
}
