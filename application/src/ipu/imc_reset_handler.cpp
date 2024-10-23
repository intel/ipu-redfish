/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/imc_reset_handler.hpp"
#include "agent-framework/action/task_runner.hpp"
#include <chrono>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <thread>
#include <unistd.h>

using namespace agent_framework::model;

namespace psme {
namespace ipu {

void ImcResetHandler::run(const enums::ResetType& reset_type) {
    agent_framework::action::TaskRunner::get_instance().run(&ImcResetHandler::imc_reset, this, reset_type);
}

void ImcResetHandler::imc_reset(const enums::ResetType& reset_type) {
    log_info("ipu", "IMC power action requested: " << reset_type.to_string());
    std::this_thread::sleep_for(std::chrono::seconds(2));
    sync();
    switch (reset_type) {
    case enums::ResetType::ForceRestart:
        reboot(LINUX_REBOOT_CMD_RESTART);
        break;
    default:
        break;
    }
}

} // namespace ipu
} // namespace psme
