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

#include "psme/rest/endpoints/system/system_reset.hpp"
#include "context.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/reset.hpp"

using namespace agent_framework::model;
using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::validators;

endpoint::SystemReset::SystemReset(const std::string& path) : EndpointBase(path) {}

endpoint::SystemReset::~SystemReset() {}

void endpoint::SystemReset::post(const server::Request& request, server::Response& response) {
    psme::rest::model::find<agent_framework::model::System>(request.params).get();
    const auto& json = JsonValidator::validate_request_body<schema::ResetPostSchema>(request);
    const auto& reset_type = json[constants::Common::RESET_TYPE];
    const auto reset_type_enum = enums::ResetType::from_string(reset_type);

    if (!JsonValidator::validate_allowable_values({enums::ResetType::GracefulShutdown, enums::ResetType::GracefulRestart},
                                                  reset_type_enum)) {
        throw error::ServerException(
            error::ErrorFactory::create_error_for_not_allowable_value(
                "Requested reset target is not supported.",
                constants::Common::RESET_TYPE, reset_type,
                endpoint::PathBuilder(constants::Common::ACTIONS)
                    .append(constants::System::HASH_COMPUTER_SYSTEM_RESET)
                    .append(Common::ALLOWABLE_RESET_TYPES)
                    .build()));
    }

    Context::get_instance()->service->trigger_acc_reset(reset_type_enum);

    response.set_status(server::status_2XX::NO_CONTENT);
}
