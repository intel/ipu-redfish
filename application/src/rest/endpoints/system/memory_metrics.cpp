/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief MemoryMetrics endpoint implementation
 *
 * @copyright Copyright (c) 2017-2019 Intel Corporation.
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
 * @file memory_metrics.cpp
 */

#include "psme/rest/endpoints/system/memory_metrics.hpp"
#include "psme/rest/utils/status_helpers.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace agent_framework::model::enums;

namespace {
json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#MemoryMetrics.MemoryMetrics";
    r[Common::ODATA_ID] = json::Json::value_t::null;
    r[Common::ODATA_TYPE] = "#MemoryMetrics.v1_1_2.MemoryMetrics";
    r[Common::ID] = json::Json::value_t::null;
    r[Common::NAME] = "Memory Metrics";
    r[Common::DESCRIPTION] = "Memory Metrics";
    r[MemoryMetrics::BLOCK_SIZE_BYTES] = json::Json::value_t::null;

    json::Json current_period = json::Json();
    current_period[MemoryMetrics::BLOCKS_READ] = json::Json::value_t::null;
    current_period[MemoryMetrics::BLOCKS_WRITTEN] = json::Json::value_t::null;
    r[MemoryMetrics::CURRENT_PERIOD] = std::move(current_period);

    json::Json life_time = json::Json();
    life_time[MemoryMetrics::BLOCKS_READ] = json::Json::value_t::null;
    life_time[MemoryMetrics::BLOCKS_WRITTEN] = json::Json::value_t::null;
    r[MemoryMetrics::LIFE_TIME] = std::move(life_time);

    json::Json alarm_trips = json::Json();
    alarm_trips[MemoryMetrics::TEMPERATURE] = json::Json::value_t::null;
    alarm_trips[MemoryMetrics::SPARE_BLOCK] = json::Json::value_t::null;
    alarm_trips[MemoryMetrics::UNCORRECTABLE_ECC_ERROR] = json::Json::value_t::null;
    alarm_trips[MemoryMetrics::CORRECTABLE_ECC_ERROR] = json::Json::value_t::null;
    alarm_trips[MemoryMetrics::ADDRESS_PARITY_ERROR] = json::Json::value_t::null;

    json::Json health_data = json::Json();
    health_data[Common::ODATA_TYPE] = "#MemoryMetrics.v1_1_0.HealthData";
    health_data[MemoryMetrics::REMAINING_SPARE_BLOCK_PERCENTAGE] = json::Json::value_t::null;
    health_data[MemoryMetrics::LAST_SHUTDOWN_SUCCESS] = json::Json::value_t::null;
    health_data[MemoryMetrics::DATA_LOSS_DETECTED] = json::Json::value_t::null;
    health_data[MemoryMetrics::PERFORMANCE_DEGRADED] = json::Json::value_t::null;
    health_data[MemoryMetrics::PREDICTED_MEDIA_LIFE_LEFT_PERCENT] = json::Json::value_t::null;
    health_data[MemoryMetrics::ALARM_TRIPS] = std::move(alarm_trips);
    r[MemoryMetrics::HEALTH_DATA] = std::move(health_data);

    return r;
}

} // namespace

endpoint::MemoryMetrics::MemoryMetrics(const std::string& path) : EndpointBase(path) {}

endpoint::MemoryMetrics::~MemoryMetrics() {}

void endpoint::MemoryMetrics::get(const server::Request& req, server::Response& res) {
    using agent_framework::model::Memory;
    using agent_framework::model::System;

    auto memory_uuid = psme::rest::model::find<System, Memory>(req.params).get_uuid();
    auto json = make_prototype();

    json[Common::ODATA_ID] = PathBuilder(req).build();
    json[Common::ID] = constants::Common::METRICS;
    json[Common::NAME] = "Memory " + req.params[PathParam::MEMORY_ID] + " Metrics";
    json[constants::MemoryMetrics::BLOCK_SIZE_BYTES] = 64;
    utils::populate_metrics(json, memory_uuid);

    set_response(res, json);
}
