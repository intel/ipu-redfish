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
 * @brief Constant literals for System endpoint.
 */
namespace System {
extern const char* SYSTEM_TYPE;
extern const char* HOST_NAME;
extern const char* INDICATOR_LED;
extern const char* POWER_STATE;
extern const char* BOOT;
extern const char* BOOT_MODE;
extern const char* BOOT_SOURCE_OVERRIDE_ENABLED;
extern const char* BOOT_SOURCE_OVERRIDE_ENABLED_ALLOWABLE_VALUES;
extern const char* BOOT_SOURCE_OVERRIDE_TARGET;
extern const char* BOOT_SOURCE_OVERRIDE_TARGET_ALLOWABLE_VALUES;
extern const char* BOOT_SOURCE_OVERRIDE_MODE;
extern const char* UEFI_TARGET;
extern const char* BIOS_VERSION;
extern const char* HASH_COMPUTER_SYSTEM_RESET;
extern const char* COMPUTER_SYSTEM_RESET;
} // namespace System

} // namespace constants
} // namespace rest
} // namespace psme
