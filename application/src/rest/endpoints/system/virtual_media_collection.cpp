/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "psme/rest/endpoints/system/virtual_media_collection.hpp"
#include "agent-framework/module/common_components.hpp"
#include "psme/rest/constants/constants.hpp"

using namespace psme::rest;
using namespace agent_framework::model;
using namespace agent_framework::module;

namespace {
json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);
    r[constants::Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#VirtualMediaCollection.VirtualMediaCollection";
    r[constants::Common::ODATA_ID] = json::Json::value_t::null;
    r[constants::Common::ODATA_TYPE] = "#VirtualMediaCollection.VirtualMediaCollection";
    r[constants::Common::NAME] = "Virtual Media Collection";
    r[constants::Common::DESCRIPTION] = "Collection of Virtual Media for this Manager";
    r[constants::Collection::ODATA_COUNT] = json::Json::value_t::null;
    r[constants::Collection::MEMBERS] = json::Json::value_t::array;
    return r;
}
} // namespace

namespace psme {
namespace rest {
namespace endpoint {

VirtualMediaCollection::VirtualMediaCollection(const std::string& path) : EndpointBase(path) {}

VirtualMediaCollection::~VirtualMediaCollection() {}

void VirtualMediaCollection::get(const server::Request& request, server::Response& response) {
    auto r = make_prototype();
    r[constants::Common::ODATA_ID] = PathBuilder(request).build();

    auto manager_uuid = model::find<agent_framework::model::System>(request.params).get_uuid();
    auto media_ids = get_manager<agent_framework::model::VirtualMedia>().get_ids(manager_uuid);

    r[constants::Collection::ODATA_COUNT] = std::uint32_t(media_ids.size());

    for (const auto& media_id : media_ids) {
        json::Json link(json::Json::value_t::object);
        link[constants::Common::ODATA_ID] = PathBuilder(request).append(media_id).build();
        r[constants::Collection::MEMBERS].push_back(std::move(link));
    }

    set_response(response, r);
}

} // namespace endpoint
} // namespace rest
} // namespace psme
