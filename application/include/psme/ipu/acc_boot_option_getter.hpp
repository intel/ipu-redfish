/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "json-wrapper/json-wrapper.hpp"

namespace psme {
namespace ipu {

/*!
 * @brief ACC Boot Option Command Getter declaration
 */
class AccBootOptionGetter {
public:
    AccBootOptionGetter() = default;
    ~AccBootOptionGetter() = default;
    void run();
private:
    void get_boot_state(const json::Json& file_config);
    void get_boot_type(const json::Json& file_config);
    void get_error(const json::Json& file_config);
};

} // namespace ipu
} // namespace psme
