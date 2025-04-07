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
 * @file connector_options.hpp
 *
 * @brief Declaration of Connector Options for Connector initialization.
 * */

#pragma once

#include "agent-framework/module/utils/optional_field.hpp"
#include "json-wrapper/json-wrapper.hpp"
#include <string>

namespace psme {
namespace rest {
namespace server {

/*!
 * @brief ConnectorOptions parsed from JSON configuration file
 * for Connector initialization.
 *
 * Provided implementation (MHDConnector) of Connector interface is based on
 * <a href="https://www.gnu.org/software/libmicrohttpd">Libmicrohttpd</a> library.
 * Please refer to its documentation for explanation of some options: #PORT,
 * #THREAD_MODE, #THREAD_POOL_SIZE, #DEBUG_MODE.
 **/
class ConnectorOptions {
public:
    /*! @brief Property name of restricted to interface to listen on */
    static constexpr const char RESTRICTED_TO_INTERFACE[] = "restricted-to-interface";
    /*! @brief Property name of port Connector should listen on */
    static constexpr const char PORT[] = "port";
    /*! @brief Property name of Path to directory containing files for ssl Connector */
    static constexpr const char CERTS_DIR[] = "certs-directory";
    /*! @brief Property name of flag indicating if client certificate is required */
    static constexpr const char CLIENT_CERT_REQUIRED[] = "client-cert-required";
    /*! @brief Property hostname for certificate verification */
    static constexpr const char HOSTNAME[] = "hostname";
    /*! @brief Property name of threading mode */
    static constexpr const char THREAD_MODE[] = "thread-mode";
    /*! @brief Value of threading mode property */
    static constexpr const char THREAD_MODE_SELECT[] = "select";
    /*! @brief Value of threading mode property */
    static constexpr const char THREAD_MODE_THREAD_PER_CONNECTION[] = "thread-per-connection";
    /*! @brief Property name of authentication type */
    static constexpr const char AUTHENTICATION_TYPE[] = "authentication-type";
    /*! @brief Value of authentication type property */
    static constexpr const char AUTHENTICATION_TYPE_NONE[] = "none";
    /*! @brief Value of authentication type property */
    static constexpr const char AUTHENTICATION_TYPE_BASIC[] = "basic";
    /*! @brief Value of authentication type property */
    static constexpr const char AUTHENTICATION_TYPE_REDFISH_SESSION[] = "session";
    /*! @brief Value of authentication type property */
    static constexpr const char AUTHENTICATION_TYPE_BASIC_OR_SESSION[] = "basic-or-session";
    /*! @brief Property name of thread pool size */
    static constexpr const char THREAD_POOL_SIZE[] = "thread-pool-size";
    /*! @brief Property name of flag indicating if debug mode should be enabled */
    static constexpr const char DEBUG_MODE[] = "debug-mode";

    /*! @brief Threading mode of connector */
    enum class ThreadMode {
        SELECT = 0,
        THREAD_PER_CONNECTION
    };

    enum class AuthenticationType {
        NONE = -1,
        BASIC_AUTH,
        REDFISH_SESSION_AUTH,
        BASIC_AUTH_OR_REDFISH_SESSION_AUTH
    };

    /*!
     * @brief Constructor.
     * @param connector_config JSON Object with connector options.
     * */
    explicit ConnectorOptions(const json::Json& connector_config);

    /*!
     * @brief Constructor.
     * */
    ConnectorOptions() = delete;

    /*!
     * @brief Destructor.
     */
    ~ConnectorOptions() = default;

    /*!
     * @brief Default copy Constructor.
     */
    ConnectorOptions(const ConnectorOptions&) = default;

    /*!
     * @brief Default assignment operator.
     */
    ConnectorOptions& operator=(const ConnectorOptions&) = default;

    /*!
     * @brief Default move Constructor.
     */
    ConnectorOptions(ConnectorOptions&&) = default;

    /*!
     * @brief Default move assignment operator.
     */
    ConnectorOptions& operator=(ConnectorOptions&&) = default;

    /*!
     * @brief Threading Mode the Connector should use.
     *
     * @return ThreadMode
     */
    ThreadMode get_thread_mode() const;

    /*!
     * @brief Authentication Type the Connector should use.
     *
     * @return AuthenticationType
     */
    AuthenticationType get_authentication_type() const;

    /*!
     * @return Port the Connector should listen on.
     */
    uint16_t get_port() const;

    /*!
     * @return Certificates directory location for HTTPS configuration.
     */
    const std::string& get_certs_dir() const;

    /*!
     * @return true if HTTPS Client is required to provide certificate.
     */
    bool is_client_cert_required() const;

    /*!
     * @return Hostname for certificate verification.
     */
    const std::string& get_hostname() const;

    /*!
     * @return Thread pool size of Connector.
     */
    unsigned int get_thread_pool_size() const;

    /*!
     * @return true if debug mode should be enabled for Connector.
     */
    bool use_debug() const;

    /*!
     * Getter for network interface name on which connector listens incoming requests
     * @return Optional network interface name
     */
    const OptionalField<std::string>& get_network_interface_name() const;
private:
    uint16_t m_port{443};
    std::string m_certs_dir{};
    bool m_is_client_cert_required{false};
    std::string m_hostname{};
    unsigned int m_thread_pool_size{0};
    ThreadMode m_thread_mode{ThreadMode::SELECT};
    AuthenticationType m_authentication_type{AuthenticationType::BASIC_AUTH};
    bool m_use_debug{false};
    OptionalField<std::string> m_network_interface_name{};
};

} // namespace server
} // namespace rest
} // namespace psme
