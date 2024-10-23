/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "psme/rest/endpoints/system/virtual_media_eject.hpp"
#include "context.hpp"
#include "psme/rest/validators/json_validator.hpp"

namespace psme {
namespace rest {
namespace endpoint {

VirtualMediaEject::VirtualMediaEject(const std::string& path) : EndpointBase(path) {}

VirtualMediaEject::~VirtualMediaEject() {}

void VirtualMediaEject::post(const server::Request& request, server::Response& response) {
    psme::rest::validators::JsonValidator::validate_empty_request(request);

    model::find<agent_framework::model::System, agent_framework::model::VirtualMedia>(request.params).get();

    Context::get_instance()->service->eject_virtual_media();

    response.set_status(server::status_2XX::NO_CONTENT);
}

} // namespace endpoint
} // namespace rest
} // namespace psme
