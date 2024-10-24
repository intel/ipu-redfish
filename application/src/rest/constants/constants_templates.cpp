/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "psme/rest/constants/constants_templates.hpp"

#include "psme/rest/constants/constants.hpp"

namespace psme {
namespace rest {
namespace constants {

template <>
const char* get_resource_id<agent_framework::model::Memory>() {
    return PathParam::MEMORY_ID;
}

template <>
const char* get_resource_id<agent_framework::model::System>() {
    return PathParam::SYSTEM_ID;
}

template <>
const char* get_resource_id<agent_framework::model::Chassis>() {
    return PathParam::CHASSIS_ID;
}

template <>
const char* get_resource_id<agent_framework::model::Drive>() {
    return PathParam::DRIVE_ID;
}

template <>
const char* get_resource_id<agent_framework::model::StorageSubsystem>() {
    return PathParam::STORAGE_ID;
}

template <>
const char* get_resource_id<agent_framework::model::Manager>() {
    return PathParam::MANAGER_ID;
}

template <>
const char* get_resource_id<agent_framework::model::NetworkInterface>() {
    return PathParam::NIC_ID;
}

template <>
const char* get_resource_id<agent_framework::model::NetworkDevice>() {
    return PathParam::NETWORK_INTERFACE_ID;
}

template <>
const char* get_resource_id<agent_framework::model::Task>() {
    return PathParam::TASK_ID;
}

template <>
const char* get_resource_id<agent_framework::model::NetworkDeviceFunction>() {
    return PathParam::NETWORK_DEVICE_FUNCTION_ID;
}

template <>
const char* get_resource_id<agent_framework::model::MetricDefinition>() {
    return PathParam::METRIC_DEFINITION_ID;
}

template <>
const char* get_resource_id<agent_framework::model::Processor>() {
    return PathParam::PROCESSOR_ID;
}

template <>
const char* get_resource_id<agent_framework::model::PcieDevice>() {
    return PathParam::DEVICE_ID;
}

template <>
const char* get_resource_id<agent_framework::model::PcieFunction>() {
    return PathParam::FUNCTION_ID;
}

template <>
const char* get_resource_id<agent_framework::model::LogService>() {
    return PathParam::LOG_SERVICE_ID;
}

template <>
const char* get_resource_id<agent_framework::model::LogEntry>() {
    return PathParam::LOG_ENTRY_ID;
}

template <>
const char* get_resource_id<agent_framework::model::VirtualMedia>() {
    return PathParam::VIRTUAL_MEDIA_ID;
}

} // namespace constants
} // namespace rest
} // namespace psme
