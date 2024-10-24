/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2018-2019 Intel Corporation
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
 * @file session_service.cpp
 * */

#include "psme/rest/validators/schemas/session_service.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/validators/schemas/common.hpp"

using namespace psme::rest;
using namespace psme::rest::validators::schema;

const jsonrpc::ProcedureValidator& SessionServicePatchSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        jsonrpc::PARAMS_BY_NAME,
        constants::Common::SERVICE_ENABLED, VALID_OPTIONAL(VALID_JSON_BOOLEAN),
        constants::SessionService::SESSION_TIMEOUT, VALID_OPTIONAL(VALID_NUMERIC_RANGE(INT64, 30, 86400)),
        nullptr};
    return procedure;
}
