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

FetchContent_Declare(nlohmann_json
    URL "https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz"
    URL_HASH MD5=127794b2c82c0c5693805feaa2a703e2
)

FetchContent_Declare(nlohmann_json_schema
    URL "https://github.com/pboettch/json-schema-validator/archive/refs/tags/2.3.0.tar.gz"
    URL_HASH MD5=a6fbc02a604988c1f75139c28cd768f8
)

FetchContent_MakeAvailable(nlohmann_json)
FetchContent_MakeAvailable(nlohmann_json_schema)

target_compile_options(nlohmann_json_schema_validator PRIVATE -w)
