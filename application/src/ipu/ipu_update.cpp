/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/ipu_update.hpp"
#include "logger/logger.hpp"
namespace ipu_update {
#include <ipu.h>
} // namespace ipu_update
#include <cstdarg>
#include <cstring>

namespace {
/* Without ignoring this warning, passing this to IfutCtxSetLogCallback fails */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-attribute=format"
void log_callback(ipu_update::LogLevel type, const char* format, const size_t format_size, ...) {
    char message[4096] = {};
    va_list list;
    va_start(list, format_size);
    vsnprintf(message, sizeof(message), format, list);
    va_end(list);

    switch (type) {
    case ipu_update::kLogLevelBasic:
        log_info("ifut", message);
        break;
    case ipu_update::kLogLevelInfo:
        log_debug("ifut", message);
        break;
    case ipu_update::kLogLevelError:
        log_error("ifut", message);
        break;
    case ipu_update::kLogLevelWarning:
        log_warning("ifut", message);
        break;

    default:
        break;
    }
}

void vlog_callback(ipu_update::LogLevel type, const char* format, const size_t /* unused */, bool /* unused */, va_list args) {
    char message[4096] = {};
    vsnprintf(message, sizeof(message), format, args);

    switch (type) {
    case ipu_update::kLogLevelBasic:
        log_info("ifut-dcql", message);
        break;
    case ipu_update::kLogLevelInfo:
        log_debug("ifut-dcql", message);
        break;
    case ipu_update::kLogLevelError:
        log_error("ifut-dcql", message);
        break;
    case ipu_update::kLogLevelWarning:
        log_warning("ifut-dcql", message);
        break;

    default:
        break;
    }
}
#pragma GCC diagnostic pop

static void throw_if_not_success(ipu_update::Result result) {
    if (!IsResultSuccess(result)) {
        std::string result_description(ipu_update::GetResultDescription(result));
        log_error("ifut", "Operation failed " << result_description);
        throw std::runtime_error(result_description);
    }
}

} // namespace

namespace psme {
namespace ipu {

void ipu_update(const std::string& package_path) {
    log_notice("ipu", "Start IPU Update");

    auto ctx = ipu_update::IfutAllocateIfutContext();
    if (!ctx) {
        throw std::runtime_error("Failed to allocate IfutContext");
    }

    ipu_update::Result res;

    res = ipu_update::IfutCtxSetLogCallback(ctx, log_callback, vlog_callback);
    throw_if_not_success(res);

    res = ipu_update::IfutCtxAddPldmFilePath(ctx,
                                             {package_path.size() + 1 /* include NULL */,
                                              const_cast<char*>(package_path.c_str())});
    throw_if_not_success(res);

    // TODO: remove for release
    res = ipu_update::IfutCtxSetPartialUpdate(ctx, ipu_update::kComponentRecovery, false);
    throw_if_not_success(res);

    res = ipu_update::IfutExecuteUpdate(ctx);
    throw_if_not_success(res);

    res = ipu_update::IfutFreeIfutContext(ctx);
    throw_if_not_success(res);

    log_info("ipu", "Successfully updated the IMC. Restart the device to complete the process.");
}

} // namespace ipu
} // namespace psme
