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

include(ProcessorCount)
ProcessorCount(CPU_CNT)
if(CPU_CNT EQUAL 0)
    message(FATAL_ERROR " unable to get processor count")
endif()

set(BUILD_THREADS -j${CPU_CNT})
set(BUILD_EXTRA_ARGS ${BUILD_THREADS})

# configure flags common to all targets - install prefix & sysroot for cross-compilation
set(CONFIGURE_EXTRA_FLAGS)
# FIXME: why is ${INSTALL} not visible here?
list(APPEND CONFIGURE_EXTRA_FLAGS --prefix=${CMAKE_BINARY_DIR}/3rdparty)
if (CMAKE_CROSSCOMPILING)
    list(APPEND CONFIGURE_EXTRA_FLAGS --host=aarch64-intel-linux-gnu)
    list(APPEND CONFIGURE_EXTRA_FLAGS --build=x86_64-build_pc-linux-gnu)
    list(APPEND CONFIGURE_EXTRA_FLAGS --with-sysroot=$ENV{SDKTARGETSYSROOT})
endif()
