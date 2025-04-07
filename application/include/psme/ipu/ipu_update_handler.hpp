/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/utils/optional_field.hpp"

#include <cstdarg>
#include <cstring>
#include <memory>
#include <string>

namespace psme {
namespace ipu {

#ifdef INTEL_IPU
namespace ipu_update {
#include <ipu.h>
} // namespace ipu_update
#endif

struct InventoryVersion {
    OptionalField<std::string> board_id_version;
    OptionalField<std::string> boot_image_version;
    OptionalField<std::string> imc_version;
    OptionalField<std::string> imc_orom_version;
    OptionalField<std::string> acc_bios_version;
    OptionalField<std::string> recovery_imc_version;

    InventoryVersion() : board_id_version{},
                         boot_image_version{},
                         imc_version{},
                         imc_orom_version{},
                         acc_bios_version{},
                         recovery_imc_version{} {}
};

class IpuUpdateHandler {
public:
    IpuUpdateHandler();
    IpuUpdateHandler(const IpuUpdateHandler& handler) = delete;
    IpuUpdateHandler(IpuUpdateHandler&& handler) = delete;
    ~IpuUpdateHandler() = default;
    IpuUpdateHandler& operator=(const IpuUpdateHandler& handler) = delete;
    IpuUpdateHandler& operator=(IpuUpdateHandler&& handler) = delete;
    std::string run_update(const std::string& package_path);
    InventoryVersion get_component_info();
#ifdef INTEL_IPU
private:
    void throw_if_not_success(ipu_update::Result result);
    OptionalField<std::string> version_to_string(const ipu_update::IfutComponentVersion& version);
    std::string get_reset_type(ipu_update::Result update_code);
private:
    static void log_callback(ipu_update::LogLevel type, const char* format, const size_t format_size, ...);
    static void vlog_callback(ipu_update::LogLevel type, const char* format, const size_t /* unused */, bool /* unused */, va_list args);
private:
    std::unique_ptr<ipu_update::IfutContext, ipu_update::Result (*)(ipu_update::IfutContext*)> m_ctx;
#endif
};

} // namespace ipu
} // namespace psme
