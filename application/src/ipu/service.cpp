/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "psme/ipu/service.hpp"
#include "psme/ipu/acc_boot_override_handler.hpp"
#include "psme/ipu/imc_reset_handler.hpp"
#include "psme/ipu/loader.hpp"
#include "psme/ipu/virtual_media_eject_handler.hpp"

using std::chrono::steady_clock;
using namespace psme::ipu;
using namespace agent_framework::model;
using namespace agent_framework::model::enums;

Service::Service() {
    Loader loader;
    loader.load();

    std::lock_guard lock(m_acc_boot_override_mutex);
    AccBootOverrideHandler handler;
    handler.read_initial_state();

    start();
}

Service::~Service() {
    log_info("ipu", "Stopping IPU service update loop...");
    m_break_loop_condition.notify_one();
    stop();
}

void Service::trigger_acc_reset(const ResetType& reset_type) {
    m_cpchnl_cmd_handler.trigger_acc_reset(reset_type);
}

void Service::trigger_imc_reset(const ResetType& reset_type) {
    ImcResetHandler handler;
    handler.run(reset_type);
}

void Service::trigger_ipu_update(const std::string& img,
                                 const OptionalField<std::string>& username,
                                 const OptionalField<std::string>& password,
                                 std::string& task_uuid) {
    m_simple_update_handler.try_lock();
    m_simple_update_handler.update_info(img, username, password);
    m_simple_update_handler.invoke_update(task_uuid);
}

void Service::set_acc_boot_override(OptionalField<BootOverride> override,
                                    OptionalField<BootOverrideTarget> target) {

    std::lock_guard lock(m_acc_boot_override_mutex);
    AccBootOverrideHandler handler;
    handler.set(std::move(override), std::move(target));
}

void Service::insert_virtual_media(const std::string& img, const TransferMethod& transfer_method,
                                   const OptionalField<std::string>& username,
                                   const OptionalField<std::string>& password,
                                   std::string& uuid) {
    m_virtual_media_insert_handler.try_lock();
    m_virtual_media_insert_handler.update_info(img, transfer_method, username, password);
    m_virtual_media_insert_handler.run(uuid);
}

void Service::eject_virtual_media() {
    VirtualMediaEjectHandler handler;
    handler.remove_image();
    handler.update_virtual_media();
}

void Service::execute() {
    std::unique_lock loop_lock(m_loop_mutex);
    while (is_running()) {
        log_info("ipu", "Running IPU service update loop");

        auto start_point = steady_clock::now();

        try {
            std::lock_guard lock(m_acc_boot_override_mutex);
            AccBootOverrideHandler handler;
            handler.check_for_updates();
        }
        catch (const std::runtime_error& e) {
            log_error("ipu",
                      " Exception in IPU service update loop: " << e.what());
        }
        catch (...) {
            log_error("ipu",
                      " Unknown exception in IPU service update loop.");
        }

        log_info("ipu", "Done running IPU service update loop");

        auto sleep_period = m_interval - (steady_clock::now() - start_point);

        m_break_loop_condition.wait_for(loop_lock, sleep_period);
    }
}
