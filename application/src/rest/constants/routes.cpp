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

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/path_builder.hpp"

using namespace psme::rest::constants;
using namespace psme::rest::endpoint;

// "/redfish
const std::string Routes::REDFISH_PATH =
    PathBuilder()
        .append(PathParam::REDFISH)
        .build();

// "/redfish/v1"
const std::string Routes::ROOT_PATH =
    PathBuilder(PathParam::BASE_URL)
        .build();

// "/redfish/v1/odata"
const std::string Routes::ODATA_SERVICE_DOCUMENT =
    PathBuilder(PathParam::BASE_URL)
        .append(PathParam::ODATA)
        .build();

// "/redfish/v1/$metadata"
const std::string Routes::METADATA_ROOT_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(PathParam::METADATA_ROOT)
        .build();

// "/redfish/v1/metadata/{metadata_file:*}"
const std::string Routes::METADATA_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(PathParam::METADATA)
        .append_regex(PathParam::METADATA_FILE, PathParam::XML_REGEX)
        .build();

// "/redfish/v1/UpdateService"
const std::string Routes::UPDATE_SERVICE_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::UPDATE_SERVICE)
        .build();

// "/redfish/v1/UpdateService/Actions/UpdateService.SimpleUpdate"
const std::string Routes::SIMPLE_UPDATE_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::UPDATE_SERVICE)
        .append(Common::ACTIONS)
        .append(UpdateService::UPDATE_SERVICE_SIMPLE_UPDATE)
        .build();

// "/redfish/v1/UpdateService/SimpleUpdateActionInfo"
const std::string Routes::SIMPLE_UPDATE_ACTION_INFO_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::UPDATE_SERVICE)
        .append(UpdateService::SIMPLE_UPDATE_ACTION_INFO)
        .build();

// "/redfish/v1/SessionService"
const std::string Routes::SESSION_SERVICE_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::SESSION_SERVICE)
        .build();

// "/redfish/v1/SessionService/Sessions"
const std::string Routes::SESSION_COLLECTION_PATH =
    PathBuilder(SESSION_SERVICE_PATH)
        .append(SessionService::SESSIONS)
        .build();

// "/redfish/v1/SessionService/Sessions/{sessionId:[0-9]+}"
const std::string Routes::SESSION_PATH =
    PathBuilder(SESSION_COLLECTION_PATH)
        .append_regex(PathParam::SESSION_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/MessageRegistryFiles"
const std::string Routes::MESSAGE_REGISTRY_FILE_COLLECTION_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::REGISTRIES)
        .build();

// "/redfish/v1/Registries/{MessageRegistryFileId: [0-9]+}"
const std::string Routes::MESSAGE_REGISTRY_FILE_PATH =
    PathBuilder(MESSAGE_REGISTRY_FILE_COLLECTION_PATH)
        .append_regex(PathParam::MESSAGE_REGISTRY_FILE_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/TaskService"
const std::string Routes::TASK_SERVICE_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(TaskService::TASK_SERVICE)
        .build();

// "/redfish/v1/TaskService/Tasks"
const std::string Routes::TASK_COLLECTION_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(TaskService::TASK_SERVICE)
        .append(Root::TASKS)
        .build();

// "/redfish/v1/TaskService/Tasks/{TaskId:[0-9]+}"
const std::string Routes::TASK_PATH =
    PathBuilder(TASK_COLLECTION_PATH)
        .append_regex(PathParam::TASK_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/TaskService/TaskMonitors/{TaskId:[0-9]+}"
const std::string Routes::MONITOR_PATH =
    PathBuilder(TASK_SERVICE_PATH)
        .append(Monitor::TASK_MONITORS)
        .append_regex(PathParam::TASK_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Managers"
const std::string Routes::MANAGER_COLLECTION_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::MANAGERS)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}"
const std::string Routes::MANAGER_PATH =
    PathBuilder(MANAGER_COLLECTION_PATH)
        .append_regex(PathParam::MANAGER_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Managers/{managerId:[0-9]+}/Actions/Manager.Reset"
const std::string Routes::MANAGER_RESET_PATH =
    PathBuilder(MANAGER_PATH)
        .append(Common::ACTIONS)
        .append(Manager::MANAGER_RESET)
        .build();

// "/redfish/v1/Systems"
const std::string Routes::SYSTEMS_COLLECTION_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::SYSTEMS)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}"
const std::string Routes::SYSTEM_PATH =
    PathBuilder(SYSTEMS_COLLECTION_PATH)
        .append_regex(PathParam::SYSTEM_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/Actions/ComputerSystem.Reset"
const std::string Routes::SYSTEM_RESET_PATH =
    PathBuilder(SYSTEM_PATH)
        .append(Common::ACTIONS)
        .append(System::COMPUTER_SYSTEM_RESET)
        .build();

// "redfish/v1/Systems/{systemId:[0-9]+}/VirtualMedia"
const std::string Routes::VIRTUAL_MEDIA_COLLECTION_PATH =
    PathBuilder(SYSTEM_PATH)
        .append(Common::VIRTUAL_MEDIA)
        .build();

// "redfish/v1/Systems/{systemId:[0-9]+}/VirtualMedia/{virtualMediaId:[0-9]+}"
const std::string Routes::VIRTUAL_MEDIA_PATH =
    PathBuilder(VIRTUAL_MEDIA_COLLECTION_PATH)
        .append_regex(PathParam::VIRTUAL_MEDIA_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/VirtualMedia/{virtualMediaId:[0-9]+}/Actions/VirtualMedia.InsertMedia"
const std::string Routes::VIRTUAL_MEDIA_INSERT_PATH =
    PathBuilder(Routes::VIRTUAL_MEDIA_PATH)
        .append(Common::ACTIONS)
        .append(Common::VIRTUAL_MEDIA_INSERT)
        .build();

// "/redfish/v1/Systems/{systemId:[0-9]+}/VirtualMedia/{virtualMediaId:[0-9]+}/Actions/VirtualMedia.EjectMedia"
const std::string Routes::VIRTUAL_MEDIA_EJECT_PATH =
    PathBuilder(Routes::VIRTUAL_MEDIA_PATH)
        .append(Common::ACTIONS)
        .append(Common::VIRTUAL_MEDIA_EJECT)
        .build();

// "/redfish/v1/AccountService"
const std::string Routes::ACCOUNT_SERVICE_PATH =
    PathBuilder(PathParam::BASE_URL)
        .append(Root::ACCOUNT_SERVICE)
        .build();

// "/redfish/v1/AccountService/Accounts"
const std::string Routes::ACCOUNTS_COLLECTION_PATH =
    PathBuilder(ACCOUNT_SERVICE_PATH)
        .append(AccountService::ACCOUNTS)
        .build();

// "/redfish/v1/AccountService/Accounts/{accountId}
const std::string Routes::ACCOUNT_PATH =
    PathBuilder(ACCOUNTS_COLLECTION_PATH)
        .append_regex(PathParam::ACCOUNT_ID, PathParam::ID_REGEX)
        .build();

// "/redfish/v1/AccountService/Roles"
const std::string Routes::ROLES_COLLECTION_PATH =
    PathBuilder(ACCOUNT_SERVICE_PATH)
        .append(AccountService::ROLES)
        .build();

// "/redfish/v1/AccountService/Roles/{rolesId}"
const std::string Routes::ROLE_PATH =
    PathBuilder(ROLES_COLLECTION_PATH)
        .append_regex(PathParam::ROLE_ID, PathParam::STRING_ID_REGEX)
        .build();
