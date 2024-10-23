/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/cpchnl_cmd_handler.hpp"

using namespace agent_framework::model;

namespace psme {
namespace ipu {

CpchnlCmdHandler::CpchnlCmdHandler() {
}

void CpchnlCmdHandler::trigger_acc_reset(enums::ResetType reset_type) {
    log_debug("ipu", "DCQL stub - trigger_acc_reset " << reset_type);
}

} // namespace ipu
} // namespace psme
