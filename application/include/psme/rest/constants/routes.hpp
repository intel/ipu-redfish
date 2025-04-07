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
 * @file routes.hpp
 * */

#pragma once

#include <string>

namespace psme {
namespace rest {
namespace constants {

class Routes {
public:
    static const std::string REDFISH_PATH;
    static const std::string ROOT_PATH;
    static const std::string ODATA_SERVICE_DOCUMENT;
    static const std::string METADATA_ROOT_PATH;
    static const std::string METADATA_PATH;
    static const std::string UPDATE_SERVICE_PATH;
    static const std::string UPDATE_PATH;
    static const std::string SIMPLE_UPDATE_PATH;
    static const std::string SIMPLE_UPDATE_ACTION_INFO_PATH;
    static const std::string SESSION_SERVICE_PATH;
    static const std::string SESSION_COLLECTION_PATH;
    static const std::string SESSION_PATH;
    static const std::string TASK_SERVICE_PATH;
    static const std::string TASK_COLLECTION_PATH;
    static const std::string TASK_PATH;
    static const std::string MESSAGE_REGISTRY_FILE_COLLECTION_PATH;
    static const std::string MESSAGE_REGISTRY_FILE_PATH;
    static const std::string MESSAGE_REGISTRY_PATH;
    static const std::string MONITOR_PATH;

    static const std::string MANAGER_COLLECTION_PATH;
    static const std::string MANAGER_RESET_PATH;
    static const std::string MANAGER_PATH;

    static const std::string SYSTEMS_COLLECTION_PATH;
    static const std::string SYSTEM_PATH;
    static const std::string SYSTEM_RESET_PATH;
    static const std::string VIRTUAL_MEDIA_COLLECTION_PATH;
    static const std::string VIRTUAL_MEDIA_PATH;
    static const std::string VIRTUAL_MEDIA_INSERT_PATH;
    static const std::string VIRTUAL_MEDIA_EJECT_PATH;

    static const std::string ACCOUNT_SERVICE_PATH;
    static const std::string ACCOUNTS_COLLECTION_PATH;
    static const std::string ACCOUNT_PATH;
    static const std::string ROLES_COLLECTION_PATH;
    static const std::string ROLE_PATH;
};

} // namespace constants
} // namespace rest
} // namespace psme
