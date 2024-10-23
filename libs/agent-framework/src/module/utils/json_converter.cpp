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
 * @brief System model interface
 * */

#include "agent-framework/module/utils/json_converter.hpp"
#include "agent-framework/module/utils/optional_field.hpp"

namespace agent_framework {
namespace module {
namespace utils {

/* Template specializations to check if a json field is null */
bool JsonConverter::is_null(const json::Json& json) {
    return json.is_null();
}

/* Template specialization for construction from json::Json object */
template <>
OptionalField<double> JsonConverter::from_json<double>(const json::Json& json) {
    return (is_null(json) ? OptionalField<double>() : OptionalField<double>(json.get<double>()));
}

template <>
OptionalField<std::string> JsonConverter::from_json<std::string>(const json::Json& json) {
    return (is_null(json) ? OptionalField<std::string>() : OptionalField<std::string>(json.get<std::string>()));
}

template <>
OptionalField<int> JsonConverter::from_json<int>(const json::Json& json) {
    return (is_null(json) ? OptionalField<int>() : OptionalField<int>(json.get<int>()));
}

template <>
OptionalField<unsigned int> JsonConverter::from_json<unsigned int>(const json::Json& json) {
    return (is_null(json) ? OptionalField<unsigned int>() : OptionalField<unsigned int>(json.get<unsigned int>()));
}

template <>
OptionalField<int64_t> JsonConverter::from_json<int64_t>(const json::Json& json) {
    return (is_null(json) ? OptionalField<int64_t>() : OptionalField<int64_t>(json.get<std::int64_t>()));
}

template <>
OptionalField<uint64_t> JsonConverter::from_json<uint64_t>(const json::Json& json) {
    return (is_null(json) ? OptionalField<uint64_t>() : OptionalField<uint64_t>(json.get<uint64_t>()));
}

template <>
OptionalField<bool> JsonConverter::from_json<bool>(const json::Json& json) {
    return (is_null(json) ? OptionalField<bool>() : OptionalField<bool>(json.get<bool>()));
}

template <>
OptionalField<const char*> JsonConverter::from_json<const char*>(const json::Json& json) {
    return (is_null(json) ? OptionalField<const char*>() : OptionalField<const char*>(json.get_ref<const std::string&>().c_str()));
}

/* Template specializations to convert json to string */
std::string JsonConverter::json_to_string(const json::Json& json) {
    return json;
}

} // namespace utils
} // namespace module
} // namespace agent_framework
