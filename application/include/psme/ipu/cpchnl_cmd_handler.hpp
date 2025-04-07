/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/enum/common.hpp"

#ifdef INTEL_IPU
namespace dcqlxx {
class Dcqlxx;
}
#endif

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
#ifdef INTEL_IPU
private:
    dcqlxx::Dcqlxx& m_dcqlxx;
#endif
};

} // namespace ipu
} // namespace psme
