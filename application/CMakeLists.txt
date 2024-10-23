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
project("PSME Application" C CXX)

set(PSME_APPLICATION_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${BASE64_INCLUDE_DIRS}
    ${COMMON_INCLUDE_DIRS}
    ${CONFIGURATION_INCLUDE_DIRS}
    ${LOGGER_INCLUDE_DIRS}
    ${DATABASE_INCLUDE_DIRS}
    ${AGENT_FRAMEWORK_INCLUDE_DIRS}
    ${UTILS_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include_directories(SYSTEM
    ${UUID_INCLUDE_DIRS}
)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)

install(
    TARGETS ipu-redfish-server RUNTIME DESTINATION usr/bin
)

install(
    FILES config/imc-redfish-configuration.json
          config/ipu-redfish-gnutls.ini
    DESTINATION etc COMPONENT config
)

install(
    FILES config/ipu-redfish-generate-test-certs.sh
          config/ipu-redfish-generate-password-hash.sh
          config/ipu-redfish-service.sh
    DESTINATION usr/bin
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
)
