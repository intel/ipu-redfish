/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2017-2019 Intel Corporation
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
 * @file trusted_module.hpp
 * @brief Definition of Trusted Module class
 * */

#pragma once

#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/enum/compute.hpp"
#include "agent-framework/module/model/attributes/model_attributes.hpp"
#include "agent-framework/module/model/resource.hpp"

namespace agent_framework {
namespace model {

class TrustedModule : public Resource {
public:
    explicit TrustedModule(const std::string& parent_uuid = {}, enums::Component parent_type = enums::Component::None);

    ~TrustedModule();

    /*! Enable copy */
    TrustedModule(const TrustedModule&) = default;

    TrustedModule& operator=(const TrustedModule&) = default;

    TrustedModule(TrustedModule&&) = default;

    TrustedModule& operator=(TrustedModule&&) = default;

    friend auto operator<=>(const TrustedModule& lhs, const TrustedModule& rhs) = default;

    /*!
     * @brief Get component name
     * @return component name
     */
    static enums::Component get_component() {
        return TrustedModule::component;
    }

    /*!
     * @brief Sets firmware version
     * @param[in] firmware_version The firmware version of TPM.
     * */
    void set_firmware_version(const OptionalField<std::string>& firmware_version) {
        m_firmware_version = firmware_version;
    }

    /*!
     * @brief Gets firmware version.
     * @return The firmware version of TPM.
     * */
    const OptionalField<std::string>& get_firmware_version() const {
        return m_firmware_version;
    }

    /*!
     * @brief Sets interface type.
     * @param[in] interface_type TPM interface type.
     * */
    void set_interface_type(const OptionalField<enums::InterfaceType>& interface_type) {
        m_interface_type = interface_type;
    }

    /*!
     * @brief Gets interface type.
     * @return A interface type of TMP.
     * */
    const OptionalField<enums::InterfaceType>& get_interface_type() const {
        return m_interface_type;
    }

    /*!
     * @brief Sets configuration index.
     * @param[in] configuration_index Configuration index used by IPMI.
     * */
    void set_configuration_index(std::uint8_t configuration_index) {
        m_configuration_index = configuration_index;
    }

    /*!
     * @brief Gets configuration index.
     * @return Configuration index used by IPMI.
     * */
    std::uint8_t get_configuration_index() const {
        return m_configuration_index;
    }

    /*!
     * @brief Get TPM task
     * @return TPM task
     * */
    const OptionalField<std::string>& get_task() const {
        return m_task;
    }

    /*!
     * @brief Set TPM task
     * @param[in] task the TPM task
     * */
    void set_task(const OptionalField<std::string>& task) {
        m_task = task;
    }
private:
    OptionalField<std::string> m_firmware_version{};
    OptionalField<enums::InterfaceType> m_interface_type{};

    /*! IPMI specific data */
    std::uint8_t m_configuration_index{};

    /*!
     * @brief m_task - internal flag. If it's not empty, it points
     * to a running task whose purpose is to set the TPM's state.
     * Used to prevent duplicate tasks for the same TPM.
     */
    OptionalField<Uuid> m_task{};

    static const enums::Component component;
};

} // namespace model
} // namespace agent_framework
