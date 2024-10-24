/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2022 Intel Corporation */

#pragma once

#include "generic/singleton.hpp"

#include "psme/ipu/base_service.hpp"

#include <memory>

namespace psme {

class Context final : public generic::Singleton<Context> {
public:
    Context() = default;

    void initialize();

    std::shared_ptr<ipu::BaseService> service{};
};

} // namespace psme

using Context = psme::Context;
