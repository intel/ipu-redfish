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
 * @brief Identifier attribute
 * @file identifier.cpp
 */

#include "agent-framework/module/model/attributes/identifier.hpp"

using namespace agent_framework::model::attribute;

Identifier::Identifier(const OptionalField<std::string>& durable_name,
                       const OptionalField<enums::IdentifierType>& durable_format) : m_durable_name(durable_name), m_durable_name_format(durable_format) {}

Identifier::~Identifier() {}
