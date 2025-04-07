/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/ipu_update_handler.hpp"
#include "logger/logger.hpp"

namespace psme {
namespace ipu {

IpuUpdateHandler::IpuUpdateHandler() {}

std::string IpuUpdateHandler::run_update(const std::string& package_path) {
    (void)package_path;
    log_notice("ipu", "IPU Update has been triggered");
    return std::string{"POR"};
}

InventoryVersion IpuUpdateHandler::get_component_info() {
    log_notice("ipu", "IPU Update get_component info has been triggered");
    InventoryVersion versions{};
    return versions;
}

} // namespace ipu
} // namespace psme
