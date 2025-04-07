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

FetchContent_Declare(gtest
    URL "https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz"
    URL_HASH MD5=7e11f6cfcf6498324ac82d567dcb891e
)

FetchContent_MakeAvailable(gtest)

target_compile_options(gtest PRIVATE -w)
target_compile_options(gtest_main PRIVATE -w)
target_compile_options(gmock PRIVATE -w)
target_compile_options(gmock_main PRIVATE -w)

# Helper function to create tests
# Input parameters:
# test_name
# associated_target
# source files (.cpp) list

# test_name gets renamed to unittest_associated_target_test_name
# followed by the usual add_executable / add_test sequence

# Sets a test_target variable which should be used by parent CMakeLists.txt instead.

function(add_gtest test_name associated_target)
    if (NOT TARGET ${associated_target})
        message(FATAL_ERROR "A non existing target ${associated_target} was specified!")
    endif()

    # Test target's name will be renamed so as to be able to quickly 
    # find them in the Makefile / `make help` output
    # and run unit tests associated with a specific target.
    # Yes, BOTH set's are required...
    set(test_target unittest_${associated_target}_${test_name})
    set(test_target ${test_target} PARENT_SCOPE)
    message("Adding test target ${test_target}")
    # append to a list of all targets that have unit tests defined
    # this list is used later to create unittest_targets.txt
    # that, in turn, is used in build_main.sh to build/run test targets
    set_property(GLOBAL APPEND APPEND_STRING PROPERTY unittest_all_targets ";${test_target}")

    set(test_sources ${ARGN})

    add_executable(${test_target} ${test_sources})
    target_link_libraries(${test_target} ${associated_target} gtest gtest_main gmock)

    if (TARGET code-coverage)
        add_dependencies(code-coverage ${test_target})
    endif()

    set_target_properties(${test_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
    )

    if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
        set_target_properties(${test_target} PROPERTIES
            COMPILE_FLAGS "-Wno-useless-cast -Wno-effc++ -Wno-inline -Wno-zero-as-null-pointer-constant -Wno-restrict"
        )
    endif()

    if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
            set_target_properties(${test_target} PROPERTIES
            COMPILE_FLAGS "-Wno-global-constructors"
        )
    endif()

    add_test(NAME ${test_target}
        COMMAND $<TARGET_FILE:${test_target}>
    )
endfunction()
