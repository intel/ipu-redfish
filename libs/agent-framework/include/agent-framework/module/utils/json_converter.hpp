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
 * @brief JSON converter
 * */

#pragma once

#include "is_framework_enum.hpp"

#include "json-wrapper/json-wrapper.hpp"

namespace agent_framework {
namespace module {
namespace utils {

template <typename T>
class OptionalField;

/*!
 * @brief JSON converting class
 *
 * This class can be used to convert ordinary objects and framework enums to and
 * from an object representing a JSON.
 * */
class JsonConverter {
    template <typename U>
    using is_framework_enum = model::utils::is_framework_enum<U>;
public:
    /*!
     * @brief Convert an ordinary object to JSON of type json::Json.
     *
     * @param[in] in_object Object to be converted to json::Json
     *
     * @return json::Json object containing representation of in_object
     * */
    template <typename ObjectType, typename std::enable_if<!is_framework_enum<ObjectType>::value>::type* = nullptr>
    static json::Json to_json(const ObjectType& in_object) {
        return json::Json(in_object);
    }

    /*!
     * @brief Convert a framework enum object to JSON of type json::Json.
     *
     * @param[in] in_object Object to be converted to json::Json
     *
     * @return json::Json object containing representation of in_object
     * */
    template <typename ObjectType, typename std::enable_if<is_framework_enum<ObjectType>::value>::type* = nullptr>
    static json::Json to_json(const ObjectType& in_object) {
        return json::Json(in_object.to_string());
    }

    /*!
     * @brief Convert a json::Json object to an ordinary object
     *
     * @param[in] json Object to construct from
     *
     * @return Object constructed from json
     * */
    template <typename ObjectType, typename std::enable_if<!is_framework_enum<ObjectType>::value>::type* = nullptr>
    static OptionalField<ObjectType> from_json(const json::Json& json);

    /*!
     * @brief Convert a json::Json object to a framework enum
     *
     * @param[in] json Object to construct from
     *
     * @return Object constructed from json
     * */
    template <typename ObjectType, typename std::enable_if<is_framework_enum<ObjectType>::value>::type* = nullptr>
    static OptionalField<ObjectType> from_json(const json::Json& json) {
        if (!is_null(json)) {
            ObjectType out_field = out_field.from_string(json_to_string(json));
            return OptionalField<ObjectType>(out_field);
        }
        return OptionalField<ObjectType>();
    }
protected:
    /*!
     * @brief Convert a json field to string
     *
     * @param[in] json Object to convert to string
     *
     * @return String constructed from json
     * */
    static std::string json_to_string(const json::Json& json);

    /*!
     * @brief Check if a json object is null
     *
     * @param[in] json Json to be checked
     *
     * @return True if json is null, false otherwise.
     * */
    static bool is_null(const json::Json& json);
};

} // namespace utils
} // namespace module
} // namespace agent_framework
