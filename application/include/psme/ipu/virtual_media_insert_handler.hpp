/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/utils/optional_field.hpp"
#include "curl/curl.h"
#include <string>

namespace psme {
namespace ipu {

/*!
 * @brief Virtual Media Insert Handler declaration
 */
class VirtualMediaInsertHandler {
public:
    VirtualMediaInsertHandler() = default;
    ~VirtualMediaInsertHandler() = default;
    void run(std::string& uuid);
    void update(const std::string& img, const agent_framework::model::enums::TransferMethod& transfer_method,
                const OptionalField<std::string>& username,
                const OptionalField<std::string>& password);
private:
    void eject_previous_media();
    void download_image();
    void create_symlink();
    void update_virtual_media();
    void add_completion_message(const std::string& task_uuid);
    void add_exception_message(const std::string& task_uuid, const agent_framework::exceptions::GamiException& ex);
private:
    std::string m_img{};
    agent_framework::model::enums::TransferMethod m_transfer_method{agent_framework::model::enums::TransferMethod::Upload};
    OptionalField<std::string> m_username{};
    OptionalField<std::string> m_password{};
};

} // namespace ipu
} // namespace psme
