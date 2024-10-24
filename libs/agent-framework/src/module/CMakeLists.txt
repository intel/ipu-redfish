# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

# <license_header>
#
# Copyright (c) 2015-2019 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# </license_header>

set(SOURCES
    model/attributes/array.cpp
    model/attributes/cpu_id.cpp
    model/attributes/fru_info.cpp
    model/attributes/identifier.cpp
    model/attributes/ipv4_address.cpp
    model/attributes/ipv6_address.cpp
    model/attributes/location.cpp
    model/attributes/message.cpp
    model/attributes/network_service.cpp
    model/attributes/pci_device.cpp
    model/attributes/graphical_console.cpp
    model/attributes/serial_console.cpp
    model/attributes/command_shell.cpp
    model/attributes/status.cpp
    model/attributes/usb_device.cpp
    model/attributes/connection_data.cpp
    model/attributes/iscsi_boot.cpp
    model/attributes/memory_location.cpp
    model/attributes/region.cpp
    model/attributes/wildcard.cpp
    model/attributes/calculation_parameters.cpp
    model/attributes/capacity.cpp
    model/attributes/capacity_source.cpp
    model/attributes/replica_info.cpp
    model/attributes/security_capabilities.cpp
    model/attributes/power_management_policy.cpp

    model/authorization_certificate.cpp
    model/boot_option.cpp
    model/chassis.cpp
    model/drive.cpp
    model/manager.cpp
    model/memory.cpp
    model/metric.cpp
    model/metric_definition.cpp
    model/network_interface.cpp
    model/network_device_function.cpp
    model/network_device.cpp
    model/pcie_device.cpp
    model/pcie_function.cpp
    model/processor.cpp
    model/resource.cpp
    model/storage_controller.cpp
    model/storage_subsystem.cpp
    model/system.cpp
    model/task.cpp
    model/trusted_module.cpp
    model/volume.cpp
    model/log_service.cpp
    model/log_entry.cpp
    model/virtual_media.cpp

    enum/processor_instruction_set.cpp
    enum/sensor_type.cpp
    enum/entry_code.cpp

    managers/utils/manager_utils.cpp

    compute_components.cpp
    storage_components.cpp
    chassis_components.cpp
    common_components.cpp

    utils/json_converter.cpp
    utils/time.cpp
    utils/is_ascii_string.cpp
    utils/iso8601_time_interval.cpp

    # TODO Should be moved somewhere
    service_uuid.cpp
    )

add_library(agent-framework-module STATIC
    ${SOURCES}
)

target_link_libraries(agent-framework-module
    PUBLIC
    agent-framework-exceptions
    logger

    PRIVATE
    configuration
    database
    uuid
)

target_include_directories(agent-framework-module
    PUBLIC
    ${AGENT_FRAMEWORK_DIR}/include
)
