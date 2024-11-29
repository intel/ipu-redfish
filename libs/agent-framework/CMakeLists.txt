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

cmake_minimum_required(VERSION 3.20)
project("PSME Agent Framework" C CXX)

set(AGENT_FRAMEWORK_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(AGENT_FRAMEWORK_INCLUDE_DIRS
    ${DATABASE_INCLUDE_DIRS}
    ${CONFIGURATION_INCLUDE_DIRS}
    ${LOGGER_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    CACHE PATH "Agent framework include dir path"
)

add_library(agent-framework STATIC
    src/logger_loader.cpp
)

target_link_libraries(agent-framework
    PUBLIC
    agent-framework-database
    agent-framework-module
    agent-framework-threading
    agent-framework-validators
    agent-framework-action
    json

    PRIVATE
    pthread
)

target_include_directories(agent-framework
    PUBLIC
    include
)

add_subdirectory(src)
add_subdirectory(tests)

# Modern CMake targets:
add_subdirectory(database)
add_subdirectory(discovery)
