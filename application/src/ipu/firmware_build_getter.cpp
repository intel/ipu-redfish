/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/firmware_build_getter.hpp"
#include "ipu/ipu_constants.hpp"
#include "logger/logger.hpp"
#include <fstream>

using namespace psme::ipu::constants;

namespace psme {
namespace ipu {

FirmwareBuildGetter::FirmwareBuildGetter() {
#ifdef INTEL_IPU
    std::string file_content;
    try {
        std::getline(std::ifstream(FIRMWARE_VERSION_FILEPATH), file_content, '\n');
    }
    catch (const std::exception& e) {
        log_error("ipu", "Failed to read firmware build file: " << FIRMWARE_VERSION_FILEPATH << " " << e.what());
        return;
    }
    /* Parsing of "IPU IMC MEV-HW-B1-ci-ts.release.x.x.x.x" string */
    size_t start_pos = file_content.rfind(' ');
    if (start_pos == std::string::npos) {
        log_error("ipu", "Failed to parse firmware build string: " << FIRMWARE_VERSION_FILEPATH);
        return;
    }
    m_firmware_build = file_content.substr(start_pos + 1);
#else
    m_firmware_build = "Default x86-64 system";
#endif
}

const std::string FirmwareBuildGetter::value() const {
    return m_firmware_build;
}

} // namespace ipu
} // namespace psme
