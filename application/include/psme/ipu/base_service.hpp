/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/enum/compute.hpp"
#include "agent-framework/module/utils/optional_field.hpp"

namespace psme {
namespace ipu {

/*!
 * @brief Base IPU Service declaration
 */
class BaseService {
public:
    virtual void trigger_acc_reset(const agent_framework::model::enums::ResetType& reset_type) = 0;

    virtual void trigger_imc_reset(const agent_framework::model::enums::ResetType& reset_type) = 0;

    virtual void trigger_ipu_update(const std::string& img,
                                    const OptionalField<std::string>& username,
                                    const OptionalField<std::string>& password,
                                    std::string& task_uuid) = 0;

    virtual void set_acc_boot_override(OptionalField<agent_framework::model::enums::BootOverride>,
                                       OptionalField<agent_framework::model::enums::BootOverrideTarget>) = 0;

    virtual void insert_virtual_media(const std::string& img, const agent_framework::model::enums::TransferMethod& transfer_method,
                                      const OptionalField<std::string>& username,
                                      const OptionalField<std::string>& password,
                                      std::string& task_uuid) = 0;

    virtual void eject_virtual_media() = 0;
};

} // namespace ipu
} // namespace psme
