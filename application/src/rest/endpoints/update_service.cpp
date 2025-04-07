/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2017-2019 Intel Corporation
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

#include "psme/rest/endpoints/update_service.hpp"
#include "psme/rest/constants/constants.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace agent_framework::model::enums;

namespace {

json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#UpdateService.UpdateService";
    r[Common::ODATA_TYPE] = "#UpdateService.v1_14_1.UpdateService";
    r[Common::ODATA_ID] = json::Json::value_t::null;

    r[Common::ID] = "UpdateService";
    r[Common::NAME] = "Update service";
    r[Common::DESCRIPTION] = "Redfish Server Update service";

    r[Common::STATUS][Common::STATE] = "Enabled";
    r[Common::STATUS][Common::HEALTH] = "OK";
    r[Common::STATUS][Common::HEALTH_ROLLUP] = "OK";

    r[Common::SERVICE_ENABLED] = true;

    json::Json simple_update(json::Json::value_t::object);
    simple_update[Common::TARGET] = json::Json::value_t::null;
    simple_update[ActionInfo::REDFISH_ACTION_INFO] = json::Json::value_t::null;
    r[Common::ACTIONS][UpdateService::HASH_UPDATE_SERVICE_SIMPLE_UPDATE] = std::move(simple_update);

    return r;
}
} // namespace

endpoint::UpdateService::UpdateService(const std::string& path) : EndpointBase(path) {}

endpoint::UpdateService::~UpdateService() {}

void endpoint::UpdateService::get(const server::Request& request, server::Response& response) {
    auto r = make_prototype();

    r[Common::ODATA_ID] = PathBuilder(request).build();

    r[Common::ACTIONS][constants::UpdateService::HASH_UPDATE_SERVICE_SIMPLE_UPDATE][Common::TARGET] =
        PathBuilder(request).append(Common::ACTIONS).append(constants::UpdateService::UPDATE_SERVICE_SIMPLE_UPDATE).build();
    r[Common::ACTIONS][constants::UpdateService::HASH_UPDATE_SERVICE_SIMPLE_UPDATE][ActionInfo::REDFISH_ACTION_INFO] =
        PathBuilder(request).append(constants::UpdateService::SIMPLE_UPDATE_ACTION_INFO).build();

    set_response(response, r);
}
