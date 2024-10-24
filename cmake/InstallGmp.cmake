# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

# <license_header>
#
# Copyright (c) 2017-2019 Intel Corporation
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

if(CMAKE_CROSSCOMPILING)
    return()
endif()

# Not the latest version - intentionally using 6.2.1 - same as in IPU SDK
FetchContent_Declare(gmp
    URL "https://ftp.gnu.org/gnu/gmp/gmp-6.2.1.tar.xz"
    URL_HASH MD5=0b82665c4a92fd2ade7440c13fcaa42b
)

pkg_check_modules(gmp IMPORTED_TARGET gmp=6.2.1)
if(gmp_FOUND)
    message("gmp library ver: ${gmp_VERSION}")
    message("gmp include dir: ${gmp_INCLUDE_DIRS}")
    message("gmp library dir: ${gmp_LIBRARY_DIRS}")
endif()

if(NOT gmp_POPULATED AND NOT gmp_FOUND)
    message("Building gmp...")
    FetchContent_Populate(gmp)

    set(CONFIGURE_FLAGS)
    list(APPEND CONFIGURE_FLAGS --enable-static=yes)
    list(APPEND CONFIGURE_FLAGS --enable-shared=no)
    list(APPEND CONFIGURE_FLAGS ${CONFIGURE_EXTRA_FLAGS})

    # Configure
    execute_process(
        COMMAND ${gmp_SOURCE_DIR}/configure ${CONFIGURE_FLAGS}
        WORKING_DIRECTORY ${gmp_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    # Build
    execute_process(
        COMMAND make ${BUILD_EXTRA_ARGS}
        WORKING_DIRECTORY ${gmp_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    # Install
    execute_process(
        COMMAND make install
        WORKING_DIRECTORY ${gmp_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp=6.2.1)
    message("gmp library ver: ${gmp_VERSION}")
    message("gmp include dir: ${gmp_INCLUDE_DIRS}")
    message("gmp library dir: ${gmp_LIBRARY_DIRS}")
endif()
