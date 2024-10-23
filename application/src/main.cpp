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
#include <filesystem>
#include <gcrypt.h>
#include <gnutls/gnutls.h>
#include <sys/stat.h>
#include <unistd.h>

#if defined __aarch64__
namespace {
namespace fs = std::filesystem;

/*!
 * @brief Checks whether the specified folder has the required permissions
 * @return true in case of success, false in case of failure
 */
bool check_folder_permissions(const std::string& dir_path) {
    if (!fs::exists(dir_path)) {
        log_error("app", "The directory " + dir_path + " does not exist");
        return false;
    }

    fs::perms perm = fs::status(dir_path).permissions();
    if ((perm & fs::perms::owner_all) != fs::perms::owner_all) {
        log_error("app", "The required owner permissions are not set for " + dir_path);
        return false;
    }
    if ((perm & (fs::perms::group_all | fs::perms::others_all)) != fs::perms::none) {
        log_error("app", "The permissions for " + dir_path + " are too open!");
        return false;
    }
    return true;
}
} // namespace
#endif

/*!
 * @brief PSME REST application main method
 * */
int main(int argc, const char* argv[]) {
    umask(0077);
#if defined __aarch64__
    if (getuid() != 0) {
        log_error("app", "This application must be run as root");
        return 1;
    }
    if (!check_folder_permissions("/work/redfish") ||
        !check_folder_permissions("/work/redfish/certs")) {
        return 1;
    }
    if (!gnutls_fips140_mode_enabled()) {
        log_error("app", "FIPS mode is not enabled. Make sure to run the IPU Redfish server as a service!");
        return 1;
    }
#endif
    if (!gcry_check_version(nullptr)) {
        log_error("app", "libgcrypt initialization failed");
        return 1;
    }
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

    psme::app::App app(argc, argv);
    app.run();
    return 0;
}
