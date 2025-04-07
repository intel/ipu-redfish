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

#include "psme/rest/endpoints/endpoint_builder.hpp"
#include "psme/rest/constants/routes.hpp"
#include "psme/rest/endpoints/endpoints.hpp"
#include "psme/rest/server/multiplexer.hpp"
#include "psme/rest/server/utils.hpp"

using namespace psme::rest;
using namespace psme::rest::endpoint;
using namespace psme::rest::server;

EndpointBuilder::~EndpointBuilder() {}

void EndpointBuilder::build_endpoints() {
    auto& mp = *(psme::rest::server::Multiplexer::get_instance());
    mp.use_before([this](const Request&, Response& res) {
        res.set_header(ContentType::CONTENT_TYPE, ContentType::JSON);
    });

    // "/redfish
    mp.register_handler(Redfish::UPtr(new Redfish(constants::Routes::REDFISH_PATH)));

    // "/redfish/v1"
    mp.register_handler(Root::UPtr(new Root(constants::Routes::ROOT_PATH)));

    // "/redfish/v1/odata"
    mp.register_handler(Root::UPtr(new OdataServiceDocument(constants::Routes::ODATA_SERVICE_DOCUMENT)));

    // "/redfish/v1/$metadata"
    mp.register_handler(MetadataRoot::UPtr(new MetadataRoot(constants::Routes::METADATA_ROOT_PATH)));

    // "/redfish/v1/metadata/{metadata_file:*}"
    mp.register_handler(Metadata::UPtr(new Metadata(constants::Routes::METADATA_PATH)));

    // "/redfish/v1/UpdateService"
    mp.register_handler(UpdateService::UPtr(new UpdateService(constants::Routes::UPDATE_SERVICE_PATH)));

    // "/redfish/v1/UpdateService/SimpleUpdateActionInfo"
    mp.register_handler(
        SimpleUpdateActionInfo::UPtr(new SimpleUpdateActionInfo(constants::Routes::SIMPLE_UPDATE_ACTION_INFO_PATH)));

    // "/redfish/v1/UpdateService/Actions/SimpleUpdate"
    mp.register_handler(SimpleUpdate::UPtr(new SimpleUpdate(constants::Routes::SIMPLE_UPDATE_PATH)));

    // "/redfish/v1/SessionService"
    mp.register_handler(SessionService::UPtr(new SessionService(constants::Routes::SESSION_SERVICE_PATH)));

    // "/redfish/v1/SessionService/Sessions"
    mp.register_handler(SessionCollection::UPtr(new SessionCollection(constants::Routes::SESSION_COLLECTION_PATH)));

    // "/redfish/v1/SessionService/Sessions/{sessionId:[0-9]+}"
    mp.register_handler(Session::UPtr(new Session(constants::Routes::SESSION_PATH)));

    // "/redfish/v1/Registries"
    mp.register_handler(MessageRegistryFileCollection::UPtr(
        new MessageRegistryFileCollection(constants::Routes::MESSAGE_REGISTRY_FILE_COLLECTION_PATH)));

    // "/redfish/v1/Registries/{MessageRegistryFileId: [0-9]+}"
    mp.register_handler(MessageRegistryFile::UPtr(
        new MessageRegistryFile(constants::Routes::MESSAGE_REGISTRY_FILE_PATH)));

    // "/redfish/v1/TaskService"
    mp.register_handler(TaskService::UPtr(new TaskService(constants::Routes::TASK_SERVICE_PATH)));

    // "/redfish/v1/TaskService/Tasks"
    mp.register_handler(TaskCollection::UPtr(new TaskCollection(constants::Routes::TASK_COLLECTION_PATH)));

    // "/redfish/v1/Task/Service/Tasks/{taskId:[0-9]+}"
    mp.register_handler(Task::UPtr(new Task(constants::Routes::TASK_PATH)));

    // "/redfish/v1/Task/Service/Tasks/{taskId:[0-9]+}/Monitor"
    mp.register_handler(Monitor::UPtr(new Monitor(constants::Routes::MONITOR_PATH)));

    // "/redfish/v1/Managers"
    mp.register_handler(ManagerCollection::UPtr(new ManagerCollection(constants::Routes::MANAGER_COLLECTION_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}"
    mp.register_handler(Manager::UPtr(new Manager(constants::Routes::MANAGER_PATH)));

    // "/redfish/v1/Managers/{managerId:[0-9]+}/Actions/Manager.Reset"
    mp.register_handler(ManagerReset::UPtr(new ManagerReset(constants::Routes::MANAGER_RESET_PATH)));

    // "/redfish/v1/Systems"
    mp.register_handler(SystemsCollection::UPtr(new SystemsCollection(constants::Routes::SYSTEMS_COLLECTION_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}"
    mp.register_handler(System::UPtr(new System(constants::Routes::SYSTEM_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/Actions/ComputerSystem.Reset"
    mp.register_handler(SystemReset::UPtr(new SystemReset(constants::Routes::SYSTEM_RESET_PATH)));

    // "redfish/v1/Systems/{systemId:[0-9]+}+/VirtualMedia"
    mp.register_handler(VirtualMediaCollection::UPtr(
        new VirtualMediaCollection(constants::Routes::VIRTUAL_MEDIA_COLLECTION_PATH)));

    // "redfish/v1/Systems/{systemId:[0-9]+}+/VirtualMedia/{virtualMediaId:[0-9]+}"
    mp.register_handler(VirtualMedia::UPtr(
        new VirtualMedia(constants::Routes::VIRTUAL_MEDIA_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/VirtualMedia/{virtualMediaId:[0-9]+}/Actions/VirtualMedia.InsertMedia"
    mp.register_handler(VirtualMediaInsert::UPtr(new VirtualMediaInsert(constants::Routes::VIRTUAL_MEDIA_INSERT_PATH)));

    // "/redfish/v1/Systems/{systemId:[0-9]+}/VirtualMedia/{virtualMediaId:[0-9]+}/Actions/VirtualMedia.EjectMedia"
    mp.register_handler(VirtualMediaInsert::UPtr(new VirtualMediaEject(constants::Routes::VIRTUAL_MEDIA_EJECT_PATH)));

    // "/redfish/v1/AccountService"
    mp.register_handler(AccountService::UPtr(new AccountService(constants::Routes::ACCOUNT_SERVICE_PATH)));

    // "/redfish/v1/AccountService/Accounts"
    mp.register_handler(AccountCollection::UPtr(new AccountCollection(constants::Routes::ACCOUNTS_COLLECTION_PATH)));

    // "/redfish/v1/AccountService/Accounts/{accountId}"
    mp.register_handler(Account::UPtr(new Account(constants::Routes::ACCOUNT_PATH)));

    // "/redfish/v1/AccountService/Roles"
    mp.register_handler(RoleCollection::UPtr(new RoleCollection(constants::Routes::ROLES_COLLECTION_PATH)));

    // "/redfish/v1/AccountService/Roles/{rolesId}"
    mp.register_handler(Role::UPtr(new Role(constants::Routes::ROLE_PATH)));
}
