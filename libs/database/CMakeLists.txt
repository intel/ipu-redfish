# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

# <license_header>
#
# Copyright (c) 2015-2019 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# </license_header>

cmake_minimum_required(VERSION 3.4)
project("Database" CXX)

set(DATABASE_LIBRARIES
    database
    CACHE PATH "Database libraries"
    )

set(DATABASE_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    CACHE PATH "Database include directory"
    )

add_library(database STATIC
    src/database.cpp
    src/file_database.cpp
    src/persistent_attributes.cpp
)

target_include_directories(database
    SYSTEM PRIVATE

    PUBLIC
    include
)

target_link_libraries(database
    PRIVATE
    logger
    generic
    safestring_static
)

add_subdirectory(tests)
