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
 * @file resource.hpp
 * @brief GAMI Resource interface
 * */
#pragma once

#include "agent-framework/generic/obj_reference.hpp"
#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/model/attributes/model_attributes.hpp"
#include "agent-framework/module/utils/utils.hpp"

#include <chrono>
#include <mutex>

namespace agent_framework {
namespace model {

/*! Resource */
class Resource {
public:
    using Reference = agent_framework::generic::ObjReference<Resource, std::recursive_mutex>;
    using RestId = std::uint64_t;

    explicit Resource(const Uuid& parent_uuid, enums::Component parent_type = enums::Component::None);

    virtual ~Resource();

    /*! Enable copy */
    Resource(const Resource&) = default;

    Resource& operator=(const Resource&) = default;

    Resource(Resource&&) = default;

    Resource& operator=(Resource&&) = default;

    friend auto operator==(const Resource& lhs, const Resource& rhs) {
        return (lhs.m_name == rhs.m_name) &&
               (lhs.m_description == rhs.m_description) &&
               (lhs.m_id == rhs.m_id) &&
               (lhs.m_parent_type == rhs.m_parent_type) &&
               (lhs.m_status == rhs.m_status) &&
               (lhs.get_uuid() == rhs.get_uuid());
    };

    /*!
     * @brief Get uuid
     * @return Component uuid
     * */
    const Uuid& get_uuid() const {
        return (m_is_uuid_persistent ? m_persistent_uuid : m_temporary_uuid);
    }

    void set_uuid(const Uuid& uuid) {
        m_is_uuid_persistent = false;
        m_temporary_uuid = uuid;
    }

    const Uuid& get_temporary_uuid() const {
        return m_temporary_uuid;
    }

    const Uuid& get_persistent_uuid() const {
        return m_persistent_uuid;
    }

    /*!
     * @brief Create persistent resource UUID
     *
     * Attempts to create persistent UUIDv5 for the resource based on the unique
     * key set by the function set_unique_key(). If the key was not set prior to
     * calling make_persistent_uuid(), UUIDv1 is generated and no there is no
     * guarantee that upon service restart the resource will get the same UUID.
     * */
    const Uuid& make_persistent_uuid();

    /*!
     * @brief Create random resource UUID
     * */
    const Uuid& make_random_uuid();

    /*!
     * @brief Get unique key
     *
     * @return OptionalField wrapper containing the resource unique key
     * */
    const OptionalField<std::string>& get_unique_key() const {
        return m_unique_key;
    }

    /*!
     * @brief Set unique key
     *
     * @param[in] in_key Value of the unique key
     * */
    void set_unique_key(const OptionalField<std::string>& in_key) {
        m_unique_key = in_key;
    }

    /*!
     * @brief Get parent's uuid
     * @return parent's uuid
     * */
    const Uuid& get_parent_uuid() const {
        return m_parent_uuid;
    }

    /*!
     * @brief Set parent UUID
     * @param[in] parent Parent UUID
     * */
    void set_parent_uuid(const Uuid& parent) {
        m_parent_uuid = parent;
    }

    /*!
     * @brief Gets parent component type
     *
     * @return parent component type
     * */
    enums::Component get_parent_type() const {
        return m_parent_type;
    }

    /*!
     * @brief Sets parent component type
     *
     * @param[in] type parent component type
     * */
    void set_parent_type(enums::Component type) {
        m_parent_type = type;
    }

    /*!
     * @brief Gets status
     *
     * @return Status reference
     * */
    const attribute::Status& get_status() const {
        return m_status;
    }

    /*!
     * @brief Set status
     *
     * @param[in] status Resource's status
     * */
    void set_status(const attribute::Status& status) {
        m_status = status;
    }

    /*!
     * @brief Gets name of the resource
     * @return Resource's name
     * */
    const OptionalField<std::string>& get_name() const {
        return m_name;
    }

    /*!
     * @brief Sets name of the resource
     * @param[in] name Resource's name
     * */
    void set_name(const OptionalField<std::string>& name) {
        m_name = name;
    }

    /*!
     * @brief Gets description of the resource
     * @return Description's name
     * */
    const OptionalField<std::string>& get_description() const {
        return m_description;
    }

    /*!
     * @brief Sets description of the resource
     * @param[in] description Description's name
     * */
    void set_description(const OptionalField<std::string>& description) {
        m_description = description;
    }

    /*!
     * @brief Get id
     *
     * @return resource's REST id
     */
    RestId get_id() const {
        return m_id;
    }

    /*!
     * @brief Set id
     *
     * @param id the REST id to set for resource
     */
    void set_id(const RestId& id) {
        m_id = id;
    }

    /*!
     * @brief Update resource touched epoch
     *
     * param epoch Touch epoch
     * */
    void touch(const std::uint64_t epoch) {
        m_touched_at = epoch;
    }

    /*!
     * @brief Verify if resource was updated after a given epoch
     *
     * @param epoch Touch epoch
     *
     * @return True if modified after epoch, else otherwise
     * */
    bool was_touched_after(const std::uint64_t epoch) const {
        return (m_touched_at > epoch);
    }

    /*!
     * @brief Check if the resource UUID is persistent or not
     *
     * @return True if UUID is persistent, false otherwise
     * */
    bool has_persistent_uuid() const {
        return m_is_uuid_persistent;
    }

    /*!
     * @brief Check if given resource has Enabled/OK status.
     * @param resource Reference to resource.
     * @return Returns true if resource is healthy, false othrwise.
     */
    static bool is_healthy(const Resource& resource) {
        return resource.get_status().get_state() == enums::State::Enabled &&
               resource.get_status().get_health() == enums::Health::OK;
    }
private:
    /* GAMI model attributes */
    OptionalField<std::string> m_name{};
    OptionalField<std::string> m_description{};
    attribute::Status m_status{};

    enums::Component m_parent_type{enums::Component::None};

    /* UUIDs */
    Uuid m_temporary_uuid{};
    Uuid m_persistent_uuid{};
    Uuid m_parent_uuid{};
    OptionalField<std::string> m_unique_key{};
    bool m_is_uuid_persistent{false};

    /* These members are being used by REST application */
    RestId m_id{0};
    std::uint64_t m_touched_at{0};
};

} // namespace model
} // namespace agent_framework
