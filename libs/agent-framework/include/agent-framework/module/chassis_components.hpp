/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2015-2019 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * @file chassis_components.hpp
 * @brief Chassis Components Interface
 * */

#pragma once

#include "agent-framework/generic/singleton.hpp"
#include "agent-framework/module/managers/generic_manager.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"
#include "agent-framework/module/model/model_chassis.hpp"

/*! Psme namespace */
namespace agent_framework {
namespace module {

/*!
 * @brief Class for managing chassis components and subcomponents
 * */
class ChassisComponents : public generic::Singleton<ChassisComponents> {
public:
    using CertificateManager = GenericManager<model::AuthorizationCertificate>;

    virtual ~ChassisComponents();

    /*!
     * @brief Get certificate manager
     * @return certificate manager
     * */
    CertificateManager& certificates() {
        return m_certificate_manager;
    }
private:
    CertificateManager m_certificate_manager{};
};

} // namespace module
} // namespace agent_framework
