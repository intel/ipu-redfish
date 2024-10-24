/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief Cryptographic utils.

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
 * @file crypt_utils.hpp
 */

#pragma once

#include <string>

namespace utils {

/*!
 * @brief Generates key/hash with PBKDF2 key derivation function (SHA512 based).
 * @param password string with the plain text password to be hashed.
 * @param salt string with random data
 * @return password hash with salt added.
 * */
std::string salted_hash(const std::string& password, const std::string& salt);

/*!
 * @brief Gets derived key/hash size for a configured PBKDF2 function.
 * @return key/hash size in Bytes.
 * */
size_t get_kdf_key_size();

/*!
 * @brief Gets salt size for a configured PBKDF2 function.
 * @return salt size in Bytes.
 * */
size_t get_salt_size();

/*!
 * @brief Generates random 16-byte salt.
 * @return cryptographic salt (to be used for PBKDF2 by default).
 * */
std::string generate_salt();

} // namespace utils
