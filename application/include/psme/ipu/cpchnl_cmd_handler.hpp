/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/enum/common.hpp"

namespace dcqlxx {
class Dcqlxx;
}

namespace psme {
namespace ipu {

/*!
 * @brief CPChannel Command Handler declaration
 */
class CpchnlCmdHandler {
public:
    CpchnlCmdHandler();
    ~CpchnlCmdHandler() = default;
    void trigger_acc_reset(agent_framework::model::enums::ResetType);
private:
#if !defined __x86_64__
    dcqlxx::Dcqlxx& m_dcqlxx;
#endif
};

} // namespace ipu
} // namespace psme
