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

namespace psme {
namespace rest {
namespace constants {

namespace Chassis {
const char* CHASSIS_TYPE = "ChassisType";
const char* INDICATOR_LED = "IndicatorLED";
const char* CONTAINS = "Contains";
const char* COMPUTER_SYSTEMS = "ComputerSystems";
const char* ETHERNET_SWITCHES = "EthernetSwitches";
const char* MANAGERS_IN_CHASSIS = "ManagersInChassis";
const char* DRIVES = "Drives";
const char* STORAGE = "Storage";
const char* PCIE_DEVICES = "PCIeDevices";
const char* NETWORK_ADAPTERS = "NetworkAdapters";
} // namespace Chassis

namespace Drive {
const char* CAPABLE_SPEED = "CapableSpeedGbs";
const char* INDICATOR_LED = "IndicatorLED";
const char* ENDPOINTS = "Endpoints";
const char* VOLUMES = "Volumes";
const char* MEDIA_TYPE = "MediaType";
const char* NEGOTIATED_SPEED = "NegotiatedSpeedGbs";
const char* PROTOCOL = "Protocol";
const char* CAPACITY_BYTES = "CapacityBytes";
const char* INFO = "Info";
const char* INFO_FORMAT = "InfoFormat";
const char* ERASE_ON_DETACH = "EraseOnDetach";
const char* STATUS_INDICATOR = "StatusIndicator";
const char* REVISION = "Revision";
const char* FAILURE_PREDICTED = "FailurePredicted";
const char* HOTSPARE_TYPE = "HotspareType";
const char* ENCRYPTION_ABILITY = "EncryptionAbility";
const char* ENCRYPTION_STATUS = "EncryptionStatus";
const char* ROTATION_SPEED_RPM = "RotationSpeedRPM";
const char* BLOCK_SIZE_BYTES = "BlockSizeBytes";
const char* PREDICTED_MEDIA_LIFE_LEFT = "PredictedMediaLifeLeftPercent";
const char* INTERFACE = "Interface";
const char* CAPACITY = "CapacityGiB";
const char* TYPE = "Type";
const char* RPM = "RPM";
const char* BUS_INFO = "BusInfo";
} // namespace Drive

namespace PcieDevice {
const char* PCIE_FUNCTIONS = "PCIeFunctions";
const char* DEVICE_TYPE = "DeviceType";
} // namespace PcieDevice

namespace PcieFunction {
const char* FUNCTION_ID = "FunctionId";
const char* FUNCTION_TYPE = "FunctionType";
const char* FUNCTION_NUMBER = "FunctionNumber";
const char* DEVICE_CLASS = "DeviceClass";
const char* DEVICE_ID = "DeviceId";
const char* VENDOR_ID = "VendorId";
const char* CLASS_CODE = "ClassCode";
const char* REVISION_ID = "RevisionId";
const char* SUBSYSTEM_ID = "SubsystemId";
const char* SUBSYSTEM_VENDOR_ID = "SubsystemVendorId";
const char* PCIE_DEVICE = "PCIeDevice";
} // namespace PcieFunction

} // namespace constants
} // namespace rest
} // namespace psme
