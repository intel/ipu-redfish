/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "psme/rest/endpoints/endpoint_base.hpp"

namespace psme {
namespace rest {
namespace endpoint {

/*!
 * A class representing the rest api VirtualMediaCollection endpoint
 */
class VirtualMediaCollection : public EndpointBase {
public:
    /*!
     * @brief The constructor for VirtualMediaCollection class
     */
    explicit VirtualMediaCollection(const std::string& path);

    /*!
     * @brief VirtualMediaCollection class destructor
     */
    virtual ~VirtualMediaCollection();

    void get(const server::Request& request, server::Response& response) override;
};

} // namespace endpoint
} // namespace rest
} // namespace psme
