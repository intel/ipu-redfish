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

#include "psme/rest/endpoints/manager/manager_network_interface_collection.hpp"
#include "agent-framework/module/model/manager.hpp"
#include "configuration/configuration.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/utils.hpp"

using namespace psme::rest;
using namespace psme::rest::endpoint;
using namespace psme::rest::constants;

namespace {
json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EthernetInterfaceCollection.EthernetInterfaceCollection";
    r[Common::ODATA_ID] = json::Json::value_t::null;
    r[Common::ODATA_TYPE] = "#EthernetInterfaceCollection.EthernetInterfaceCollection";
    r[Common::NAME] = "Ethernet Network Interface Collection";
    r[Common::DESCRIPTION] = "Collection of Manager Ethernet Interfaces";
    r[Collection::ODATA_COUNT] = json::Json::value_t::null;
    r[Collection::MEMBERS] = json::Json::value_t::array;

    return r;
}
} // namespace

ManagerNetworkInterfaceCollection::ManagerNetworkInterfaceCollection(const std::string& path) : EndpointBase(path) {}

ManagerNetworkInterfaceCollection::~ManagerNetworkInterfaceCollection() {}

void ManagerNetworkInterfaceCollection::get(const server::Request& request, server::Response& response) {
    auto json = ::make_prototype();

    json[Common::ODATA_ID] = PathBuilder(request).build();

    auto manager = psme::rest::model::find<agent_framework::model::Manager>(request.params).get();
    auto nic_ids = agent_framework::module::get_manager<agent_framework::model::NetworkInterface>()
                       .get_ids(manager.get_uuid());

    json[Collection::ODATA_COUNT] = nic_ids.size();

    for (const auto& nic_id : nic_ids) {
        json::Json link(json::Json::value_t::object);
        link[Common::ODATA_ID] = PathBuilder(request).append(nic_id).build();
        json[Collection::MEMBERS].push_back(std::move(link));
    }

    set_response(response, json);
}
