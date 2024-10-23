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

#include "psme/rest/validators/schemas/system.hpp"
#include "agent-framework/module/enum/compute.hpp"
#include "psme/rest/constants/constants.hpp"

using namespace psme::rest;
using namespace psme::rest::validators::schema;
using namespace agent_framework::model;

const jsonrpc::ProcedureValidator& SystemPatchSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        jsonrpc::PARAMS_BY_NAME,
        constants::System::BOOT, VALID_ATTRIBUTE(BootSchema),
        nullptr};
    return procedure;
}

const jsonrpc::ProcedureValidator& SystemPatchSchema::BootSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        jsonrpc::PARAMS_BY_NAME,
        constants::System::BOOT_SOURCE_OVERRIDE_ENABLED, VALID_OPTIONAL(VALID_ENUM(enums::BootOverride)),
        constants::System::BOOT_SOURCE_OVERRIDE_TARGET, VALID_OPTIONAL(VALID_ENUM(enums::BootOverrideTarget)),
        nullptr};
    return procedure;
}
