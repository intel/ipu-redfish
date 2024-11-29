# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

if (NOT ENABLE_TESTS)
    return()
endif()

add_gtest(generic generic
    singleton_test.cpp
)

