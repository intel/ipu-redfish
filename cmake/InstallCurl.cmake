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

FetchContent_Declare(curl
    URL "https://curl.haxx.se/download/curl-8.11.1.tar.gz"
    URL_HASH MD5=8eed752aeeb8ee54063b75baf95d3e14
)

pkg_check_modules(curl IMPORTED_TARGET libcurl=8.11.1)
if(curl_FOUND)
    message("curl library ver: ${curl_VERSION}")
    message("curl include dir: ${curl_INCLUDE_DIRS}")
    message("curl library dir: ${curl_LIBRARY_DIRS}")
endif()

if(NOT curl_POPULATED AND NOT curl_FOUND)
    message("Building cURL...")
    FetchContent_Populate(curl)

    set(CONFIGURE_FLAGS)
    list(APPEND CONFIGURE_FLAGS --enable-static=yes)
    list(APPEND CONFIGURE_FLAGS --enable-shared=no)
    list(APPEND CONFIGURE_FLAGS --enable-http)
    list(APPEND CONFIGURE_FLAGS --enable-ipv6)
    list(APPEND CONFIGURE_FLAGS --enable-proxy)
    list(APPEND CONFIGURE_FLAGS --disable-ftp)
    list(APPEND CONFIGURE_FLAGS --disable-file)
    list(APPEND CONFIGURE_FLAGS --disable-ldap)
    list(APPEND CONFIGURE_FLAGS --disable-ldaps)
    list(APPEND CONFIGURE_FLAGS --disable-rtsp)
    list(APPEND CONFIGURE_FLAGS --disable-dict)
    list(APPEND CONFIGURE_FLAGS --disable-telnet)
    list(APPEND CONFIGURE_FLAGS --disable-tftp)
    list(APPEND CONFIGURE_FLAGS --disable-pop3)
    list(APPEND CONFIGURE_FLAGS --disable-imap)
    list(APPEND CONFIGURE_FLAGS --disable-smb)
    list(APPEND CONFIGURE_FLAGS --disable-smtp)
    list(APPEND CONFIGURE_FLAGS --disable-gopher)
    list(APPEND CONFIGURE_FLAGS --disable-manual)
    list(APPEND CONFIGURE_FLAGS --disable-mqtt)
    list(APPEND CONFIGURE_FLAGS --without-libidn2)
    list(APPEND CONFIGURE_FLAGS --with-gnutls)
    list(APPEND CONFIGURE_FLAGS --without-libpsl)
    list(APPEND CONFIGURE_FLAGS --without-libgsasl)
    list(APPEND CONFIGURE_FLAGS --disable-libcurl-option)
    list(APPEND CONFIGURE_FLAGS --without-nghttp2)
    list(APPEND CONFIGURE_FLAGS --disable-docs)
    list(APPEND CONFIGURE_FLAGS --prefix=${CUSTOM_INSTALL_PREFIX})

    # Configure
    execute_process(
        COMMAND ${curl_SOURCE_DIR}/configure ${CONFIGURE_FLAGS} --cache-file=../configure.cache
        WORKING_DIRECTORY ${curl_BINARY_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )

    # Build
    execute_process(
        COMMAND make ${BUILD_THREADS} install
        WORKING_DIRECTORY ${curl_BINARY_DIR}
        OUTPUT_QUIET
    )

    pkg_check_modules(curl REQUIRED IMPORTED_TARGET libcurl=8.11.1)
    message("curl library ver: ${curl_VERSION}")
    message("curl include dir: ${curl_INCLUDE_DIRS}")
    message("curl library dir: ${curl_LIBRARY_DIRS}")
endif()
