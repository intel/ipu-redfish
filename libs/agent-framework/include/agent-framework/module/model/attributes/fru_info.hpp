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
 * @file fru_info.hpp
 * @brief FRU info
 * */
#pragma once

#include "agent-framework/module/utils/utils.hpp"
#include "json-wrapper/json-wrapper.hpp"

#include <string>

namespace agent_framework {
namespace model {
namespace attribute {

/*! FRU Info */
class FruInfo {
public:
    explicit FruInfo();

    FruInfo(const OptionalField<std::string>& serial_number,
            const OptionalField<std::string>& manufacturer = {},
            const OptionalField<std::string>& model_number = {},
            const OptionalField<std::string>& part_number = {}) : m_serial_number{serial_number},
                                                                  m_manufacturer{manufacturer},
                                                                  m_model_number{model_number},
                                                                  m_part_number{part_number} {}

    ~FruInfo();

    /*! Enable copy */
    FruInfo(const FruInfo&) = default;

    FruInfo& operator=(const FruInfo&) = default;

    FruInfo(FruInfo&&) = default;

    FruInfo& operator=(FruInfo&&) = default;

    friend auto operator<=>(const FruInfo& lhs, const FruInfo& rhs) = default;

    /*!
     * Sets serial number.
     *
     * @param[in] serial_number serial number.
     * */
    void set_serial_number(const OptionalField<std::string>& serial_number) {
        m_serial_number = serial_number;
    }

    /*!
     * Gets serial number.
     * */
    const OptionalField<std::string>& get_serial_number() const {
        return m_serial_number;
    }

    /*!
     * Sets manufacturer name
     *
     * @param[in] manufacturer Manufacturer name
     * */
    void set_manufacturer(const OptionalField<std::string>& manufacturer) {
        m_manufacturer = manufacturer;
    }

    /*!
     * Gets manufacturer name
     * */
    const OptionalField<std::string>& get_manufacturer() const {
        return m_manufacturer;
    }

    /*!
     * Sets model number
     *
     * @param[in] model_number model number
     * */
    void set_model_number(const OptionalField<std::string>& model_number) {
        m_model_number = model_number;
    }

    /*!
     * Gets model number
     * */
    const OptionalField<std::string>& get_model_number() const {
        return m_model_number;
    }

    /*!
     * Sets part number
     *
     * @param[in] part_number part number
     * */
    void set_part_number(const OptionalField<std::string>& part_number) {
        m_part_number = part_number;
    }

    /*!
     * Gets part number
     * */
    const OptionalField<std::string>& get_part_number() const {
        return m_part_number;
    }
private:
    OptionalField<std::string> m_serial_number{};
    OptionalField<std::string> m_manufacturer{};
    OptionalField<std::string> m_model_number{};
    OptionalField<std::string> m_part_number{};
};

} // namespace attribute
} // namespace model
} // namespace agent_framework
