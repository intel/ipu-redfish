/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "json-wrapper/json-wrapper.hpp"

namespace psme {
namespace ipu {

/*!
 * @brief Command Handler interface declaration
 */
class BaseHandler {
public:
    virtual void init() = 0;
    virtual void set(const json::Json& new_config) = 0;
    virtual void get() = 0;
    virtual ~BaseHandler() = default;
};

} // namespace ipu
} // namespace psme
