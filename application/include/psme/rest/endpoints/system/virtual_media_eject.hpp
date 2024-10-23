/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "psme/rest/endpoints/endpoint_base.hpp"

namespace psme {
namespace rest {
namespace endpoint {

/*!
 * A class representing the rest api VirtualMediaEject endpoint
 */
class VirtualMediaEject : public EndpointBase {
public:
    /*!
     * @brief The constructor for VirtualMediaEject class
     */
    explicit VirtualMediaEject(const std::string& path);

    /*!
     * @brief VirtualMediaEject class destructor
     */
    virtual ~VirtualMediaEject();

    void post(const server::Request& request, server::Response& response) override;
};

} // namespace endpoint
} // namespace rest
} // namespace psme
