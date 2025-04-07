# <license_header>
#
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation
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

FetchContent_Declare(safestringlib
    URL "https://github.com/intel/safestringlib/archive/refs/tags/v1.2.0.tar.gz"
    URL_HASH MD5=b48ee0caac2fd8e1c5ec24504c29fb9d
)

FetchContent_MakeAvailable(safestringlib)

target_compile_options(safestring_objlib PRIVATE -w)
target_compile_options(safestring_static PRIVATE -w)
target_compile_options(safestring_shared PRIVATE -w)
