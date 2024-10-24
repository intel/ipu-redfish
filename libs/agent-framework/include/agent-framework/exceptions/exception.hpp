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
 * @file exception.hpp
 *
 * @brief Definition of THROW macro
 * */

#pragma once

#include "agent-framework/exceptions/invalid_field.hpp"
#include "agent-framework/exceptions/invalid_uuid.hpp"
#include "agent-framework/exceptions/invalid_value.hpp"
#include "agent-framework/exceptions/method_conflict.hpp"
#include "agent-framework/exceptions/not_found.hpp"
#include "agent-framework/exceptions/not_implemented.hpp"

#include "logger/logger.hpp"

/*! @brief Logs and throws exception */
#define THROW(clazz, logger, ...)                              \
    do {                                                       \
        clazz _ex{__VA_ARGS__};                                \
        log_error((logger), #clazz ": " << _ex.get_message()); \
        throw _ex;                                             \
    } while (0)
