/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "agent-framework/action/task_creator.hpp"
#include "agent-framework/action/task_runner.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "psme/rest/endpoints/task_service/monitor_content_builder.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/server/error/server_exception.hpp"

#include "ipu/curl_handler.hpp"
#include "ipu/ipu_constants.hpp"
#include "ipu/ipu_update_handler.hpp"
#include "ipu/simple_update_handler.hpp"

using namespace psme::rest;
using namespace psme::ipu::constants;
using namespace psme::ipu;
using namespace psme::rest::error;

namespace psme {
namespace ipu {

void SimpleUpdateHandler::download_package() {
    log_notice("ipu", "Starting download from " << m_img << " to " << DESTINATION_PLDM_FILEPATH);
    std::filesystem::space_info si = std::filesystem::space(PLDM_FILE_FOLDER);
    if (si.available < MAX_PLDM_IMAGE_SIZE) {
        throw std::runtime_error("There is not enough space left on " + std::string(PLDM_FILE_FOLDER));
    }
    CurlHandler()
        .set_url(m_img)
        .set_credentials(m_username, m_password)
        .set_file_name(DESTINATION_PLDM_FILEPATH)
        .set_max_file_size(MAX_PLDM_IMAGE_SIZE)
        .set_progress_report()
        .run_request();
}

void SimpleUpdateHandler::update_ipu() {
    m_reset_type = IpuUpdateHandler().run_update(DESTINATION_PLDM_FILEPATH);
}

void SimpleUpdateHandler::invoke_update(std::string& uuid) {
    agent_framework::action::TaskCreator task_creator{};
    task_creator.prepare_task();
    task_creator.add_subtask(std::bind(&SimpleUpdateHandler::download_package, this));
    task_creator.add_subtask(std::bind(&SimpleUpdateHandler::update_ipu, this));
    task_creator.add_subtask(std::bind(&SimpleUpdateHandler::remove_package, this));

    agent_framework::model::Task task_resource = task_creator.get_task_resource();
    auto task_uuid = task_resource.get_uuid();
    auto& task_manager = agent_framework::module::get_manager<agent_framework::model::Task>();
    auto tasks_count = task_manager.get_entry_count();
    task_resource.set_id(static_cast<std::uint64_t>(tasks_count + 1));
    task_manager.add_entry(task_resource);

    task_creator.add_exception_callback(std::bind(&SimpleUpdateHandler::exception_handler, this, task_uuid, std::placeholders::_1));
    task_creator.add_completion_callback(std::bind(&SimpleUpdateHandler::completion_handler, this, task_uuid));

    task_creator.set_promised_response([]() { return json::Json{}; });
    task_creator.set_promised_error_thrower([](const agent_framework::exceptions::GamiException& exception) {
        return agent_framework::exceptions::InvalidValue("Simple Update failed: " + exception.get_message());
    });

    auto update_task = task_creator.get_task();

    agent_framework::action::TaskRunner::get_instance().run(update_task);

    uuid = task_resource.get_uuid();
}

void SimpleUpdateHandler::remove_package() {
    bool exists = std::filesystem::exists(DESTINATION_PLDM_FILEPATH);
    if (!exists) {
        return;
    }
    bool rem_res = std::filesystem::remove(DESTINATION_PLDM_FILEPATH);
    if (rem_res) {
        log_info("ipu", "The pldm image has been successfully removed.");
    } else {
        log_error("ipu", "The pldm image removal failed.");
    }
}

void SimpleUpdateHandler::update_info(const std::string& img,
                                      const OptionalField<std::string>& username,
                                      const OptionalField<std::string>& password) {
    m_img = img;
    m_username = username;
    m_password = password;
}

void SimpleUpdateHandler::completion_handler(const std::string& task_uuid) {
    m_lock.clear();
    log_info("ipu", "The " + m_reset_type + " reset is required to apply the update.");
    auto task = agent_framework::module::get_manager<agent_framework::model::Task>()
                    .get_entry_reference(task_uuid);
    agent_framework::model::Task::Messages messages{
        agent_framework::model::attribute::Message{"Base.1.18.Success", "Successfully Completed Simple Update",
                                                   agent_framework::model::enums::Health::OK,
                                                   "None",
                                                   agent_framework::model::attribute::Message::RelatedProperties{},
                                                   agent_framework::model::attribute::Message::MessageArgs{}}};

    messages.add_entry(get_message());
    task->set_messages(messages);
    remove_package();
}

void SimpleUpdateHandler::exception_handler(const std::string& task_uuid, const agent_framework::exceptions::GamiException& ex) {
    m_lock.clear();
    auto task = agent_framework::module::get_manager<agent_framework::model::Task>()
                    .get_entry_reference(task_uuid);
    agent_framework::model::Task::Messages messages{
        agent_framework::model::attribute::Message{"Base.1.18.GeneralError",
                                                   "Simple Update failed: " + ex.get_message(),
                                                   agent_framework::model::enums::Health::OK,
                                                   "None",
                                                   agent_framework::model::attribute::Message::RelatedProperties{},
                                                   agent_framework::model::attribute::Message::MessageArgs{}}};
    task->set_messages(messages);
    log_info("ipu", "Update procedure failed.");
    remove_package();
}

void SimpleUpdateHandler::try_lock() {
    if (m_lock.test_and_set()) {
        throw ServerException(ErrorFactory::create_resource_in_use_error("Unable to execute several update tasks simultaneously."));
    }
}

agent_framework::model::attribute::Message SimpleUpdateHandler::get_message() {
    agent_framework::model::attribute::Message msg{};

    msg.set_message_id("Base.1.8.ResetRequired");
    msg.set_severity(agent_framework::model::enums::Health::Warning);
    msg.set_related_properties(agent_framework::model::attribute::Message::RelatedProperties{});
    msg.set_resolution("Perform the required reset action on the specified component.");

    std::string msg_template("In order to apply changes, recover from errors, or complete the operation, ");

    if ((m_reset_type == "IMCR") || (m_reset_type == "ACCR")) {
        msg.set_content(msg_template +
                        "a component reset is required with the Reset action URI "
                        "'/redfish/v1/Managers/1/Actions/Manager.Reset' and ResetType 'ForceRestart'.");
        msg.set_message_args(agent_framework::model::attribute::Message::MessageArgs{"/redfish/v1/Managers/1/Actions/Manager.Reset", "ForceRestart"});
    } else if (m_reset_type == "PERST") {
        msg.set_content(msg_template + "a host reset is required.");
    } else if (m_reset_type == "POR") {
        msg.set_content(msg_template + "a host power cycle reset is required.");
    }

    m_reset_type = "";

    return msg;
}

} // namespace ipu
} // namespace psme
