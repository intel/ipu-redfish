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
                     {Component::Chassis, std::make_shared<Handler<Chassis>>()},
                     {Component::Drive, std::make_shared<Handler<Drive>>()},
                     {Component::LogEntry, std::make_shared<Handler<LogEntry>>()},
                     {Component::LogService, std::make_shared<Handler<LogService>>()},
                     {Component::VirtualMedia, std::make_shared<Handler<VirtualMedia>>()},
                     {Component::Manager, std::make_shared<Handler<Manager>>()},
                     {Component::Memory, std::make_shared<Handler<Memory>>()},
                     {Component::Metric, std::make_shared<Handler<Metric>>()},
                     {Component::MetricDefinition, std::make_shared<Handler<MetricDefinition>>()},
                     {Component::NetworkDevice, std::make_shared<Handler<NetworkDevice>>()},
                     {Component::NetworkDeviceFunction, std::make_shared<Handler<NetworkDeviceFunction>>()},
                     {Component::NetworkInterface, std::make_shared<Handler<NetworkInterface>>()},
                     {Component::PcieDevice, std::make_shared<Handler<PcieDevice>>()},
                     {Component::PcieFunction, std::make_shared<Handler<PcieFunction>>()},
                     {Component::Processor, std::make_shared<Handler<Processor>>()},
                     {Component::StorageSubsystem, std::make_shared<Handler<StorageSubsystem>>()},
                     {Component::System, std::make_shared<Handler<System>>()},
                     {Component::Task, std::make_shared<Handler<Task>>()},
                 },
                 m_hierarchy{{Component::Chassis, {Component::Drive, Component::PcieDevice, Component::NetworkDevice}}, {Component::LogService, {Component::LogEntry}}, {Component::Manager, {Component::LogService, Component::NetworkInterface}}, {Component::NetworkDevice, {Component::NetworkDeviceFunction}}, {Component::System, {Component::Memory, Component::NetworkDevice, Component::NetworkInterface, Component::Processor, Component::StorageSubsystem}}} {
    // instantiate IdPolicy needed by ipu library to ensure successful linking
    // TODO: find a new owner owner for IdPolicies. Should it be ResourceHandlers?
    using namespace psme::rest::model::handler;
    IdPolicy<Component::System, NumberingZone::SHARED> system_id_policy;
    (void)system_id_policy;
}

Model::~Model() {
}
