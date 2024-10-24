/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/virtual_media_insert_handler.hpp"
#include "agent-framework/action/task_creator.hpp"
#include "agent-framework/action/task_runner.hpp"
#include "agent-framework/database/database_entity.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "agent-framework/module/model/system.hpp"
#include "agent-framework/module/model/virtual_media.hpp"
#include "ipu/curl_handler.hpp"
#include "ipu/ipu_constants.hpp"
#include <filesystem>
#include <system_error>

using namespace psme::ipu::constants;
using namespace agent_framework::database;

namespace psme {
namespace ipu {

void VirtualMediaInsertHandler::run(std::string& uuid) {
    agent_framework::action::TaskCreator task_creator{};
    task_creator.prepare_task();
    task_creator.add_subtask(std::bind(&VirtualMediaInsertHandler::eject_previous_media, this));
    task_creator.add_subtask(std::bind(&VirtualMediaInsertHandler::download_image, this));
    task_creator.add_subtask(std::bind(&VirtualMediaInsertHandler::create_symlink, this));
    task_creator.add_subtask(std::bind(&VirtualMediaInsertHandler::update_virtual_media, this));

    // TODO: The following lines should be moved to TaskCreator
    agent_framework::model::Task task_resource = task_creator.get_task_resource();
    auto task_uuid = task_resource.get_uuid();
    auto& task_manager = agent_framework::module::get_manager<agent_framework::model::Task>();
    auto tasks_count = task_manager.get_entry_count();
    task_resource.set_id(static_cast<uint64_t>(tasks_count + 1));
    task_manager.add_entry(task_resource);

    task_creator.add_exception_callback(std::bind(&VirtualMediaInsertHandler::add_exception_message, this, task_uuid, std::placeholders::_1));
    task_creator.add_completion_callback(std::bind(&VirtualMediaInsertHandler::add_completion_message, this, task_uuid));
    task_creator.set_promised_response([]() { return json::Json{}; });
    task_creator.set_promised_error_thrower([](const agent_framework::exceptions::GamiException& exception) {
        return agent_framework::exceptions::InvalidValue("Simple Update failed: " + exception.get_message());
    });

    auto insert_task = task_creator.get_task();

    agent_framework::action::TaskRunner::get_instance().run(insert_task);

    uuid = task_resource.get_uuid();
}

void VirtualMediaInsertHandler::eject_previous_media() {
    auto& virtual_media_manager = agent_framework::module::get_manager<agent_framework::model::VirtualMedia>();
    auto uuids = virtual_media_manager.get_keys();

    if (uuids.empty()) {
        return;
    }

    auto virtual_media = virtual_media_manager.get_entry_reference(uuids[0]);
    if (!virtual_media->get_inserted()) {
        return;
    }

    log_info("ipu", "Removing the previously inserted image.");

    virtual_media->set_image_name({});
    virtual_media->set_inserted(false);

    DatabaseEntity<VIRTUAL_MEDIA_ENTITY_NAME> entity(uuids[0]);
    entity.del(psme::ipu::constants::DOWNLOADED_IMAGE_NAME);
    std::filesystem::remove(IMAGE_SYMLINK);
    std::filesystem::remove(IMAGE_PATH);
}

void VirtualMediaInsertHandler::download_image() {
    std::filesystem::space_info si = std::filesystem::space(IMAGE_FOLDER);
    if (si.available < MAX_IMAGE_SIZE) {
        throw std::runtime_error("There is not enough space left on " + std::string(IMAGE_FOLDER));
    }

    log_info("ipu", "Starting virtual media image download.");

    CurlHandler()
        .set_url(m_img)
        .set_credentials(m_username, m_password)
        .set_file_name(IMAGE_PATH)
        .set_max_file_size(MAX_IMAGE_SIZE)
        .set_progress_report()
        .run_request();
}

void VirtualMediaInsertHandler::create_symlink() {
    using namespace agent_framework::model::enums;

    auto& system_manager = agent_framework::module::get_manager<agent_framework::model::System>();
    auto system_uuids = system_manager.get_keys();

    if (system_uuids.empty()) {
        return;
    }

    auto acc = system_manager.get_entry(system_uuids[0]);

    if (acc.get_boot_override() == BootOverride::Disabled) {
        return;
    }
    if (acc.get_boot_override_target() != BootOverrideTarget::Cd) {
        return;
    }

    bool directory_exists = std::filesystem::exists(IMAGE_SYMLINK_DIRECTORY);
    if (!directory_exists) {
        log_info("ipu", "Creating symlink to virtual media image.");
        std::error_code ec{};
        std::filesystem::create_directory(IMAGE_SYMLINK_DIRECTORY, ec);
        if (ec) {
            throw std::runtime_error(std::string("Could not create directory for: ") + IMAGE_SYMLINK + ": " + ec.message());
        }
        log_info("ipu", IMAGE_SYMLINK_DIRECTORY << " has been successfully created.");
    }

    std::error_code ec{};
    std::filesystem::create_symlink(IMAGE_PATH, IMAGE_SYMLINK, ec);
    if (ec) {
        throw std::runtime_error("Could not create symlink to virtual media image: " + ec.message());
    }
    log_info("ipu", "The virtual media image symlink has been successfully created.");
}

void VirtualMediaInsertHandler::update_virtual_media() {
    auto& virtual_media_manager = agent_framework::module::get_manager<agent_framework::model::VirtualMedia>();
    auto uuids = virtual_media_manager.get_keys();

    if (uuids.empty()) {
        return;
    }

    auto virtual_media = virtual_media_manager.get_entry_reference(uuids[0]);

    std::string img_filename = std::filesystem::path(m_img).filename();

    log_info("ipu", "Updating virtual media state.");
    virtual_media->set_image_name(img_filename);
    virtual_media->set_inserted(true);

    DatabaseEntity<VIRTUAL_MEDIA_ENTITY_NAME> entity(virtual_media->get_uuid());
    entity.put(DOWNLOADED_IMAGE_NAME, img_filename);
}

void VirtualMediaInsertHandler::update(const std::string& img, const agent_framework::model::enums::TransferMethod& transfer_method,
                                       const OptionalField<std::string>& username,
                                       const OptionalField<std::string>& password) {
    m_img = img;
    m_transfer_method = transfer_method;
    m_username = username;
    m_password = password;
}

void VirtualMediaInsertHandler::add_completion_message(const std::string& task_uuid) {
    auto task = agent_framework::module::get_manager<agent_framework::model::Task>().get_entry_reference(task_uuid);
    agent_framework::model::Task::Messages messages{agent_framework::model::attribute::Message{
        "Base.1.18.Success",
        "Virtual Media inserted successfully.",
        agent_framework::model::enums::Health::OK,
        "None",
        agent_framework::model::attribute::Message::RelatedProperties{},
        agent_framework::model::attribute::Message::MessageArgs{}}};
    task->set_messages(messages);
    log_info("ipu", "Virtual Media inserted successfully.");
}

void VirtualMediaInsertHandler::add_exception_message(const std::string& task_uuid, const agent_framework::exceptions::GamiException& ex) {
    auto task = agent_framework::module::get_manager<agent_framework::model::Task>().get_entry_reference(task_uuid);
    agent_framework::model::Task::Messages messages{agent_framework::model::attribute::Message{
        "Base.1.18.GeneralError",
        "Virtual Media insert failed: " + ex.get_message(),
        agent_framework::model::enums::Health::OK,
        "None",
        agent_framework::model::attribute::Message::RelatedProperties{},
        agent_framework::model::attribute::Message::MessageArgs{}}};
    task->set_messages(messages);
    log_info("ipu", "Virtual Media insert failed.");
}

} // namespace ipu
} // namespace psme
