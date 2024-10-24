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

#include <string>

namespace psme {
namespace rest {
namespace constants {

/*!
 * @brief Constant literals for Chassis endpoint.
 */
namespace Chassis {
extern const char* CHASSIS_TYPE;
extern const char* INDICATOR_LED;
extern const char* CONTAINS;
extern const char* COMPUTER_SYSTEMS;
extern const char* ETHERNET_SWITCHES;
extern const char* MANAGERS_IN_CHASSIS;
extern const char* DRIVES;
extern const char* STORAGE;
extern const char* PCIE_DEVICES;
extern const char* NETWORK_ADAPTERS;
} // namespace Chassis

/*!
 * @brief Constant literals for Chassis/n/Drive endpoint.
 */
namespace Drive {
extern const char* CAPABLE_SPEED;
extern const char* INDICATOR_LED;
extern const char* ENDPOINTS;
extern const char* VOLUMES;
extern const char* MEDIA_TYPE;
extern const char* NEGOTIATED_SPEED;
extern const char* PROTOCOL;
extern const char* CAPACITY_BYTES;
extern const char* INFO;
extern const char* INFO_FORMAT;
extern const char* STATUS_INDICATOR;
extern const char* REVISION;
extern const char* FAILURE_PREDICTED;
extern const char* HOTSPARE_TYPE;
extern const char* ENCRYPTION_ABILITY;
extern const char* ENCRYPTION_STATUS;
extern const char* ROTATION_SPEED_RPM;
extern const char* BLOCK_SIZE_BYTES;
extern const char* PREDICTED_MEDIA_LIFE_LEFT;
extern const char* INTERFACE;
extern const char* CAPACITY;
extern const char* TYPE;
extern const char* RPM;
extern const char* BUS_INFO;
} // namespace Drive

/*!
 * @brief Constant literals for PcieDevice endpoint.
 */
namespace PcieDevice {
extern const char* PCIE_FUNCTIONS;
extern const char* DEVICE_TYPE;
} // namespace PcieDevice

/*!
 * @brief Constant literals for PcieFunction endpoint.
 */
namespace PcieFunction {
extern const char* FUNCTION_ID;
extern const char* FUNCTION_TYPE;
extern const char* FUNCTION_NUMBER;
extern const char* DEVICE_CLASS;
extern const char* DEVICE_ID;
extern const char* VENDOR_ID;
extern const char* CLASS_CODE;
extern const char* REVISION_ID;
extern const char* SUBSYSTEM_ID;
extern const char* SUBSYSTEM_VENDOR_ID;
extern const char* PCIE_DEVICE;
} // namespace PcieFunction

} // namespace constants
} // namespace rest
} // namespace psme
