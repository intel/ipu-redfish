/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "psme/ipu/service.hpp"
#include "psme/ipu/acc_boot_option_handler.hpp"
#include "psme/ipu/cpchnl_cmd_handler.hpp"
#include "psme/ipu/imc_reset_handler.hpp"
#include "psme/ipu/loader.hpp"
#include "psme/ipu/virtual_media_eject_handler.hpp"

using namespace psme::ipu;
using namespace agent_framework::model;

Service::Service() {
    Loader loader;
    loader.load();
}

Service::~Service() {
}

void Service::trigger_acc_reset(const enums::ResetType& reset_type) {
    static CpchnlCmdHandler cpchnl_cmd_handler;
    cpchnl_cmd_handler.trigger_acc_reset(reset_type);
}

void Service::trigger_imc_reset(const enums::ResetType& reset_type) {
    ImcResetHandler handler;
    handler.run(reset_type);
}

void Service::trigger_ipu_update(const std::string& img,
                                 const OptionalField<std::string>& username,
                                 const OptionalField<std::string>& password,
                                 std::string& task_uuid) {
    m_simple_update_handler.update_info(img, username, password);
    m_simple_update_handler.invoke_update(task_uuid);
}

void Service::set_acc_boot_options(const json::Json& redfish_request) {
    AccBootOptionHandler handler;
    handler.set(redfish_request);
    handler.get();
}

void Service::insert_virtual_media(const std::string& img, const enums::TransferMethod& transfer_method,
                                   const OptionalField<std::string>& username,
                                   const OptionalField<std::string>& password,
                                   std::string& uuid) {
    m_virtual_media_insert_handler.update(img, transfer_method, username, password);
    m_virtual_media_insert_handler.run(uuid);
}

void Service::eject_virtual_media() {
    VirtualMediaEjectHandler handler;
    handler.remove_image();
    handler.update_virtual_media();
}
