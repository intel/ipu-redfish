/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "psme/rest/endpoints/system/virtual_media.hpp"
#include "agent-framework/module/common_components.hpp"
#include "psme/rest/constants/constants.hpp"

using namespace psme::rest;

namespace {
json::Json make_prototype() {
    json::Json r(json::Json::value_t::object);
    r[constants::Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#VirtualMedia.VirtualMedia";
    r[constants::Common::ODATA_TYPE] = "#VirtualMedia.v1_6_4.VirtualMedia";
    r[constants::Common::NAME] = "Virtual Media Entry";
    r[constants::Common::DESCRIPTION] = "Virtual Media Entry description";
    r[constants::Common::ODATA_ID] = json::Json::value_t::null;
    r[constants::Common::ID] = json::Json::value_t::null;
    r[constants::VirtualMedia::MEDIA_TYPES] = json::Json::value_t::array;
    r[constants::VirtualMedia::IMAGE_NAME] = json::Json::value_t::null;
    r[constants::VirtualMedia::INSERTED] = json::Json::value_t::null;
    r[constants::Common::ACTIONS] = json::Json::value_t::object;
    return r;
}
} // namespace

namespace psme {
namespace rest {
namespace endpoint {

VirtualMedia::VirtualMedia(const std::string& path) : EndpointBase(path) {}

VirtualMedia::~VirtualMedia() {}

void VirtualMedia::get(const server::Request& request, server::Response& response) {
    auto r = make_prototype();
    auto media = model::find<agent_framework::model::System, agent_framework::model::VirtualMedia>(request.params).get();
    r[constants::Common::ID] = request.params[constants::PathParam::VIRTUAL_MEDIA_ID];
    r[constants::VirtualMedia::MEDIA_TYPES].push_back(media.get_media_type().to_string());
    r[constants::VirtualMedia::IMAGE_NAME] = media.get_image_name();
    r[constants::VirtualMedia::INSERTED] = media.get_inserted();
    r[constants::Common::ODATA_ID] = PathBuilder(request).build();

    json::Json insert;
    insert[constants::Common::TARGET] =
        endpoint::PathBuilder(request)
            .append(constants::Common::ACTIONS)
            .append(constants::Common::VIRTUAL_MEDIA_INSERT)
            .build();
    r[constants::Common::ACTIONS][constants::VirtualMedia::HASH_VIRTUAL_MEDIA_INSERT] = std::move(insert);

    if (media.get_inserted()) {
        json::Json eject;
        eject[constants::Common::TARGET] =
            endpoint::PathBuilder(request)
                .append(constants::Common::ACTIONS)
                .append(constants::Common::VIRTUAL_MEDIA_EJECT)
                .build();

        r[constants::Common::ACTIONS][constants::VirtualMedia::HASH_VIRTUAL_MEDIA_EJECT] = std::move(eject);
    }
    set_response(response, r);
}

} // namespace endpoint
} // namespace rest
} // namespace psme
