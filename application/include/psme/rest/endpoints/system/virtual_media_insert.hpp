/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "psme/rest/endpoints/endpoint_base.hpp"

namespace psme {
namespace rest {
namespace endpoint {

/*!
 * A class representing the rest api VirtualMediaInsert endpoint
 */
class VirtualMediaInsert : public EndpointBase {
public:
    /*!
     * @brief The constructor for VirtualMediaInsert class
     */
    explicit VirtualMediaInsert(const std::string& path);

    /*!
     * @brief VirtualMediaInsert class destructor
     */
    virtual ~VirtualMediaInsert();

    void post(const server::Request& request, server::Response& response) override;
};

} // namespace endpoint
} // namespace rest
} // namespace psme
