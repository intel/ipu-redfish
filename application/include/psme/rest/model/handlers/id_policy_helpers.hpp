/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief Helper classes for IdPolicy class template
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
 * @file id_policy_helpers.hpp
 */
#pragma once

#include <cstdint>

namespace psme {
namespace rest {
namespace model {
namespace handler {

enum class NumberingZone : uint8_t {
    PARENT_SPACE,
    SHARED
};

}
} // namespace model
} // namespace rest
} // namespace psme
