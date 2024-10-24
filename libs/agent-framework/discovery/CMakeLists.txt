# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

# <license_header>
#
# Copyright (c) 2018-2019 Intel Corporation
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
project("AGENT-FRAMEWORK DISCOVERY" CXX)

add_library(agent-framework-discovery STATIC
    src/builders/ethernet_interface_builder.cpp
)

target_include_directories(agent-framework-discovery
    PUBLIC
    include
)

target_link_libraries(agent-framework-discovery
    PUBLIC
    agent-framework-module # TODO: should use only model lib from framework

    PRIVATE
    agent-framework-database
    net
)
