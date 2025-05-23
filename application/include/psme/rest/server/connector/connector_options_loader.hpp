/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright Copyright (c) 2018-2019 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file connector_options_loader.hpp
 */

#pragma once

#include "connector_options.hpp"

namespace psme {
namespace rest {
namespace server {

/*!
 * Server options loader
 * @param config JSON configuration
 * @return Server options read from configuration
 */
ConnectorOptions load_server_options(const json::Json& config);

} // namespace server
} // namespace rest
} // namespace psme
