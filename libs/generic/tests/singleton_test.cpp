/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "generic/singleton.hpp"
#include "gtest/gtest.h"

namespace generic {

class TestClass : public Singleton<TestClass> {
public:
    void set_value(int val) { value = val; }
    int get_value() { return value; }
private:
    int value = 0;
};

TEST(SingletonTest, InstanceUnique) {
    auto instance1 = TestClass::get_instance();
    auto instance2 = TestClass::get_instance();
    EXPECT_EQ(instance1, instance2);
}

TEST(SingletonTest, InstanceStatePreserved) {
    auto instance1 = TestClass::get_instance();
    instance1->set_value(42);
    auto instance2 = TestClass::get_instance();
    EXPECT_EQ(instance2->get_value(), 42);
}

} // namespace generic
