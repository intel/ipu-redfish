/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief Memory: Security Capabilities attribute
 *
 * @copyright Copyright (c) 2018-2019 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file security_capabilities.hpp
 */
#pragma once
#include "agent-framework/module/enum/compute.hpp"
#include "agent-framework/module/model/attributes/array.hpp"
#include "agent-framework/module/utils/utils.hpp"
#include "json-wrapper/json-wrapper.hpp"

namespace agent_framework {
namespace model {
namespace attribute {

/*! Memory: Security Capabilities */
class SecurityCapabilities {
public:
    explicit SecurityCapabilities();

    SecurityCapabilities(const bool passphrase_capable,
                         const std::uint32_t max_passphrase_count) : m_passphrase_capable{passphrase_capable},
                                                                     m_max_passphrase_count{max_passphrase_count} {}

    ~SecurityCapabilities();

    /*! Enable copy */
    SecurityCapabilities(const SecurityCapabilities&) = default;
    SecurityCapabilities& operator=(const SecurityCapabilities&) = default;
    SecurityCapabilities(SecurityCapabilities&&) = default;
    SecurityCapabilities& operator=(SecurityCapabilities&&) = default;
    friend auto operator<=>(const SecurityCapabilities& lhs, const SecurityCapabilities& rhs) = default;

    /*!
     * @brief Sets passphrase capable attribute
     * @param[in] passphrase_capable Passphrase capable
     * */
    void set_passphrase_capable(const OptionalField<bool>& passphrase_capable) {
        m_passphrase_capable = passphrase_capable;
    }

    /*!
     * @brief Gets passphrase capable attribute
     * @return Passphrase capable
     * */
    OptionalField<bool> get_passphrase_capable() const {
        return m_passphrase_capable;
    }

    /*!
     * @brief Sets max passphrase count attribute
     * @param[in] max_passphrase_count Max passphrase count
     * */
    void set_max_passphrase_count(const OptionalField<std::uint32_t>& max_passphrase_count) {
        m_max_passphrase_count = max_passphrase_count;
    }

    /*!
     * @brief Gets max passphrase count attribute
     * @return Max passphrase count
     * */
    OptionalField<std::uint32_t> get_max_passphrase_count() const {
        return m_max_passphrase_count;
    }
private:
    OptionalField<bool> m_passphrase_capable{};
    OptionalField<std::uint32_t> m_max_passphrase_count{};
};

} // namespace attribute
} // namespace model
} // namespace agent_framework
