/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief Various conversion utils.

 * @copyright Copyright (c) 2018-2019 Intel Corporation.
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
 * @file conversion.hpp
 */

#pragma once

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace utils {

/*! Exponents used for units conversion, Base equals 2. */
enum class BytePrefix : uint32_t {
    Noop = 0,
    Kibi = 10,
    Mibi = 20,
    Gibi = 30,
    Tebi = 40
};

/*!
 * @brief Converts units 'from' bytes to 'to' bytes,
 * where 'from' and 'to' are binary prefixes (Kibi, Mibi, Gibi)
 * @param value byte value in 'from' units
 * @param from binary prefix for input units
 * @param to binary prefix for output units
 * @return value in 'to' units
 */
double convert_byte_units(double value, BytePrefix from, BytePrefix to);

/*!
 * @brief Converts string to hex form (ASCII codes).
 * @param ascii_string string to be represented in ASCII hex codes.
 */
std::string string_to_hex_string(const std::string& ascii_string);

/*!
 * @brief Converts hex string (ASCII codes) to string form.
 * @param hex_string ASCII hex codes string to be represented as string.
 */
std::string hex_string_to_string(const std::string& hex_string);

} // namespace utils