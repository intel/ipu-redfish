/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2022 Intel Corporation */

#include "context.hpp"

#include "ipu/service.hpp"

using namespace psme;

void Context::initialize() {
    service = std::make_shared<ipu::Service>();
}
