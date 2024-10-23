/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "psme/rest/endpoints/system/virtual_media_insert.hpp"
#include "agent-framework/module/common_components.hpp"
#include "context.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/task_service/monitor_content_builder.hpp"
#include "psme/rest/endpoints/task_service/task_service_utils.hpp"
#include "psme/rest/server/multiplexer.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/virtual_media_insert.hpp"

using namespace psme::rest;
using namespace psme::rest::validators;
using namespace agent_framework::model;

namespace psme {
namespace rest {
namespace endpoint {

VirtualMediaInsert::VirtualMediaInsert(const std::string& path) : EndpointBase(path) {}

VirtualMediaInsert::~VirtualMediaInsert() {}

void VirtualMediaInsert::post(const server::Request& request, server::Response& response) {
    model::find<agent_framework::model::System, agent_framework::model::VirtualMedia>(request.params).get();

    const auto& json = JsonValidator::validate_request_body<schema::VirtualMediaInsertPostSchema>(request);
    const auto& img = json[constants::VirtualMediaInsert::IMAGE];

    // TODO: add image validation

    enums::TransferMethod transfer_method_enum = enums::TransferMethod::from_string(json[constants::VirtualMediaInsert::TRANSFER_METHOD]);

    OptionalField<std::string> username{};
    OptionalField<std::string> password{};

    if (json.contains(constants::VirtualMediaInsert::USER_NAME)) {
        username = json[constants::VirtualMediaInsert::USER_NAME];
    }

    if (json.contains(constants::VirtualMediaInsert::PASSWORD)) {
        password = json[constants::VirtualMediaInsert::PASSWORD];
    }

    std::string task_uuid{};

    Context::get_instance()->service->insert_virtual_media(img, transfer_method_enum, username, password, task_uuid);

    auto response_renderer = [](json::Json /*in_json*/) -> server::Response {
        Response r{};
        r.set_status(server::status_2XX::NO_CONTENT);
        return r;
    };

    MonitorContentBuilder::get_instance()->add_builder(task_uuid, response_renderer);

    std::string task_monitor_url = utils::get_task_monitor_url(task_uuid);

    psme::rest::endpoint::utils::set_location_header(request, response, task_monitor_url);
    response.set_body(psme::rest::endpoint::task_service_utils::call_task_get(task_uuid).get_body());
    response.set_status(server::status_2XX::ACCEPTED);
}

} // namespace endpoint
} // namespace rest
} // namespace psme
