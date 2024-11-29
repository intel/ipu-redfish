/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2015-2019 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * */

#include "app.hpp"
#include <iostream>
#include <sys/stat.h>

/*!
 * @brief PSME REST application main method
 * */
int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Failed to start server: no configuration file provided." << std::endl;
        return -1;
    }
    umask(0077);
    psme::app::App app(argv[1]);
    app.run();
    return 0;
}
