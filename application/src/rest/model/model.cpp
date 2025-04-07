/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "psme/rest/model/model.hpp"
#include "psme/rest/model/handler.hpp"
#include "psme/rest/model/handlers/id_policy.hpp"

#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/model/model_common.hpp"
#include "agent-framework/module/model/model_compute.hpp"

using namespace psme::rest;
using namespace agent_framework::model;

Model::Model() : m_handlers{
                     {Component::VirtualMedia, std::make_shared<Handler<VirtualMedia>>()},
                     {Component::Manager, std::make_shared<Handler<Manager>>()},
                     {Component::System, std::make_shared<Handler<System>>()},
                     {Component::Task, std::make_shared<Handler<Task>>()},
                 },
                 m_hierarchy{{Component::System, {Component::VirtualMedia}}} {
    // instantiate IdPolicy needed by ipu library to ensure successful linking
    // TODO: find a new owner owner for IdPolicies. Should it be ResourceHandlers?
    using namespace psme::rest::model::handler;
    IdPolicy<Component::System, NumberingZone::SHARED> system_id_policy;
    (void)system_id_policy;
}

Model::~Model() {
}
