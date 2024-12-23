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
 * */

#include "psme/rest/server/error/server_exception.hpp"

using namespace psme::rest::error;

ServerException::ServerException(const ServerError& error) : m_error(error) {}

ServerException::~ServerException() noexcept {}

const char* ServerException::what() const noexcept {
    try {
        make_what();
    }
    catch (const std::exception& e) {
        m_what = "Failed to generate error message: ";
        m_what.append(e.what());
    }
    return m_what.c_str();
}

void ServerException::make_what() const {
    m_what = m_error.get_message();
}