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
 * @file gami_exception.cpp
 *
 * @brief ExceptionBase implementation
 * */

#include "agent-framework/exceptions/gami_exception.hpp"

#include <algorithm>
#include <sstream>

namespace {
static const constexpr std::uint8_t LF = 0x0A;
}

using namespace agent_framework::exceptions;

const constexpr char GamiException::NOT_SPECIFIED[];

GamiException::GamiException(const ErrorCode ec, const std::string& error_message) : m_data(json::Json::value_t::null), m_error_code(ec), m_message(error_message), m_what(make_what()) {}

GamiException::GamiException(const ErrorCode ec, const std::string& error_message, const json::Json& data_json) : m_data(data_json),
                                                                                                                  m_error_code(ec),
                                                                                                                  m_message(error_message), m_what(make_what()) {}

GamiException::~GamiException() {}

ErrorCode GamiException::get_error_code() const {
    return m_error_code;
}

const std::string& GamiException::get_message() const {
    return m_message;
}

const json::Json& GamiException::get_data() const {
    return m_data;
}

const char* GamiException::what() const noexcept {
    return m_what.c_str();
}

std::string GamiException::get_styled_string_from_data(const json::Json& data, const std::string& field_key) {
    if (data.count(field_key)) {
        auto field_value = data[field_key].dump();

        field_value.erase(std::remove(field_value.begin(), field_value.end(), LF), field_value.end());
        std::replace(field_value.begin(), field_value.end(), '\"', '\'');

        return field_value;
    }
    return {};
}

std::string GamiException::get_string_from_data(const json::Json& data, const std::string& field_key) {
    if (data.count(field_key)) {
        return data[field_key];
    }
    return {};
}

json::Json GamiException::get_json_from_data(const json::Json& data, const std::string& field_key) {
    if (data.count(field_key)) {
        return data[field_key];
    }
    return {};
}

std::string GamiException::make_what() {
    int code = static_cast<int>(m_error_code);
    if (code != 0) {
        std::stringstream ss{};
        ss << "Exception " << code << ": " << m_message;
        if (!m_data.empty()) {
            ss << ", data: " << m_data.dump();
        }
        return ss.str();
    } else {
        return m_message;
    }
}
