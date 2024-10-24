/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief Definition of Network Device Function endpoint
 *
 * @copyright Copyright (c) 2017-2019 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file network_device_function.cpp
 */

#include "psme/rest/endpoints/system/network_device_function.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/system/network_device_functions_collection.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/utils/status_helpers.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace agent_framework::module;

namespace {
json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#NetworkDeviceFunction.NetworkDeviceFunction";
    r[Common::ODATA_ID] = json::Json::value_t::null;
    r[Common::ODATA_TYPE] = "#NetworkDeviceFunction.v1_0_0.NetworkDeviceFunction";
    r[Common::ID] = json::Json::value_t::null;
    r[Common::NAME] = "Network Device Function";
    r[Common::DESCRIPTION] = "Network Device Function";
    r[Common::STATUS][Common::STATE] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH] = json::Json::value_t::null;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Json::value_t::null;
    r[NetworkDeviceFunction::DEVICE_ENABLED] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ETHERNET][Common::MAC_ADDRESS] = json::Json::value_t::null;
    r[Common::LINKS] = json::Json::value_t::object;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::IP_ADDRESS_TYPE] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::INITIATOR_IP_ADDRESS] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::INITIATOR_NAME] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::INITIATOR_DEFAULT_GATEWAY] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::INITIATOR_NETMASK] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::TARGET_INFO_VIA_DHCP] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::PRIMARY_TARGET_NAME] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::PRIMARY_TARGET_IP_ADDRESS] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::PRIMARY_TARGET_TCP_PORT] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::PRIMARY_LUN] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::PRIMARY_VLAN_ENABLE] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::PRIMARY_VLAN_ID] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::PRIMARY_DNS] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::SECONDARY_TARGET_NAME] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::SECONDARY_TARGET_IP_ADDRESS] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::SECONDARY_TARGET_TCP_PORT] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::SECONDARY_LUN] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::SECONDARY_VLAN_ENABLE] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::SECONDARY_VLAN_ID] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::SECONDARY_DNS] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::IP_MASK_DNS_VIA_DHCP] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::ROUTER_ADVERTISEMENT_ENABLED] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::AUTHENTICATION_METHOD] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::CHAP_USERNAME] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::CHAP_SECRET] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::MUTUAL_CHAP_USERNAME] = json::Json::value_t::null;
    r[NetworkDeviceFunction::ISCSI_BOOT][NetworkDeviceFunction::MUTUAL_CHAP_SECRET] = json::Json::value_t::null;

    return r;
}
} // namespace

endpoint::NetworkDeviceFunction::NetworkDeviceFunction(const std::string& path) : EndpointBase(path) {}

endpoint::NetworkDeviceFunction::~NetworkDeviceFunction() {}

void endpoint::NetworkDeviceFunction::get(const server::Request& req, server::Response& res) {
    auto json = make_prototype();

    psme::rest::server::Parameters request_parameters;
    request_parameters = utils::get_network_device_request_parameters(req.params);
    request_parameters.set(PathParam::NETWORK_DEVICE_FUNCTION_ID, req.params.get(PathParam::NETWORK_DEVICE_FUNCTION_ID));

    auto function = psme::rest::model::find<agent_framework::model::System, agent_framework::model::NetworkDevice, agent_framework::model::NetworkDeviceFunction>(
                        request_parameters)
                        .get();

    json[Common::ODATA_ID] = PathBuilder(req).build();

    json[constants::Common::ID] = req.params[PathParam::NETWORK_DEVICE_FUNCTION_ID];
    endpoint::status_to_json(function, json);

    json[constants::NetworkDeviceFunction::DEVICE_ENABLED] = function.get_device_enabled();
    json[constants::NetworkDeviceFunction::ETHERNET][Common::MAC_ADDRESS] = function.get_mac_address();
    json[Common::LINKS] = json::Json::value_t::object;

    const auto& iscsi_boot = function.get_iscsi_boot();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::IP_ADDRESS_TYPE] =
        iscsi_boot.get_ip_address_type().to_string();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::INITIATOR_IP_ADDRESS] =
        iscsi_boot.get_initiator_address();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::INITIATOR_NAME] =
        iscsi_boot.get_initiator_name();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::INITIATOR_DEFAULT_GATEWAY] =
        iscsi_boot.get_initiator_default_gateway();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::INITIATOR_NETMASK] =
        iscsi_boot.get_initiator_netmask();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::TARGET_INFO_VIA_DHCP] =
        iscsi_boot.get_target_info_via_dhcp();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::PRIMARY_TARGET_NAME] =
        iscsi_boot.get_primary_target_name();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::PRIMARY_TARGET_IP_ADDRESS] =
        iscsi_boot.get_primary_target_address();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::PRIMARY_TARGET_TCP_PORT] =
        iscsi_boot.get_primary_target_port();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::PRIMARY_LUN] =
        iscsi_boot.get_primary_lun();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::PRIMARY_VLAN_ENABLE] =
        iscsi_boot.get_primary_vlan_enable();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::PRIMARY_VLAN_ID] =
        iscsi_boot.get_primary_vlan_id();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::PRIMARY_DNS] =
        iscsi_boot.get_primary_dns();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::SECONDARY_TARGET_NAME] =
        iscsi_boot.get_secondary_target_name();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::SECONDARY_TARGET_IP_ADDRESS] =
        iscsi_boot.get_secondary_target_address();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::SECONDARY_TARGET_TCP_PORT] =
        iscsi_boot.get_secondary_target_port();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::SECONDARY_LUN] =
        iscsi_boot.get_secondary_lun();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::SECONDARY_VLAN_ENABLE] =
        iscsi_boot.get_secondary_vlan_enable();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::SECONDARY_VLAN_ID] =
        iscsi_boot.get_secondary_vlan_id();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::SECONDARY_DNS] =
        iscsi_boot.get_secondary_dns();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::IP_MASK_DNS_VIA_DHCP] =
        iscsi_boot.get_ip_mask_dns_via_dhcp();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::ROUTER_ADVERTISEMENT_ENABLED] =
        iscsi_boot.get_router_advertisement_enabled();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::AUTHENTICATION_METHOD] =
        iscsi_boot.get_authentication_method().to_string();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::CHAP_USERNAME] =
        iscsi_boot.get_chap_username();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::CHAP_SECRET] =
        iscsi_boot.get_chap_secret();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::MUTUAL_CHAP_USERNAME] =
        iscsi_boot.get_mutual_chap_username();
    json[constants::NetworkDeviceFunction::ISCSI_BOOT][constants::NetworkDeviceFunction::MUTUAL_CHAP_SECRET] =
        iscsi_boot.get_mutual_chap_secret();

    set_response(res, json);
}
