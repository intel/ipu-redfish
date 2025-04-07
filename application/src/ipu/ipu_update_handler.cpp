/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/ipu_update_handler.hpp"
#include "logger/logger.hpp"
#include "psme/rest/constants/constants.hpp"
#include "psme/rest/constants/routes.hpp"

using namespace psme::rest::constants;
using namespace psme::ipu::ipu_update;

namespace psme {
namespace ipu {

/* Without ignoring this warning, passing this to IfutCtxSetLogCallback fails */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-attribute=format"
void IpuUpdateHandler::log_callback(LogLevel type, const char* format, const size_t format_size, ...) {
    char message[4096] = {};
    va_list list;
    va_start(list, format_size);
    vsnprintf(message, sizeof(message), format, list);
    va_end(list);

    switch (type) {
    case kLogLevelBasic:
        log_info("ifut", message);
        break;
    case kLogLevelInfo:
        log_debug("ifut", message);
        break;
    case kLogLevelError:
        log_error("ifut", message);
        break;
    case kLogLevelWarning:
        log_warning("ifut", message);
        break;

    default:
        break;
    }
}

void IpuUpdateHandler::vlog_callback(LogLevel type, const char* format, const size_t /* unused */, bool /* unused */, va_list args) {
    char message[4096] = {};
    vsnprintf(message, sizeof(message), format, args);

    switch (type) {
    case kLogLevelBasic:
        log_info("ifut-dcql", message);
        break;
    case kLogLevelInfo:
        log_debug("ifut-dcql", message);
        break;
    case kLogLevelError:
        log_error("ifut-dcql", message);
        break;
    case kLogLevelWarning:
        log_warning("ifut-dcql", message);
        break;

    default:
        break;
    }
}
#pragma GCC diagnostic pop

std::string IpuUpdateHandler::get_reset_type(ipu_update::Result update_code) {
    std::string reset_type{};
    if (update_code == IFUT_SUCCESS_IMCR) {
        reset_type = "IMCR";
    } else if (update_code == IFUT_SUCCESS_ACCR) {
        reset_type = "ACCR";
    } else if (update_code == IFUT_SUCCESS_PERST) {
        reset_type = "PERST";
    } else if (update_code == IFUT_SUCCESS_POR) {
        reset_type = "POR";
    }
    return reset_type;
}

void IpuUpdateHandler::throw_if_not_success(Result result) {
    if (!IsResultSuccess(result)) {
        std::string result_description(GetResultDescription(result));
        log_error("ifut", "Operation failed " << result_description);
        throw std::runtime_error(result_description);
    }
}

IpuUpdateHandler::IpuUpdateHandler() : m_ctx(IfutAllocateIfutContext(),
                                             IfutFreeIfutContext) {
    if (!m_ctx) {
        throw std::runtime_error("Failed to allocate IfutContext");
    }
    Result res = IfutCtxSetLogCallback(m_ctx.get(), log_callback, vlog_callback);
    throw_if_not_success(res);
    IfutCtxDcqlInitialized(m_ctx.get());
}

std::string IpuUpdateHandler::run_update(const std::string& package_path) {
    log_notice("ipu", "Start IPU Update");

    Result res = IfutCtxAddPldmFilePath(m_ctx.get(),
                                        {package_path.size() + 1 /* include NULL */,
                                         const_cast<char*>(package_path.c_str())});
    throw_if_not_success(res);

    res = IfutExecuteUpdate(m_ctx.get());
    throw_if_not_success(res);

    log_info("ipu", "Successfully updated the IMC. Restart the device to complete the process.");

    return get_reset_type(res);
}

InventoryVersion IpuUpdateHandler::get_component_info() {
    IfutInventoryMetadata device_info{};
    Result res = IfutInventoryDevice(m_ctx.get(), &device_info);
    if (!IsResultSuccess(res)) {
        throw std::runtime_error("Failed to get device info");
    }

    InventoryVersion versions{};
    if (device_info.board_id.valid) {
        versions.board_id_version = device_info.board_id.id;
    }

    versions.boot_image_version = std::move(version_to_string(device_info.versions.component_version[IfutComponentId::kComponentBootBank]));
    versions.imc_version = std::move(version_to_string(device_info.versions.component_version[IfutComponentId::kComponentImcFs]));
    versions.imc_orom_version = std::move(version_to_string(device_info.versions.component_version[IfutComponentId::kComponentOrom]));
    versions.acc_bios_version = std::move(version_to_string(device_info.versions.component_version[IfutComponentId::kComponentAcc]));
    versions.recovery_imc_version = std::move(version_to_string(device_info.versions.component_version[IfutComponentId::kComponentRecovery]));

    return versions;
}

OptionalField<std::string> IpuUpdateHandler::version_to_string(const IfutComponentVersion& version) {
    if (!version.valid) {
        return {};
    }
    std::string version_string;
    version_string.reserve(16);
    version_string.append(std::to_string(static_cast<int>(version.major)));
    version_string.append(".");
    version_string.append(std::to_string(static_cast<int>(version.minor)));
    version_string.append(".");
    version_string.append(std::to_string(static_cast<int>(version.patch)));
    return version_string;
}

} // namespace ipu
} // namespace psme
