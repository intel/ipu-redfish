/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "psme/rest/validators/schemas/virtual_media_insert.hpp"
#include "agent-framework/module/enum/common.hpp"
#include "psme/rest/constants/constants.hpp"

using namespace psme::rest;
using namespace psme::rest::validators::schema;
using namespace agent_framework::model;

const jsonrpc::ProcedureValidator& VirtualMediaInsertPostSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        jsonrpc::PARAMS_BY_NAME,
        constants::VirtualMediaInsert::IMAGE, VALID_JSON_STRING,
        constants::VirtualMediaInsert::TRANSFER_METHOD, VALID_ENUM(enums::TransferMethod),
        constants::VirtualMediaInsert::USER_NAME, VALID_OPTIONAL(VALID_JSON_STRING),
        constants::VirtualMediaInsert::PASSWORD, VALID_OPTIONAL(VALID_JSON_STRING),
        nullptr};
    return procedure;
}
