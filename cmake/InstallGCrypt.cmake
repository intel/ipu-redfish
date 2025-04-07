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

FetchContent_Declare(libgcrypt
    URL "https://www.gnupg.org/ftp/gcrypt/libgcrypt/libgcrypt-1.10.0.tar.bz2"
    URL_HASH MD5=4faebf2c262af56ba4acfd68dd32440c
)

pkg_check_modules(libgcrypt IMPORTED_TARGET libgcrypt>=1.10.0)
if(libgcrypt_FOUND)
    message("libgcrypt library ver: ${libgcrypt_VERSION}")
    message("libgcrypt include dir: ${libgcrypt_INCLUDE_DIRS}")
    message("libgcrypt library dir: ${libgcrypt_LIBRARY_DIRS}")
endif()

if(NOT libgcrypt_POPULATED AND NOT libgcrypt_FOUND)
    message("Building libgcrypt...")
    FetchContent_Populate(libgcrypt)

    set(CONFIGURE_FLAGS)
    list(APPEND CONFIGURE_FLAGS --disable-asm)
    list(APPEND CONFIGURE_FLAGS --enable-static=yes)
    list(APPEND CONFIGURE_FLAGS --enable-shared=no)
    list(APPEND CONFIGURE_FLAGS --with-libgpg-error-prefix=${INSTALL})
    list(APPEND CONFIGURE_FLAGS ${CONFIGURE_EXTRA_FLAGS})

    # Configure
    execute_process(
        COMMAND ${libgcrypt_SOURCE_DIR}/configure ${CONFIGURE_FLAGS} --cache-file=../configure.cache
        WORKING_DIRECTORY ${libgcrypt_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    # Build & install
    execute_process(
        COMMAND make ${BUILD_EXTRA_ARGS} install
        WORKING_DIRECTORY ${libgcrypt_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    pkg_check_modules(libgcrypt REQUIRED IMPORTED_TARGET libgcrypt=1.10.0)
    message("libgcrypt library ver: ${libgcrypt_VERSION}")
    message("libgcrypt include dir: ${libgcrypt_INCLUDE_DIRS}")
    message("libgcrypt library dir: ${libgcrypt_LIBRARY_DIRS}")
endif()
