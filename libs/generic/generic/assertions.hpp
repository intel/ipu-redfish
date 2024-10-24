/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief Defines macro that might be used in assert to raise assertion with message
 *
 * @copyright Copyright (c) 2016-2019 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file assertions.hpp
 */
#pragma once

#include <cassert>

namespace generic {

/*! FAIL condition for assert. Used to raise assertion with message when given condition is met.
 * */
constexpr bool FAIL(const char*, bool raise_when = true) {
    return !raise_when;
}

} // namespace generic