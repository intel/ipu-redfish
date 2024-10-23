/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "json-wrapper/json-wrapper.hpp"

namespace psme {
namespace ipu {

/*!
 * @brief ACC Boot Option Command Setter declaration
 */
class AccBootOptionSetter {
public:
    AccBootOptionSetter() = default;
    ~AccBootOptionSetter() = default;
    void run(const json::Json& redfish_config);
private:
    void make_file_content(json::Json& file_config);
    void update_file_content(const json::Json& redfish_config, json::Json& file_config);
    void update_boot_state(const json::Json& redfish_config, json::Json& file_config);
    void update_boot_type(const json::Json& redfish_config, json::Json& file_config);
    void update_error(const json::Json& redfish_config, json::Json& file_config);
    void prepare_acc_iso_dram_boot();
    void prepare_acc_iscsi_boot();
};

} // namespace ipu
} // namespace psme
