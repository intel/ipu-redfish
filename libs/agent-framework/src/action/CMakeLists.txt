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

add_library(agent-framework-action STATIC
    task.cpp
    task_creator.cpp
    task_runner.cpp
    task_result_manager.cpp
)

target_link_libraries(agent-framework-action
    PUBLIC
    agent-framework-module
    agent-framework-threading
    agent-framework-exceptions
    generic
)

target_include_directories(agent-framework-action
    PUBLIC
    ${AGENT_FRAMEWORK_DIR}/include
)

