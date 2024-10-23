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

#include "psme/rest/endpoints/chassis/drive.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/utils/status_helpers.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::endpoint::utils;

namespace {

json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#Drive.Drive";
    r[Common::ODATA_ID] = json::Json::value_t::null;
    r[Common::ODATA_TYPE] = "#Drive.v1_2_0.Drive";
    r[Common::ID] = json::Json::value_t::null;
    r[Common::NAME] = "Drive";
    r[Common::DESCRIPTION] = "Drive description";

    r[Common::STATUS][Common::STATE] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Json::value_t::null;

    r[Common::ACTIONS] = json::Json::value_t::object;
    r[Common::ASSET_TAG] = json::Json::value_t::null;
    r[Drive::INDICATOR_LED] = json::Json::value_t::null;

    r[Common::LINKS][Common::ODATA_TYPE] = "#Drive.v1_2_0.Links";
    r[Common::LINKS][Drive::ENDPOINTS] = json::Json::value_t::array;
    r[Common::LINKS][Drive::VOLUMES] = json::Json::value_t::array;
    r[Common::LINKS][Common::CHASSIS] = json::Json::value_t::object;

    r[Common::LOCATION] = json::Json::value_t::array;
    r[Common::IDENTIFIERS] = json::Json::value_t::array;

    r[Common::MANUFACTURER] = json::Json::value_t::null;
    r[Drive::MEDIA_TYPE] = json::Json::value_t::null;
    r[Common::MODEL] = json::Json::value_t::null;
    r[Drive::CAPABLE_SPEED] = json::Json::value_t::null;
    r[Drive::NEGOTIATED_SPEED] = json::Json::value_t::null;
    r[Common::PART_NUMBER] = json::Json::value_t::null;
    r[Drive::PROTOCOL] = json::Json::value_t::null;
    r[Common::SERIAL_NUMBER] = json::Json::value_t::null;
    r[Drive::CAPACITY_BYTES] = json::Json::value_t::null;

    r[Common::SKU] = json::Json::value_t::null;
    r[Drive::STATUS_INDICATOR] = json::Json::value_t::null;
    r[Drive::REVISION] = json::Json::value_t::null;
    r[Drive::FAILURE_PREDICTED] = json::Json::value_t::null;
    r[Common::ASSET_TAG] = json::Json::value_t::null;
    r[Drive::HOTSPARE_TYPE] = json::Json::value_t::null;
    r[Drive::ENCRYPTION_ABILITY] = json::Json::value_t::null;
    r[Drive::ENCRYPTION_STATUS] = json::Json::value_t::null;
    r[Drive::ROTATION_SPEED_RPM] = json::Json::value_t::null;
    r[Drive::BLOCK_SIZE_BYTES] = json::Json::value_t::null;
    r[Drive::PREDICTED_MEDIA_LIFE_LEFT] = json::Json::value_t::null;

    return r;
}

} // namespace

endpoint::Drive::Drive(const std::string& path) : EndpointBase(path) {}

endpoint::Drive::~Drive() {}

void endpoint::Drive::get(const server::Request& req, server::Response& res) {
    auto r = make_prototype();

    r[Common::ODATA_ID] = PathBuilder(req).build();

    auto drive = psme::rest::model::find<agent_framework::model::Chassis, agent_framework::model::Drive>(
                     req.params)
                     .get();

    r[Common::ID] = req.params[PathParam::DRIVE_ID];

    fill_name_and_description(drive, r);

    const auto& fru = drive.get_fru_info();
    r[Common::MANUFACTURER] = fru.get_manufacturer();
    r[Common::MODEL] = fru.get_model_number();
    r[Common::PART_NUMBER] = fru.get_part_number();
    r[Common::SERIAL_NUMBER] = fru.get_serial_number();

    r[Common::ASSET_TAG] = drive.get_asset_tag();
    r[constants::Drive::MEDIA_TYPE] = drive.get_type();
    r[constants::Drive::CAPABLE_SPEED] = drive.get_capable_speed_gbps();
    r[constants::Drive::PROTOCOL] = drive.get_interface();
    r[constants::Drive::NEGOTIATED_SPEED] = drive.get_negotiated_speed_gbps();

    r[Common::SKU] = drive.get_sku();
    r[constants::Drive::STATUS_INDICATOR] = drive.get_status_indicator();
    r[constants::Drive::REVISION] = drive.get_revision();
    r[constants::Drive::FAILURE_PREDICTED] = drive.get_failure_predicted();
    r[constants::Drive::HOTSPARE_TYPE] = drive.get_hotspare_type();
    r[constants::Drive::ENCRYPTION_ABILITY] = drive.get_encryption_ability();
    r[constants::Drive::ENCRYPTION_STATUS] = drive.get_encryption_status();
    r[constants::Drive::ROTATION_SPEED_RPM] = drive.get_rpm();
    r[constants::Drive::BLOCK_SIZE_BYTES] = drive.get_block_size_bytes();
    r[constants::Drive::PREDICTED_MEDIA_LIFE_LEFT] = drive.get_predicted_media_life_left();

    for (const auto& location : drive.get_locations()) {
        json::Json l = json::Json();
        l[Common::ODATA_TYPE] = "#Resource.v1_1_0.Location";
        l[constants::Drive::INFO] = location.get_info();
        l[constants::Drive::INFO_FORMAT] = location.get_info_format();
        r[Common::LOCATION].push_back(std::move(l));
    }

    for (const auto& identifier : drive.get_identifiers()) {
        json::Json id = json::Json();
        id[Common::ODATA_TYPE] = "#Resource.v1_1_0.Identifier";
        id[Common::DURABLE_NAME] = identifier.get_durable_name();
        id[Common::DURABLE_NAME_FORMAT] = identifier.get_durable_name_format();
        r[Common::IDENTIFIERS].push_back(std::move(id));
    }

    if (drive.get_capacity_gb().has_value()) {
        r[constants::Drive::CAPACITY_BYTES] = std::int64_t(endpoint::utils::gb_to_b(drive.get_capacity_gb().value()));
    }

    r[Common::LINKS][Common::CHASSIS][Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL)
                                                              .append(constants::Common::CHASSIS)
                                                              .append(req.params[PathParam::CHASSIS_ID])
                                                              .build();

    endpoint::status_to_json(drive, r);
    r[Common::STATUS][Common::HEALTH_ROLLUP] = drive.get_status().get_health();

    set_response(res, r);
}
