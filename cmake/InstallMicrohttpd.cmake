# <license_header>
#
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation
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

FetchContent_Declare(microhttpd
    URL "https://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-0.9.72.tar.gz"
    URL_HASH MD5=2a286193af88ab2ae3149748d4b46187
)

pkg_check_modules(microhttpd IMPORTED_TARGET libmicrohttpd=0.9.72)
if(microhttpd_FOUND)
    message("microhttpd library ver: ${microhttpd_VERSION}")
    message("microhttpd include dir: ${microhttpd_INCLUDE_DIRS}")
    message("microhttpd library dir: ${microhttpd_LIBRARY_DIRS}")
endif()

if(NOT microhttpd_POPULATED AND NOT microhttpd_FOUND)
    message("Building microhttpd...")
    FetchContent_Populate(microhttpd)

    set(CONFIGURE_FLAGS)
    list(APPEND CONFIGURE_FLAGS --enable-static=yes)
    list(APPEND CONFIGURE_FLAGS --enable-shared=no)
    list(APPEND CONFIGURE_FLAGS --enable-https=yes)
    list(APPEND CONFIGURE_FLAGS ${CONFIGURE_EXTRA_FLAGS})

    # Configure
    execute_process(
        COMMAND ${microhttpd_SOURCE_DIR}/configure ${CONFIGURE_FLAGS}
        WORKING_DIRECTORY ${microhttpd_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    # Build
    execute_process(
        COMMAND make ${BUILD_EXTRA_ARGS}
        WORKING_DIRECTORY ${microhttpd_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    # Install
    execute_process(
        COMMAND make install
        WORKING_DIRECTORY ${microhttpd_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    pkg_check_modules(microhttpd REQUIRED IMPORTED_TARGET libmicrohttpd=0.9.72)
    message("microhttpd library ver: ${microhttpd_VERSION}")
    message("microhttpd include dir: ${microhttpd_INCLUDE_DIRS}")
    message("microhttpd library dir: ${microhttpd_LIBRARY_DIRS}")
endif()
