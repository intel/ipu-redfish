/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/utils/optional_field.hpp"
#include "psme/ipu/base_service.hpp"
#include "psme/ipu/simple_update_handler.hpp"
#include "psme/ipu/virtual_media_insert_handler.hpp"

namespace psme {
namespace ipu {

/*!
 * @brief IPU Service declaration
 */
class Service : public BaseService {
public:
    /*! @brief Constructor */
    Service();

    /*! @brief Destructor */
    ~Service();

    void trigger_acc_reset(const agent_framework::model::enums::ResetType& reset_type) override;

    void trigger_imc_reset(const agent_framework::model::enums::ResetType& reset_type) override;

    void trigger_ipu_update(const std::string& img,
                            const OptionalField<std::string>& username,
                            const OptionalField<std::string>& password,
                            std::string& task_uuid) override;

    void set_acc_boot_options(const json::Json& redfish_request) override;

    void insert_virtual_media(const std::string& img, const agent_framework::model::enums::TransferMethod& transfer_method,
                              const OptionalField<std::string>& username,
                              const OptionalField<std::string>& password,
                              std::string& task_uuid) override;

    void eject_virtual_media() override;
private:
    SimpleUpdateHandler m_simple_update_handler{};
    VirtualMediaInsertHandler m_virtual_media_insert_handler{};
};

} // namespace ipu
} // namespace psme
