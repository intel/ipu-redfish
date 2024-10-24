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
project(Configuration CXX)

# TODO: change flag when all unnecessary usages are removed  - required in GenerateConfigHeader.cmake
set(CONFIGURATION_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    CACHE PATH "Configuration include dir path"
)

add_library(configuration STATIC
    src/utils.cpp
    src/configuration.cpp
)

target_link_libraries(configuration
    PUBLIC
    json

    PRIVATE
    logger
    utils
    safestring_static
)

target_include_directories(configuration
    PUBLIC
    include
)
