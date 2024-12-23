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

#include "psme/rest/validators/schemas/subscription_collection.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/eventing/event.hpp"
#include "psme/rest/eventing/model/subscription.hpp"
#include "psme/rest/validators/schemas/common.hpp"

using namespace psme::rest;
using namespace psme::rest::validators::schema;

const jsonrpc::ProcedureValidator& SubscriptionCollectionPostSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        jsonrpc::PARAMS_BY_NAME,
        constants::Common::NAME, VALID_JSON_STRING,
        constants::EventSubscription::DESTINATION, VALID_JSON_STRING,
        constants::EventSubscription::EVENT_TYPES, VALID_ARRAY_OF(VALID_ENUM(eventing::EventType)),
        constants::EventSubscription::CONTEXT, VALID_JSON_STRING,
        constants::EventSubscription::PROTOCOL, VALID_ENUM(psme::rest::eventing::model::SubscriptionProtocol),
        constants::EventSubscription::ORIGIN_RESOURCES, VALID_OPTIONAL(VALID_NULLABLE(VALID_ARRAY_OF(VALID_ATTRIBUTE(SimpleObjectSchema)))),
        nullptr};
    return procedure;
}
