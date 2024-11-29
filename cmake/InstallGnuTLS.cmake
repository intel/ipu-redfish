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

FetchContent_Declare(gnutls
    URL "https://www.gnupg.org/ftp/gcrypt/gnutls/v3.8/gnutls-3.8.3.tar.xz"
    URL_HASH MD5=269966167fa5bf8bae5f7534bcc3c454
)

pkg_check_modules(gnutls IMPORTED_TARGET gnutls=3.8.3)
if(gnutls_FOUND)
    message("gnutls library ver: ${gnutls_VERSION}")
    message("gnutls include dir: ${gnutls_INCLUDE_DIRS}")
    message("gnutls library dir: ${gnutls_LIBRARY_DIRS}")
endif()

if(NOT gnutls_POPULATED AND NOT gnutls_FOUND)
    message("Building GnuTLS...")
    FetchContent_Populate(gnutls)

    unset(ENV{PKG_CONFIG_SYSROOT_DIR})

    set(CONFIGURE_FLAGS)
    list(APPEND CONFIGURE_FLAGS --disable-doc)
    list(APPEND CONFIGURE_FLAGS --disable-tests)
    list(APPEND CONFIGURE_FLAGS --disable-tools)
    list(APPEND CONFIGURE_FLAGS --with-included-libtasn1)
    list(APPEND CONFIGURE_FLAGS --with-included-unistring)
    list(APPEND CONFIGURE_FLAGS --without-p11-kit)
    list(APPEND CONFIGURE_FLAGS --without-tpm)
    list(APPEND CONFIGURE_FLAGS --with-zstd)
    list(APPEND CONFIGURE_FLAGS --without-brotli)
    list(APPEND CONFIGURE_FLAGS --without-idn)
    list(APPEND CONFIGURE_FLAGS --disable-hardware-acceleration)
    list(APPEND CONFIGURE_FLAGS --enable-static=yes)
    list(APPEND CONFIGURE_FLAGS --enable-shared=no)
    list(APPEND CONFIGURE_FLAGS ${CONFIGURE_EXTRA_FLAGS})

    # Configure
    execute_process(
        COMMAND ${gnutls_SOURCE_DIR}/configure ${CONFIGURE_FLAGS} --cache-file=../configure.cache
        WORKING_DIRECTORY ${gnutls_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    # Build & install
    execute_process(
        COMMAND make install ${BUILD_EXTRA_ARGS}
        WORKING_DIRECTORY ${gnutls_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    # gnutls library requires gmp & nettle - but does not list them in the 'Libs' section of gnutls.pc
    # pkg-config based builds (curl, microhttpd) thus fail to link gnutls at configure stage and assume no TLS support
    # Fix this by explicitly listing all the libraries gnutls needs and fixing gnutls.pc after it is built.
    execute_process(
        COMMAND bash -c "sed -i 's|-lgnutls|-lgnutls -lnettle -lhogweed -lgmp -lz -lnettle|g' gnutls.pc"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rdparty/lib/pkgconfig
        OUTPUT_QUIET
        ERROR_QUIET
    )

    pkg_check_modules(gnutls REQUIRED IMPORTED_TARGET gnutls=3.8.3)
    message("gnutls library ver: ${gnutls_VERSION}")
    message("gnutls include dir: ${gnutls_INCLUDE_DIRS}")
    message("gnutls library dir: ${gnutls_LIBRARY_DIRS}")
endif()
