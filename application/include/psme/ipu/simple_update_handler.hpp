/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include <atomic>
#include <string>

namespace psme {
namespace ipu {

/*!
 * @brief Simple Update Handler declaration
 */
class SimpleUpdateHandler {
public:
    SimpleUpdateHandler() = default;
    ~SimpleUpdateHandler() = default;
    void invoke_update(std::string& task_uuid);
    void update_info(const std::string& img,
                     const OptionalField<std::string>& username,
                     const OptionalField<std::string>& password);
    void try_lock();
private:
    void download_package();
    void update_ipu();
    void remove_package();
    void completion_handler(const std::string& task_uuid);
    void exception_handler(const std::string& task_uuid, const agent_framework::exceptions::GamiException& ex);
private:
    std::string m_img{};
    OptionalField<std::string> m_username{};
    OptionalField<std::string> m_password{};
    std::atomic_flag m_lock{ATOMIC_FLAG_INIT};
};

} // namespace ipu
} // namespace psme
