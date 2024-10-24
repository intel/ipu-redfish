/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/enum/common.hpp"

namespace psme {
namespace ipu {

/*!
 * @brief IMC Reset Handler declaration
 */
class ImcResetHandler {
public:
    ImcResetHandler() = default;
    ~ImcResetHandler() = default;
    void run(const agent_framework::model::enums::ResetType& reset_type);
private:
    void imc_reset(const agent_framework::model::enums::ResetType& reset_type);
};

} // namespace ipu
} // namespace psme
