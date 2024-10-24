/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/model/attributes/model_attributes.hpp"
#include "agent-framework/module/model/resource.hpp"

namespace agent_framework {
namespace model {

/*! Virtual Media */
class VirtualMedia : public Resource {
public:
    explicit VirtualMedia(const std::string& parent_uuid = {},
                          enums::Component parent_type = enums::Component::System);

    virtual ~VirtualMedia();

    friend auto operator<=>(const VirtualMedia& lhs, const VirtualMedia& rhs) = default;

    /*!
     * @brief Get media type
     * @return media type
     * */
    const enums::MediaType& get_media_type() const {
        return m_media_type;
    }

    /*!
     * @brief Get inserted value
     * @return inserted
     * */
    bool get_inserted() const {
        return m_inserted;
    }

    /*!
     * @brief Set inserted
     * @param[in] inserted
     * */
    void set_inserted(bool inserted) {
        m_inserted = inserted;
    }

    /*!
     * @brief Get image name reference
     * @return image name reference
     * */
    const OptionalField<std::string>& get_image_name() const {
        return m_image_name;
    }

    /*!
     * @brief Set image name
     * @param[in] image name reference
     * */
    void set_image_name(const OptionalField<std::string>& image_name) {
        m_image_name = image_name;
    }

    /*!
     * @brief Get component name
     * @return component name
     */
    static enums::Component get_component() {
        return VirtualMedia::component;
    }
private:
    const enums::MediaType m_media_type = enums::MediaType::CD;
    bool m_inserted = false;
    OptionalField<std::string> m_image_name{};

    static const enums::Component component;
};

} // namespace model
} // namespace agent_framework
