/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief BMC username and password encrypt utility (uses AES256 CBC).
 *
 * @copyright Copyright (c) 2015-2019 Intel Corporation.
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
 * @file main.cpp
 */

#include <iostream>
#include <safe_lib.h>
#include <string.h>
#include <string>

#include "utils/conversion.hpp"
#include "utils/crypt_utils.hpp"

#include <gcrypt.h>

static constexpr const int STATUS_OK = 0;
static constexpr const int STATUS_ERROR_IO = 3;

static constexpr const int CFG_ARG_MIN = 2;
static constexpr const size_t CFG_PASS_LENGTH_MAX = 256;
static constexpr const size_t CFG_PASS_LENGTH_MIN = 14;

namespace encrypter {

/*!
 * @brief Prints tool's usage help and examples.
 * @param program_name name equals argv[0].
 */
void print_usage(std::string program_name);

/*!
 * @brief Parses and validates arguments given in command line.
 * @param argc argument count.
 * @param argv argument vector.
 * @return password to hash
 */
std::string parse_options(int argc, char** argv);

/*!
 * @brief Hashes the password using PBKDF2 algorithm (SHA512 based with salt).
 *        Prints the hash value to stdout.
 * @param password string containing password to hash.
 */
void hash_operation(const std::string& password);

void print_usage(std::string program_name) {
    std::cerr << "Tool to hash given password with PBKDF2 (SHA-512 based with salt).\n"
              << "Usage: " << program_name << " <password> | [--help]]" << std::endl;
}

std::string parse_options(int argc, char** argv) {
    if (argc < CFG_ARG_MIN || (strcmp(argv[1], "--help") == 0)) {
        print_usage(argv[0]);
        throw std::invalid_argument("Too few arguments!");
    }

    // Get password to hash
    auto cmd_password_length = strlen(argv[1]);
    if (cmd_password_length > CFG_PASS_LENGTH_MAX - 1) {
        std::cout << "Password length: " << cmd_password_length
                  << "\nPassword max length: " << CFG_PASS_LENGTH_MAX - 1 << std::endl;
        throw std::invalid_argument("Password too long!");
    }

    if (cmd_password_length < CFG_PASS_LENGTH_MIN) {
        std::cout << "Password length: " << cmd_password_length
                  << "\nPassword min length: " << CFG_PASS_LENGTH_MIN << std::endl;
        throw std::invalid_argument("Password too short!");
    }

    return std::string(argv[1]);
}

void hash_operation(const std::string& password) {
    std::string salt = utils::generate_salt();
    std::string hashed_password = utils::salted_hash(password, salt);

    // Hashed password as a hex string to be pasted to 'authentication' property in configuration file, in the form of:
    // Concatenated salted hash string [80B] = salt [16B] + hash [64B]
    std::cout << utils::string_to_hex_string(salt) + utils::string_to_hex_string(hashed_password) << std::endl;
}

} // namespace encrypter

int main(int argc, char** argv) {
    if (!gcry_check_version(nullptr)) {
        std::cerr << "libgcrypt initialization failed" << std::endl;
        return 1;
    }
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

    try {
        auto options = encrypter::parse_options(argc, argv);
        encrypter::hash_operation(options);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return STATUS_ERROR_IO;
    }

    return STATUS_OK;
}
