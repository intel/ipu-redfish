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

cmake_minimum_required(VERSION 3.20)
message(STATUS "Current cmake is ${CMAKE_VERSION}")

project("PSME FW Build" C CXX)

if("$ENV{MACHINE}" STREQUAL "aarch64" OR "$ENV{ARCH}" STREQUAL "arm64" )
    message("Setting CMAKE_CROSSCOMPILING")
    set(CMAKE_CROSSCOMPILING 1)
endif()

option(RUN_CLANG_FORMAT "Run clang-format" ON)

# adding unified cmake tools
include(FetchContent)

option(FETCHCONTENT_NETWORK_ACCESS "Enable FetchContent to pull dependencies using Git" ON)

if(FETCHCONTENT_NETWORK_ACCESS)
  FetchContent_Declare(
    cmake_utilities
    GIT_REPOSITORY https://github.com/intel-innersource/libraries.devops.cmake-utilities
    GIT_TAG main
  )
else()
  FetchContent_Declare(
    cmake_utilities
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake/utilities
  )
endif()

FetchContent_MakeAvailable(cmake_utilities)
list(APPEND CMAKE_MODULE_PATH ${cmake_utilities_SOURCE_DIR})
include(CommonComponents)
###############################################################################
string(TIMESTAMP BUILD_TIME "%H:%M:%S, %d-%m-%Y")

# Target folder defaults
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Limit libraries search path to <build>/3rdparty - exclude standard system paths (/usr, /lib etc)
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/3rdparty")

message("CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")

if(CMAKE_CROSSCOMPILING)
    set(CMAKE_C_COMPILER_WORKS 1)
    set(CMAKE_CXX_COMPILER_WORKS 1)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        message("ccache found - using it...")
        set(ENV{CC} "ccache aarch64-intel-linux-gnu-gcc --sysroot=$ENV{SDKTARGETSYSROOT}")
        set(ENV{CXX} "ccache aarch64-intel-linux-gnu-g++ --sysroot=$ENV{SDKTARGETSYSROOT}")
        set(ENV{CPP} "ccache aarch64-intel-linux-gnu-gcc -E --sysroot=$ENV{SDKTARGETSYSROOT}")
    else()
        set(ENV{CC} "aarch64-intel-linux-gnu-gcc --sysroot=$ENV{SDKTARGETSYSROOT}")
        set(ENV{CXX} "aarch64-intel-linux-gnu-g++ --sysroot=$ENV{SDKTARGETSYSROOT}")
        set(ENV{CPP} "aarch64-intel-linux-gnu-gcc -E --sysroot=$ENV{SDKTARGETSYSROOT}")
    endif()
endif()

message("C Compiler works: ${CMAKE_C_COMPILER_WORKS}")
message("Cross-compiling: ${CMAKE_CROSSCOMPILING}")
message("C compiler is : $ENV{CC}")
message("C++ compiler is : $ENV{CXX}")
message("Preprocessor is : $ENV{CPP}")

# Compiler & linker flags for autotools-based 3rd party components
# CMake-based components use flags set by AddGnuCompiler.cmake / AddClangCompiler.cmake
set(ENV{CXXFLAGS} "-fPIE -fPIC -fstack-protector-strong -O3 -fdata-sections -ffunction-sections -s -I${CMAKE_BINARY_DIR}/3rdparty/include")
set(ENV{CFLAGS} "-fPIE -fPIC -fstack-protector-strong -O3 -fdata-sections -ffunction-sections -s -I${CMAKE_BINARY_DIR}/3rdparty/include")
set(ENV{CPPFLAGS} "-D_FORTIFY_SOURCE=2 -DNDEBUG")
set(ENV{LDFLAGS} "-z noexecstack -z relro -z now -pie -Wl,--gc-sections -s -L${CMAKE_BINARY_DIR}/3rdparty/lib -L${CMAKE_BINARY_DIR}/3rdparty/lib64")

# Path to folder with all cmake scripts
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Setup compilers
include(AddGnuCompiler)
include(AddClangCompiler)

# Add code coverage
include(AddCodeCoverage)

# Add documentation
include(AddDocumentation)

# 3rd party components build parameters
include(ConfigurationPackage)

option(ENABLE_HTTPS "Enable HTTPS support" ON)

find_package(PkgConfig REQUIRED)
find_package(PackageHandleStandardArgs REQUIRED)

set(INSTALL ${CMAKE_BINARY_DIR}/3rdparty)
set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
set(CUSTOM_INSTALL_PREFIX ${INSTALL})

