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
 *
 * @file mhd_connector.hpp
 *
 * @brief HTTP Connector implementation based on
 * <a href="https://www.gnu.org/software/libmicrohttpd">Libmicrohttpd</a>.
 * */

#pragma once

#include "psme/rest/server/connector/connector.hpp"

/*! forward declarations */
struct MHD_Daemon;
struct MHD_Connection;

namespace psme {
namespace rest {
namespace server {

/*!
 * @brief HTTP Connector implementation based on
 * <a href="https://www.gnu.org/software/libmicrohttpd">Libmicrohttpd</a>.
 * */
class MHDConnector : public Connector {
public:
    /*!
     * @brief Constructor
     * @param[in] options ConnectorOptions for Connector initialization.
     */
    MHDConnector(const ConnectorOptions& options);

    /*! @brief Destructor */
    ~MHDConnector();

    void start() override;

    void stop() override;
private:
    using MHDDaemonUPtr = std::unique_ptr<MHD_Daemon, void (*)(MHD_Daemon*)>;
    MHDDaemonUPtr m_daemon;

    MHDConnector(const MHDConnector&) = delete;

    MHDConnector& operator=(const MHDConnector&) = delete;
};

} // namespace server
} // namespace rest
} // namespace psme
