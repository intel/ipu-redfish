/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/enum/compute.hpp"
#include "agent-framework/module/model/resource.hpp"

namespace agent_framework {
namespace model {

/*! BootOption model class */
class BootOption : public Resource {
public:
    explicit BootOption(const std::string& parent_uuid = {},
                        enums::Component parent_type = enums::Component::System);

    virtual ~BootOption();

    BootOption(const BootOption&) = default;

    BootOption& operator=(const BootOption&) = default;

    BootOption(BootOption&&) = default;

    BootOption& operator=(BootOption&&) = default;

    friend auto operator<=>(const BootOption& lhs, const BootOption& rhs) = default;

    /*!
     * Get component name enum
     *
     * @return component name enum
     * */
    static enums::Component get_component() {
        return BootOption::component;
    }

    /*!
     * @brief Get boot option reference
     * @return boot option reference
     * */
    const OptionalField<std::string>& get_reference() const {
        return m_reference;
    }

    /*!
     * @brief Set boot option reference
     * @param[in] reference boot option reference
     * */
    void set_reference(const OptionalField<std::string>& reference) {
        m_reference = reference;
    }

    /*!
     * @brief Get boot option display name
     * @return boot option display name
     * */
    const OptionalField<std::string>& get_display_name() const {
        return m_display_name;
    }

    /*!
     * @brief Set boot option display name
     * @param[in] display_name boot option display name
     * */
    void set_display_name(const OptionalField<std::string>& display_name) {
        m_display_name = display_name;
    }

    /*!
     * @brief Check if boot option is enabled
     * @return True if is enabled, false if not.
     */
    bool is_enabled() const {
        return m_enabled;
    }

    /*!
     * @brief Set enabled flag
     * @param[in] enabled boot option enabled flag.
     */
    void set_enabled(bool enabled) {
        m_enabled = enabled;
    }

    /*!
     * @brief Set boot option alias
     * @param[in] alias boot option alias
     * */
    void set_alias(const OptionalField<enums::BootOverrideTarget>& alias) {
        m_alias = alias;
    }

    /*!
     * @brief Get boot option alias
     * @return boot option alias
     * */
    const OptionalField<enums::BootOverrideTarget>& get_alias() const {
        return m_alias;
    }
private:
    OptionalField<std::string> m_reference{};
    OptionalField<std::string> m_display_name{};
    bool m_enabled{false};
    OptionalField<enums::BootOverrideTarget> m_alias{};

    static const enums::Component component;
};

} // namespace model
} // namespace agent_framework
