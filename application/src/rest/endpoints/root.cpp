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

#include "agent-framework/module/service_uuid.hpp"
#include "configuration/configuration.hpp"

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/root.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;

namespace {

json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#ServiceRoot.ServiceRoot";
    r[Common::ODATA_ID] = "/redfish/v1";
    r[Common::ODATA_TYPE] = "#ServiceRoot.v1_17_0.ServiceRoot";
    r[Common::ID] = "RootService";
    r[Common::DESCRIPTION] = "Service Root";
    r[Root::REDFISH_VERSION] = "1.21.0";
    r[Common::UUID] = "00000000000-000000-0000000-00000000000000000";
    r[Root::SESSION_SERVICE][Common::ODATA_ID] = "/redfish/v1/SessionService";
    r[Root::UPDATE_SERVICE][Common::ODATA_ID] = "/redfish/v1/UpdateService";
    r[Root::TASKS][Common::ODATA_ID] = "/redfish/v1/TaskService";
    r[Root::REGISTRIES][Common::ODATA_ID] = "/redfish/v1/Registries";
    r[Root::SYSTEMS][Common::ODATA_ID] = "/redfish/v1/Systems";
    r[Root::MANAGERS][Common::ODATA_ID] = "/redfish/v1/Managers";
    r[Root::ACCOUNT_SERVICE][Common::ODATA_ID] = "/redfish/v1/AccountService";
    r[Common::LINKS][SessionService::SESSIONS][Common::ODATA_ID] = "/redfish/v1/SessionService/Sessions";
    return r;
}
} // namespace

endpoint::Root::Root(const std::string& path) : EndpointBase(path) {
    const auto& config = configuration::Configuration::get_instance().to_json();
    m_service_root_name = config.value("service", json::Json()).get<std::string>();
}

endpoint::Root::~Root() {}

void endpoint::Root::get(const server::Request&, server::Response& response) {
    auto json = make_prototype();

    json[Common::UUID] = agent_framework::module::ServiceUuid::get_instance()->get_service_uuid();
    json[Common::NAME] = m_service_root_name;

    set_response(response, json);
}
