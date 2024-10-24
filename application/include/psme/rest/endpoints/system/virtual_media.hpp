/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "psme/rest/endpoints/endpoint_base.hpp"

namespace psme {
namespace rest {
namespace endpoint {

/*!
 * A class representing the rest api VirtualMedia endpoint
 */
class VirtualMedia : public EndpointBase {
public:
    /*!
     * @brief The constructor for VirtualMedia class
     */
    explicit VirtualMedia(const std::string& path);

    /*!
     * @brief VirtualMedia class destructor
     */
    virtual ~VirtualMedia();

    void get(const server::Request& request, server::Response& response) override;
};

} // namespace endpoint
} // namespace rest
} // namespace psme
