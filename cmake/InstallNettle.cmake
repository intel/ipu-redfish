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

# Important: Nettle will only build libhogweed if libgmp headers & library is found
# Make sure GMP is built before Nettle by including this file after InstallGmp.cmake

FetchContent_Declare(nettle
    URL "https://ftp.gnu.org/gnu/nettle/nettle-3.9.1.tar.gz"
    URL_HASH MD5=29fcd2dec6bf5b48e5e3ffb3cbc4779e
)

pkg_check_modules(nettle IMPORTED_TARGET nettle=3.9.1)
if(nettle_FOUND)
    message("nettle library ver: ${nettle_VERSION}")
    message("nettle include dir: ${nettle_INCLUDE_DIRS}")
    message("nettle library dir: ${nettle_LIBRARY_DIRS}")
endif()

if(NOT nettle_POPULATED AND NOT nettle_FOUND)
    message("Building nettle...")
    FetchContent_Populate(nettle)

    set(CONFIGURE_FLAGS)
    list(APPEND CONFIGURE_FLAGS --disable-openssl)
    list(APPEND CONFIGURE_FLAGS --disable-shared)
    list(APPEND CONFIGURE_FLAGS --disable-documentation)
    list(APPEND CONFIGURE_FLAGS ${CONFIGURE_EXTRA_FLAGS})

    # Configure
    execute_process(
        COMMAND ${nettle_SOURCE_DIR}/configure ${CONFIGURE_FLAGS} --cache-file=../configure.cache
        WORKING_DIRECTORY ${nettle_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    # generated Makefile is broken - it assumes desdata target is built
    execute_process(
        COMMAND make desdata ${BUILD_EXTRA_ARGS}
        WORKING_DIRECTORY ${nettle_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    # Install
    execute_process(
        COMMAND make install ${BUILD_EXTRA_ARGS}
        WORKING_DIRECTORY ${nettle_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    pkg_check_modules(nettle REQUIRED IMPORTED_TARGET nettle=3.9.1)
    message("nettle library ver: ${nettle_VERSION}")
    message("nettle include dir: ${nettle_INCLUDE_DIRS}")
    message("nettle library dir: ${nettle_LIBRARY_DIRS}")
endif()
