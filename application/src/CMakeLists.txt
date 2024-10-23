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


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
    app.cpp
    context.cpp
    main.cpp
   )

include_directories(
    ${PSME_APPLICATION_DIR}/include
    ${PSME_APPLICATION_DIR}/include/psme
)

add_subdirectory(rest)
add_subdirectory(ssdp)
add_subdirectory(ipu)

add_executable(ipu-redfish-server ${SOURCES})

target_link_libraries(ipu-redfish-server
    application-rest
    ipu
    ssdp-config-loader
    metadata
    agent-framework
    uuid
    microhttpd
    curl
    json
    configuration
    base64
    gnutls
    ${libzstd_LIBRARIES}
    ${libbrotlidec_LIBRARIES}
    gcrypt
    gpg-error
    hogweed
    nettle
    gmp
    ZLIB::ZLIB
    ssdp
    logger
    utils
)

set(VERSION_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_custom_command(
    OUTPUT ${VERSION_LOCATION}/version.hpp
    COMMAND ${VERSION_LOCATION}/update_version.sh
    DEPENDS ${VERSION_LOCATION}/update_version.sh
    WORKING_DIRECTORY ${VERSION_LOCATION}
    COMMENT "Running update_version.sh to update version.hpp"
)

# Ensure the custom command is run before building the target
add_custom_target(run_update_version ALL
    DEPENDS ${VERSION_LOCATION}/version.hpp
)

add_dependencies(ipu-redfish-server run_update_version)
