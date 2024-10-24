/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once
#include "agent-framework/validators/procedure_validator.hpp"

namespace psme {
namespace rest {
namespace validators {
namespace schema {

/*! @brief Schema for validating POST requests for virtual media insert. */
class VirtualMediaInsertPostSchema {
public:
    static const jsonrpc::ProcedureValidator& get_procedure();
};

} // namespace schema
} // namespace validators
} // namespace rest
} // namespace psme
