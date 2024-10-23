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
 * @file common.hpp
 * @brief string enums for model classes, see enum_builder.hpp for more info
 * */

#pragma once

#include "enum_builder.hpp"
#include "processor_instruction_set.hpp"
#include "sensor_type.hpp"

namespace agent_framework {
namespace model {
namespace enums {

/*!
 * @brief ENUM ResetType as defined in metadata in Resource.ResetType
 *
 */
ENUM(ResetType, uint32_t, On, ForceOff, GracefulShutdown, GracefulRestart,
     ForceRestart, Nmi, ForceOn, PushPowerButton, PowerCycle, Suspend, Pause, Resume);

/*!
 * @brief ENUM DriveType for Drive class member
 */
ENUM(DriveType, uint32_t, HDD, SSD, SMR);

/*!
 * @brief ENUM IPv4AddressOrigin for attribute IPv4Address class member
 */
ENUM(Ipv4AddressOrigin, uint32_t, DHCP, Static);

/*!
 * @brief ENUM IPv6AddressOrigin for attribute IPv6Address class member
 */
ENUM(Ipv6AddressOrigin, uint32_t, DHCP, Static, SLAAC);

/*!
 * @brief ENUM IPv6AddressState for attribute IPv6Address class member
 */
ENUM(Ipv6AddressState, uint32_t, Preferred, Deprecated, Tentative, Failed);

/*!
 * @brief ENUM State for attribute class Status State member
 */
ENUM(State, uint32_t, Enabled, Disabled, StandbyOffline, StandbySpare, InTest, Starting, Absent, UnavailableOffline,
     Deferring, Quiesced, Updating);

/*!
 * @brief ENUM Health for attribute class Status Health member
 */
ENUM(Health, uint32_t, OK, Warning, Critical);

/*!
 * @brief ENUM ManagerInfoType describes type of the manager
 */
ENUM(ManagerInfoType, uint32_t,
     ManagementController, EnclosureManager, BMC, RackManager, AuxiliaryController);

/*!
 * @brief ENUM SerialConsoleSupportedType describes type of communication with
 * serial
 */
ENUM(SerialConsoleSupportedType, uint32_t,
     Telnet, IPMI, SSH);

/*!
 * @brief ENUM GraphicalConsoleSupportedType describes type of communication with
 * graphical console
 */
ENUM(GraphicalConsoleSupportedType, uint32_t,
     KVMIP, dummy);

/*!
 * @brief ENUM CommandShellSupportedType describes type of communication with
 * graphical console
 */
ENUM(CommandShellSupportedType, uint32_t,
     VirtualMedia, Telnet, SSH);

/*!
 * @brief ENUM Component describes type of the object
 */
ENUM(Component, uint32_t,
     None, AuthorizationCertificate, Chassis, Drive,
     Manager, Memory,
     NetworkInterface, NetworkDevice, Processor,
     NetworkDeviceFunction, StorageController, System,
     PcieDevice, PcieFunction,
     StorageSubsystem, Task, Metric, MetricDefinition,
     TrustedModule, Volume, LogService, LogEntry, BootOption,
     VirtualMedia);

/*!
 * @brief ENUM IdentifierType for Identifier attribute durableNameFormat field
 */
ENUM(IdentifierType, uint32_t, NAA, iQN, FC_WWN, UUID, EUI, NQN, NSID);

/*!
 * @brief ENUM IndicatorLed for Chassis and Drive and Computer System indicatorLed field
 */
ENUM(IndicatorLed, uint32_t, Unknown, Lit, Blinking, Off);

/*!
 * @brief ENUM StatusIndicator for Drive statusIndicator field
 */
ENUM(StatusIndicator, uint32_t, Ok, Fail, Rebuild, PredictiveFailureAnalysis,
     Hotspare, InACriticalArray, InAFailedArray);

/*!
 * @brief ENUM HotspareType for Drive hotSpare field
 */
ENUM(HotspareType, uint32_t, None, Global, Chassis, Dedicated);

/*!
 * @brief ENUM EncryptionAbility for Drive encryptionAbility field
 */
ENUM(EncryptionAbility, uint32_t, None, SelfEncryptingDrive, Other);

/*!
 * @brief ENUM EncryptionStatus for Drive encryptionStatus field
 */
ENUM(EncryptionStatus, uint32_t, Unencrypted, Unlocked, Locked, Foreign);

/*!
 * @brief ENUM TaskState for Task class
 * */
ENUM(TaskState, uint32_t, New, Starting, Running, Suspended, Interrupted, Pending,
     Stopping, Completed, Killed, Exception, Service);

/*!
 * @brief ENUM ManagerPowerState state enum for Manager class
 * */
ENUM(ManagerPowerState, uint32_t, On, Off);

/*!
 * @brief ENUM ProcessorType for Processor class member
 *
 */
ENUM(ProcessorType, uint32_t, CPU, GPU, FPGA, DSP, Accelerator, OEM);

/*!
 * @brief ENUM ProcessorModel for Processor class member
 *
 */
ENUM(ProcessorModel, uint32_t, E3, E5, E7, X3, X5, X7, I3, I5, I7, Silver, Gold, Platinum);

/*!
 * @brief ENUM ProcessorArchitecture for Processor class member
 */
ENUM(ProcessorArchitecture, uint32_t, x86, ARM, MIPS, OEM);

/*!
 * @brief ENUM TransferProtocol for Update service
 * */
ENUM(TransferProtocol, uint32_t, CIFS, FTP, SFTP, HTTP, HTTPS, NFS, SCP, TFTP, OEM);

/*!
 * @brief ENUM MetricDataType for Metric Definition class
 * */
ENUM(MetricDataType, uint32_t, Binary, Boolean, Byte, Date, DateTimeOffset, Duration,
     TimeOfDay, Decimal, Double, Single, Int16, Int32, Int64, String, SByte);

/*!
 * @brief ENUM MetricType for Metric Definition class
 * */
ENUM(MetricType, uint32_t, Counter, Gauge, Numeric, Discrete);

/*!
 * @brief ENUM MetricImplementation for Metric Definition class
 * */
ENUM(MetricImplementation, uint32_t, PhysicalSensor, Synthesized, Calculated, DigitalMeter);

/*!
 * @brief ENUM DiscreteMetricType for Metric Definition
 */
ENUM(DiscreteMetricType, uint32_t, Single, Multiple);

/*!
 * @brief ENUM MetricCalculable for Metric Definition class
 * */
ENUM(MetricCalculable, uint32_t, NonCalculable, Summable, NonSummable);

/*!
 * @brief ENUM MetricAlgorithm for Metric Definition class
 * */
ENUM(MetricAlgorithm, uint32_t, AverageOverInterval, MaximumDuringInterval, MinimumDuringInterval);

/*!
 * @brief ENUM Interface type for TrustedModule class
 */
ENUM(InterfaceType, uint32_t, TPM1_2, TPM2_0, TCM1_0);

/*!
 * @brief ENUM Volume Types
 */
ENUM(VolumeType, uint32_t,
     RawDevice, NonRedundant, Mirrored, StripedWithParity, SpannedMirrors, SpannedStripesWithParity);

/*!
 * @brief ENUM Encryption Types
 */
ENUM(EncryptionType, uint32_t, NativeDriveEncryption, ControllerAssisted, SoftwareAssisted);

/*!
 * @brief ENUM Replica Read Only Access
 */
ENUM(ReplicaReadOnlyAccess, uint32_t, SourceElement, ReplicaElement, Both);

/*!
 * @brief ENUM Replica Type
 */
ENUM(ReplicaType, uint32_t, Mirror, Snapshot, Clone, TokenizedClone);

/*!
 * @brief ENUM Replica Role
 */
ENUM(ReplicaRole, uint32_t, Source, Target);

/*!
 * @brief ENUM Access Capabilities
 */
ENUM(AccessCapability, uint32_t, Read, Write, WriteOnce, Append, Streaming);

/*!
 * @brief ENUM TransportProtocol types as specified by Redfish "Protocol.Protocol"
 */
ENUM(TransportProtocol, uint32_t, PCIe, AHCI, UHCI, SAS, SATA, USB, NVMe, FC, iSCSI, FCoE, FCP, FICON, NVMeOverFabrics,
     SMB, NFSv3, NFSv4, HTTP, HTTPS, FTP, SFTP, iWARP, RoCE, RoCEv2, OEM);

/*!
 * @brief ENUM LogEntryType types as specified by Redfish "LogEntryType"
 */
ENUM(LogEntryType, uint32_t, Event, Multiple, OEM, SEL);

/*!
 * @brief ENUM OverWritePolicy types as specified by Redfish "OverWritePolicy"
 */
ENUM(OverWritePolicy, uint32_t, NeverOverWrites, Unknown, WrapsWhenFull);

/*!
 * @brief ENUM EntryType types as specified by Redfish "EntryType"
 */
ENUM(EntryType, uint32_t, Event, Oem, SEL);

/*!
 * @brief ENUM EventType types as specified by Redfish "EventType"
 */
ENUM(EventType, uint32_t, Alert, ResourceAdded, ResourceRemoved, ResourceUpdated, StatusChange);

/*!
 * @brief ENUM NetworkServiceName for Manager class Network Service subclass
 *  member
 * */
ENUM(NetworkServiceName, uint32_t, HTTP, HTTPS, SNMP, VirtualMedia, Telnet,
     SSDP, IPMI, SSH, KVMIP);

/*!
 * @brief ENUM OperationApplyTime representing Redfish enumeration of the same name from Settings namespace
 * */
ENUM(OperationApplyTime, uint32_t, Immediate, OnReset, AtMaintenanceWindowStart, InMaintenanceWindowOnReset, OnStartUpdateRequest);

/*!
 * @brief ENUM MediaType for VirtualMedia class
 */
ENUM(MediaType, uint32_t, CD, Floppy, USBStick, DVD);

/*!
 * @brief ENUM TransferMethod for VirtualMediaInsert class
 */
ENUM(TransferMethod, uint32_t, Upload);

} // namespace enums
} // namespace model
} // namespace agent_framework
