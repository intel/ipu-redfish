/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/threading/thread.hpp"
#include "psme/ipu/base_service.hpp"
#include "psme/ipu/cpchnl_cmd_handler.hpp"
#include "psme/ipu/simple_update_handler.hpp"
#include "psme/ipu/virtual_media_insert_handler.hpp"

#include <chrono>
#include <condition_variable>
#include <mutex>

namespace psme {
namespace ipu {

/*!
 * @brief IPU Service declaration
 */
class Service : public BaseService, private agent_framework::threading::Thread {
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

    /*!
     * @brief Set the boot override settings requested by the user
     * @param[in] New boot override setting, or an empty optional if none was provided
     * @param[in] New boot override target, or an empty optional if none was provided
     * */
    void set_acc_boot_override(OptionalField<agent_framework::model::enums::BootOverride>,
                               OptionalField<agent_framework::model::enums::BootOverrideTarget>) override;

    void insert_virtual_media(const std::string& img, const agent_framework::model::enums::TransferMethod& transfer_method,
                              const OptionalField<std::string>& username,
                              const OptionalField<std::string>& password,
                              std::string& task_uuid) override;

    void eject_virtual_media() override;
private:
    /*!
     * @brief This function is run in the thread owned by this class.
     * It's used to check for update of the IPU's state.
     * */
    void execute() override;

    CpchnlCmdHandler m_cpchnl_cmd_handler{};
    SimpleUpdateHandler m_simple_update_handler{};
    VirtualMediaInsertHandler m_virtual_media_insert_handler{};
    std::mutex m_acc_boot_override_mutex{};
    std::condition_variable m_break_loop_condition{};
    std::mutex m_loop_mutex{};
    const std::chrono::seconds m_interval{120};
};

} // namespace ipu
} // namespace psme
