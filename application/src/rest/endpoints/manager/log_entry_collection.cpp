/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2019 Intel Corporation
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

#include "psme/rest/endpoints/manager/log_entry_collection.hpp"
#include "agent-framework/module/common_components.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/endpoints.hpp"
#include "psme/rest/endpoints/utils.hpp"

using namespace psme::rest;
using namespace psme::rest::endpoint;
using namespace psme::rest::constants;
using namespace agent_framework::module;

namespace {
json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#LogEntryCollection.LogEntryCollection";
    r[Common::ODATA_ID] = json::Json::value_t::null;
    r[Common::ODATA_TYPE] = "#LogEntryCollection.LogEntryCollection";
    r[Common::NAME] = "Log Entry Collection";
    r[Common::DESCRIPTION] = "Collection of LogEntries for this Log Service";
    r[Collection::ODATA_COUNT] = json::Json::value_t::null;
    r[Collection::MEMBERS] = json::Json::value_t::array;

    return r;
}
} // namespace

LogEntryCollection::LogEntryCollection(const std::string& path) : EndpointBase(path) {}

LogEntryCollection::~LogEntryCollection() {}

void LogEntryCollection::get(const server::Request& request, server::Response& response) {
    auto r = ::make_prototype();

    r[Common::ODATA_ID] = PathBuilder(request).build();

    auto log_service_uuid = psme::rest::model::find<agent_framework::model::Manager, agent_framework::model::LogService>(request.params).get_uuid();
    auto log_entry_ids = get_manager<agent_framework::model::LogEntry>().get_ids(log_service_uuid);

    r[Collection::ODATA_COUNT] = std::uint32_t(log_entry_ids.size());

    for (const auto& log_entry_id : log_entry_ids) {
        auto log_entry = get_manager<agent_framework::model::LogEntry>().get_entry(get_manager<agent_framework::model::LogEntry>().rest_id_to_uuid(log_entry_id));
        auto log_entry_json = LogEntry::get_log_entry_json(log_entry);
        log_entry_json[Common::ODATA_ID] = PathBuilder(request).append(log_entry_id).build();
        r[Collection::MEMBERS].push_back(std::move(log_entry_json));
    }

    set_response(response, r);
}
