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

FetchContent_Declare(gpg-error
    URL "https://www.gnupg.org/ftp/gcrypt/libgpg-error/libgpg-error-1.42.tar.bz2"
    URL_HASH MD5=133fed221ba8f63f5842858a1ff67cb3
)

pkg_check_modules(gpg-error IMPORTED_TARGET gpg-error=1.42)
if(gpg-error_FOUND)
    message("gpg-error library ver: ${gpg-error_VERSION}")
    message("gpg-error include dir: ${gpg-error_INCLUDE_DIRS}")
    message("gpg-error library dir: ${gpg-error_LIBRARY_DIRS}")
endif()

if(NOT gpg-error_POPULATED AND NOT gpg-error_FOUND)
    message("Building GPG-error...")
    FetchContent_Populate(gpg-error)

    set(CONFIGURE_FLAGS)
    list(APPEND CONFIGURE_FLAGS --enable-static=yes)
    list(APPEND CONFIGURE_FLAGS --enable-shared=no)
    list(APPEND CONFIGURE_FLAGS ${CONFIGURE_EXTRA_FLAGS})

    # Configure
    execute_process(
        COMMAND ${gpg-error_SOURCE_DIR}/configure ${CONFIGURE_FLAGS} --cache-file=../configure.cache
        WORKING_DIRECTORY ${gpg-error_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    # Build & install
    execute_process(
        COMMAND make install ${BUILD_EXTRA_ARGS}
        WORKING_DIRECTORY ${gpg-error_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    pkg_check_modules(gpg-error REQUIRED IMPORTED_TARGET gpg-error=1.42)
    message("gpg-error library ver: ${gpg-error_VERSION}")
    message("gpg-error include dir: ${gpg-error_INCLUDE_DIRS}")
    message("gpg-error library dir: ${gpg-error_LIBRARY_DIRS}")
endif()
