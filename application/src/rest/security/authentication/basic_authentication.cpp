/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2018-2019 Intel Corporation
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
 * @file basic_authentication.cpp
 * */

#include "psme/rest/security/authentication/basic_authentication.hpp"
#include "logger/logger_factory.hpp"
#include "psme/rest/security/account/account_manager.hpp"
#include <chrono>
#include <random>
#include <thread>

#include <microhttpd.h>

using namespace psme::rest::security::authentication;
using namespace psme::rest::security::account;
using namespace psme::rest::server;

bool BasicAuthentication::credentials_valid(MHD_Connection* connection) {
    char* user = nullptr;
    char* pass = nullptr;

    user = MHD_basic_auth_get_username_password(connection, &pass);
    bool success = false;
    if (user && pass) {
        success = AccountManager::get_instance()->validate_credentials(std::string(user), std::string(pass));
        if (!success) {
            log_error("rest", "AccountManager: failed to validate credentials for user " << user);
            // introduce a random delay before reporting authentication failure
            // SDLe task "T73: Use random delays in authentication failures"
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distr(1.0, 5.0);
            double random_sleep_time = distr(gen);
            std::this_thread::sleep_for(std::chrono::duration<double>(random_sleep_time));
        } else {
            log_debug("rest", "AccountManager: successfully validated credentials for user " << user);
        }
    }

    if (user != NULL) {
        free(user);
    }
    if (pass != NULL) {
        free(pass);
    }
    return success;
}

AuthStatus
BasicAuthentication::perform(MHD_Connection* connection, const std::string& url, server::Response& response) {
    if (!credentials_valid(connection)) {
        response.set_status(server::status_4XX::UNAUTHORIZED);
        auto header_value = std::string(server::http_headers::WWWAuthenticate::BASIC) + " " + std::string(server::http_headers::WWWAuthenticate::REALM) + "=" + url;
        response.set_header(http_headers::ContentType::CONTENT_TYPE, http_headers::ContentType::JSON);
        response.set_header(http_headers::WWWAuthenticate::WWW_AUTHENTICATE, header_value);
        return AuthStatus::FAIL;
    }
    return AuthStatus::SUCCESS;
}
