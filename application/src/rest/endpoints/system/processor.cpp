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

#include "psme/rest/endpoints/system/processor.hpp"
#include "psme/rest/endpoints/chassis/pcie_device.hpp"
#include "psme/rest/endpoints/chassis/pcie_function.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/utils/status_helpers.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::endpoint::utils;

namespace {

json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#Processor.Processor";
    r[Common::ODATA_ID] = json::Json::value_t::null;
    r[Common::ODATA_TYPE] = "#Processor.v1_4_0.Processor";
    r[Common::ID] = json::Json::value_t::null;
    r[Common::NAME] = "Processor";
    r[Common::DESCRIPTION] = "Processor Description";
    r[Processor::SOCKET] = json::Json::value_t::null;
    r[Common::MODEL] = json::Json::value_t::null;
    r[Common::MANUFACTURER] = json::Json::value_t::null;
    r[Processor::PROCESSOR_TYPE] = json::Json::value_t::null;
    r[Processor::PROCESSOR_ARCHITECTURE] = json::Json::value_t::null;
    r[Processor::INSTRUCTION_SET] = json::Json::value_t::null;
    r[Processor::PROCESSOR_ID][Common::VENDOR_ID] = json::Json::value_t::null;
    r[Processor::PROCESSOR_ID][Processor::IDENTIFICATION_REGISTERS] = json::Json::value_t::null;
    r[Processor::PROCESSOR_ID][Processor::EFFECTIVE_FAMILY] = json::Json::value_t::null;
    r[Processor::PROCESSOR_ID][Processor::EFFECTIVE_MODEL] = json::Json::value_t::null;
    r[Processor::PROCESSOR_ID][Processor::STEP] = json::Json::value_t::null;
    r[Processor::PROCESSOR_ID][Processor::MICROCODE_INFO] = json::Json::value_t::null;
    r[Processor::MAX_SPEED] = json::Json::value_t::null;
    r[Processor::TOTAL_CORES] = json::Json::value_t::null;
    r[Processor::TOTAL_THREADS] = json::Json::value_t::null;
    r[Common::STATUS][Common::STATE] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Json::value_t::null;

    r[Common::LINKS][Common::ODATA_TYPE] = "#Processor.v1_4_0.Links";
    r[Common::LINKS][constants::Processor::ENDPOINTS] = json::Json::value_t::array;

    return r;
}

void fill_cpu_id(const agent_framework::model::Processor& processor, json::Json& json) {
    const auto& cpu_id = processor.get_cpu_id();
    json[constants::Processor::PROCESSOR_ID][Common::VENDOR_ID] = cpu_id.get_vendor_id();
    json[constants::Processor::PROCESSOR_ID][constants::Processor::IDENTIFICATION_REGISTERS] = cpu_id.get_numeric_id();
    json[constants::Processor::PROCESSOR_ID][constants::Processor::EFFECTIVE_FAMILY] = cpu_id.get_family();
    json[constants::Processor::PROCESSOR_ID][constants::Processor::EFFECTIVE_MODEL] = cpu_id.get_model();
    json[constants::Processor::PROCESSOR_ID][constants::Processor::STEP] = cpu_id.get_step();
    json[constants::Processor::PROCESSOR_ID][constants::Processor::MICROCODE_INFO] = cpu_id.get_microcode_info();
}

} // namespace

endpoint::Processor::Processor(const std::string& path) : EndpointBase(path) {}

endpoint::Processor::~Processor() {}

void endpoint::Processor::get(const server::Request& req, server::Response& res) {
    auto json = make_prototype();

    auto processor = psme::rest::model::find<agent_framework::model::System, agent_framework::model::Processor>(
                         req.params)
                         .get();

    json[Common::ODATA_ID] = PathBuilder(req).build();
    json[constants::Common::ID] = req.params[PathParam::PROCESSOR_ID];
    json[constants::Processor::SOCKET] = processor.get_socket();
    json[constants::Common::MODEL] = processor.get_model_name();
    json[constants::Common::MANUFACTURER] = processor.get_manufacturer();
    json[constants::Processor::PROCESSOR_TYPE] = processor.get_processor_type();
    json[constants::Processor::PROCESSOR_ARCHITECTURE] = processor.get_processor_architecture();
    json[constants::Processor::INSTRUCTION_SET] = processor.get_instruction_set();
    json[constants::Processor::TOTAL_CORES] = processor.get_total_cores();

    endpoint::status_to_json(processor, json);
    json[Common::STATUS][Common::HEALTH_ROLLUP] = processor.get_status().get_health();

    if (agent_framework::model::enums::ProcessorType::CPU == processor.get_processor_type()) {
        fill_cpu_id(processor, json);
        json[constants::Processor::MAX_SPEED] = processor.get_max_speed_mhz();
        json[constants::Processor::TOTAL_THREADS] = processor.get_total_threads();
    }
    json[constants::Common::METRICS][Common::ODATA_ID] =
        PathBuilder(req).append(constants::Common::METRICS).build();

    set_response(res, json);
}
