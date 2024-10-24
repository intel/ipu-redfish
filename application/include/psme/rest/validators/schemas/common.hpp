/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2016-2019 Intel Corporation
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

#include "agent-framework/validators/procedure_validator.hpp"

namespace psme {
namespace rest {
namespace validators {
namespace schema {

/*! @brief Schema for validating simple redfish object with odata.id property. */
class SimpleObjectSchema {
public:
    static const jsonrpc::ProcedureValidator& get_procedure();
};

/*! @brief Schema for validating empty JSON object. */
class EmptyObjectSchema {
public:
    static const jsonrpc::ProcedureValidator& get_procedure();
};

} // namespace schema
} // namespace validators
} // namespace rest
} // namespace psme