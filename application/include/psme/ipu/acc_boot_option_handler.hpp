/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "acc_boot_option_getter.hpp"
#include "acc_boot_option_setter.hpp"
#include "base_handler.hpp"

namespace psme {
namespace ipu {

/*!
 * @brief ACC Boot Option Command Handler declaration
 */
class AccBootOptionHandler : public BaseHandler {
public:
    AccBootOptionHandler() = default;
    ~AccBootOptionHandler() = default;
    void init() override;
    void set(const json::Json& redfish_config) override;
    void get() override;
private:
    AccBootOptionSetter m_setter{};
    AccBootOptionGetter m_getter{};
};

} // namespace ipu
} // namespace psme
