/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include <memory>

namespace generic {

template <typename T>
class Singleton {
public:
    virtual ~Singleton() {}

    static std::shared_ptr<T> get_instance() {
        static std::shared_ptr<T> ptr = std::make_shared<T>();
        return ptr;
    }
protected:
    Singleton() = default;
private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

} // namespace generic
