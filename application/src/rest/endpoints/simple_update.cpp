/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2017-2019 Intel Corporation
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

#include "psme/rest/endpoints/simple_update.hpp"
#include "agent-framework/action/task_creator.hpp"
#include "agent-framework/action/task_runner.hpp"
#include "context.hpp"
#include "psme/rest/endpoints/task_service/monitor_content_builder.hpp"
#include "psme/rest/endpoints/task_service/task_service_utils.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/server/multiplexer.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/simple_update.hpp"

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::validators;

endpoint::SimpleUpdate::SimpleUpdate(const std::string& path) : EndpointBase(path) {}

endpoint::SimpleUpdate::~SimpleUpdate() {}

void endpoint::SimpleUpdate::post(const server::Request& request, server::Response& response) {
    const auto& json = JsonValidator::validate_request_body<schema::SimpleUpdatePostSchema>(request);
    const auto& image_uri = json.value(constants::UpdateService::IMAGE_URI, std::string{});

    OptionalField<std::string> username{};
    OptionalField<std::string> password{};

    if (json.contains(constants::UpdateService::USER_NAME)) {
        username = json[constants::UpdateService::USER_NAME];
    }

    if (json.contains(constants::UpdateService::PASSWORD)) {
        password = json[constants::UpdateService::PASSWORD];
    }

    std::string task_uuid;
    Context::get_instance()->service->trigger_ipu_update(image_uri, username, password, task_uuid);

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