#FIXME: use find_library() instead
link_directories(${INSTALL}/lib ${INSTALL}/lib64)
include_directories(${INSTALL}/include)

list(APPEND CMAKE_PREFIX_PATH ${INSTALL})

set(ENV{PKG_CONFIG_PATH} "${INSTALL}/lib/pkgconfig:${INSTALL}/lib64/pkgconfig:$ENV{PKG_CONFIG_PATH}")
unset(ENV{PKG_CONFIG_SYSROOT_DIR})

# Find packages built by us or provided by the system
include(InstallGmp)
include(InstallNettle)
include(InstallGnuTLS)
include(InstallGPG-error)
include(InstallGCrypt)

FetchMevCpchnl()
FetchResult()
FetchDcql()
FetchNlohmannJson()
FetchNlohmannJsonSchema()
FetchIPUUpdate()
FetchSafestringlib()

# IPU SDK already has libmicrohttpd
if(NOT CMAKE_CROSSCOMPILING)
    FetchMicrohttpd()
endif()
FetchCurl()

target_compile_options(nlohmann_json_schema_validator PRIVATE -w)
# Disable warnings in Intel components
# TODO: attempt to fix them
target_compile_options(safestring_objlib PRIVATE -w)
target_compile_options(safestring_static PRIVATE -w)
target_compile_options(safestring_shared PRIVATE -w)
target_compile_options(result PRIVATE -w)
target_compile_options(dcql PRIVATE -w)
target_compile_options(dcqlxx PRIVATE -w)
target_compile_options(sec_c PRIVATE -w)
target_compile_options(linkedlist PRIVATE -w)
target_compile_options(pldm_shared_code PRIVATE -w)
target_compile_options(ipuupdate_library PRIVATE -w)
target_compile_options(ipu-update-file PRIVATE -w)
target_compile_options(ifut_pldm PRIVATE -w)
target_compile_options(ifut_resources PRIVATE -w)
target_compile_options(ifut_version_device PRIVATE -w)
if(NOT CMAKE_CROSSCOMPILING)
target_compile_options(fake_dcql PRIVATE -w)
endif()

# Enable unit testing
if(CMAKE_CROSSCOMPILING)
    # empty stub to avoid changin every cmakelists.txt that uses it
    function(add_gtest test_name associated_target)
    endfunction()
else()
    include(InstallGoogleTest)
    include(GoogleTest)
    enable_testing()
    set(ENABLE_TESTS TRUE)
    set(GTEST_LIBRARIES gtest)
endif()

find_package(ZLIB)

if(NOT CMAKE_CROSSCOMPILING)
    # These libraries don't provide find_package config - but have .pc files
    pkg_check_modules(libzstd IMPORTED_TARGET libzstd)
    pkg_check_modules(libbrotlidec IMPORTED_TARGET libbrotlidec)
endif()

###############################################################################
message(STATUS "Building sources...")
add_definitions($ENV{OPTIONS})

add_subdirectory(libs)
add_subdirectory(application)
add_subdirectory(encrypter)

# create unittest_targets.txt with test targets names
# used by build_main.sh to build & run unit tests
if (ENABLE_TESTS)
    get_property(ut_targets GLOBAL PROPERTY unittest_all_targets)
    list(REMOVE_DUPLICATES ut_targets)
    file(WRITE ${CMAKE_BINARY_DIR}/unittest_targets.txt " ")
    foreach(ut_target ${ut_targets})
        file(APPEND ${CMAKE_BINARY_DIR}/unittest_targets.txt " ${ut_target}")
    endforeach()
endif()

if(RUN_CLANG_FORMAT)
  find_program(CLANG_FORMAT "clang-format")
  if(CLANG_FORMAT)
    file(GLOB_RECURSE FILES_TO_FORMAT
      ${PROJECT_SOURCE_DIR}/application/*.hpp
      ${PROJECT_SOURCE_DIR}/application/*.cpp
      ${PROJECT_SOURCE_DIR}/libs/*.hpp
      ${PROJECT_SOURCE_DIR}/libs/*.cpp
      ${PROJECT_SOURCE_DIR}/libs/*.h
      ${PROJECT_SOURCE_DIR}/libs/*.c
    )
    add_custom_target(
      format ALL
      COMMAND clang-format
      -i
      --style=file
      ${FILES_TO_FORMAT}
    )
  else()
    message(WARNING "Clang-format not found, do not commit changes without running the formatter!")
  endif()
endif()
