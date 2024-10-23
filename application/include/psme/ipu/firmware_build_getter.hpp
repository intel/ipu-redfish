/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include <string>

namespace psme {
namespace ipu {

/*!
 * @brief FirmwareBuildGetter declaration
 */
class FirmwareBuildGetter {
public:
    FirmwareBuildGetter();
    const std::string value() const;
private:
    std::string m_firmware_build{"Unknown"};
};

} // namespace ipu
} // namespace psme
