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
 * @file cert_manager.cpp
 *
 * */

#include "psme/rest/server/certs/cert_manager.hpp"
#include "psme/rest/server/certs/file_cert_loader.hpp"

using namespace psme::rest::server;

CertManager* CertManager::get_instance() {
    static CertManager* g_instance = new CertManager{};
    return g_instance;
}

CertManager::~CertManager() {}

void CertManager::load_certs(const uint16_t server_port,
                             bool ca_cert_required,
                             const std::string& certs_dir) {
    FileCertLoader loader(certs_dir);
    const auto certs = loader.load_certs(ca_cert_required);
    add_certs(server_port, certs);
}

void CertManager::add_certs(const uint16_t server_port,
                            const Certs& certs) {
    m_certs_map.emplace(server_port, certs);
}

const char* CertManager::get_server_key(const uint16_t server_port) const {
    const auto& certs = m_certs_map.at(server_port);
    return std::get<SERVER_KEY>(certs).c_str();
}

const char* CertManager::get_server_cert(const uint16_t server_port) const {
    const auto& certs = m_certs_map.at(server_port);
    return std::get<SERVER_CERT>(certs).c_str();
}

const char* CertManager::get_ca_cert(const uint16_t server_port) const {
    const auto& certs = m_certs_map.at(server_port);
    return std::get<CA_CERT>(certs).c_str();
}
