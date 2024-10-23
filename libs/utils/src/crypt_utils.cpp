/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright Copyright (c) 2019 Intel Corporation.
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
 * @file crypt_utils.cpp
 */

#include "utils/crypt_utils.hpp"
#include <array>
#include <cstdint>
#include <gcrypt.h>
#include <stdexcept>

static constexpr const uint32_t PSME_KDF_ALGO = GCRY_KDF_PBKDF2;
static constexpr const uint32_t PSME_KDF_SUBALGO = GCRY_MD_SHA512;

static constexpr const size_t KDF_KEY_SIZE = 64;
static constexpr const size_t KDF_SALT_SIZE = 16;
static constexpr const size_t KDF_ITERATIONS = 10000;

namespace utils {

std::string salted_hash(const std::string& password, const std::string& salt) {
    std::array<uint8_t, KDF_KEY_SIZE> derived_key;
    derived_key.fill(0);

    gcry_error_t error = gcry_kdf_derive(password.data(),
                                         password.size(),
                                         PSME_KDF_ALGO,
                                         PSME_KDF_SUBALGO,
                                         salt.data(),
                                         salt.size(),
                                         KDF_ITERATIONS,
                                         KDF_KEY_SIZE,
                                         derived_key.data());
    if (error) {
        throw std::runtime_error("Error on hashing (KDF): " + std::string(gcry_strerror(error)));
    }

    std::string salted_hash_string(derived_key.begin(), derived_key.end());
    return salted_hash_string;
}

size_t get_kdf_key_size() {
    return KDF_KEY_SIZE;
}

size_t get_salt_size() {
    return KDF_SALT_SIZE;
}

std::string generate_salt() {
    char salt[KDF_SALT_SIZE + 1]{0};
    gcry_create_nonce(salt, KDF_SALT_SIZE);

    if (std::string(salt).length() < KDF_SALT_SIZE) {
        for (uint32_t index = 0; index < KDF_SALT_SIZE; index++) {
            while (!salt[index]) {
                gcry_create_nonce(salt + index, 1);
            }
        }
    }

    return std::string(salt);
}

} // namespace utils
