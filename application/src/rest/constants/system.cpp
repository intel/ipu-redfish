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

#include "psme/rest/constants/system.hpp"

namespace psme {
namespace rest {
namespace constants {

namespace System {
const char* SYSTEM_TYPE = "SystemType";
const char* HOST_NAME = "HostName";
const char* INDICATOR_LED = "IndicatorLED";
const char* POWER_STATE = "PowerState";
const char* BOOT = "Boot";
const char* BOOT_MODE = "BootMode";
const char* BOOT_SOURCE_OVERRIDE_ENABLED = "BootSourceOverrideEnabled";
const char* BOOT_SOURCE_OVERRIDE_ENABLED_ALLOWABLE_VALUES = "BootSourceOverrideEnabled@Redfish.AllowableValues";
const char* BOOT_SOURCE_OVERRIDE_TARGET = "BootSourceOverrideTarget";
const char* BOOT_SOURCE_OVERRIDE_TARGET_ALLOWABLE_VALUES = "BootSourceOverrideTarget@Redfish.AllowableValues";
const char* BOOT_SOURCE_OVERRIDE_MODE = "BootSourceOverrideMode";
const char* UEFI_TARGET = "UefiTargetBootSourceOverride";
const char* BIOS_VERSION = "BiosVersion";
const char* HASH_COMPUTER_SYSTEM_RESET = "#ComputerSystem.Reset";
const char* COMPUTER_SYSTEM_RESET = "ComputerSystem.Reset";

} // namespace System

} // namespace constants
} // namespace rest
} // namespace psme
