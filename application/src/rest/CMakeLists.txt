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

add_subdirectory(metadata)

set(SOURCES
    server/certs/cert_loader.cpp
    server/certs/cert_manager.cpp
    server/certs/file_cert_loader.cpp

    server/mux/empty_matcher.cpp
    server/mux/segment_matcher.cpp
    server/mux/regex_matcher.cpp
    server/mux/static_matcher.cpp
    server/mux/variable_matcher.cpp
    server/mux/utils.cpp

    server/status.cpp
    server/response.cpp
    server/request.cpp
    server/parameters.cpp
    server/multiplexer.cpp
    server/methods_handler.cpp
    server/content_types.cpp
    server/http_headers.cpp
    server/utils.cpp

    server/error/error_factory.cpp
    server/error/server_error.cpp
    server/error/server_exception.cpp
    server/error/message_object.cpp

    server/connector/connector.cpp
    server/connector/connector_options.cpp
    server/connector/connector_options_loader.cpp
    server/connector/microhttpd/mhd_connector_options.cpp
    server/connector/microhttpd/mhd_connector.cpp
    rest_server.cpp

    security/account/account.cpp
    security/account/account_manager.cpp
    security/account/role.cpp
    security/account/role_manager.cpp

    security/authentication/authentication_factory.cpp
    security/authentication/basic_authentication.cpp
    security/authentication/client_cert_authentication.cpp
    security/authentication/session_authentication.cpp

    security/session/session.cpp
    security/session/session_manager.cpp
    security/session/session_service.cpp
    security/session/session_service_manager.cpp

    constants/constants.cpp
    constants/constants_templates.cpp
    constants/system.cpp
    constants/routes.cpp

    endpoints/endpoint_base.cpp
    endpoints/redfish.cpp
    endpoints/root.cpp
    endpoints/odata_service_document.cpp
    endpoints/metadata_root.cpp
    endpoints/metadata.cpp
    endpoints/utils.cpp
    endpoints/task_service/task_service_utils.cpp
    endpoints/path_builder.cpp
    endpoints/manager/manager_collection.cpp
    endpoints/manager/manager.cpp
    endpoints/manager/manager_reset.cpp
    endpoints/update_service.cpp
    endpoints/simple_update.cpp
    endpoints/simple_update_action_info.cpp
    endpoints/session_service.cpp
    endpoints/session_collection.cpp
    endpoints/session.cpp
    endpoints/task_service/task_service.cpp
    endpoints/task_service/task_collection.cpp
    endpoints/task_service/task.cpp
    endpoints/message_registry_file_collection.cpp
    endpoints/message_registry_file.cpp
    endpoints/message_registry.cpp
    endpoints/task_service/monitor.cpp
    endpoints/task_service/monitor_content_builder.cpp

    endpoints/system/systems_collection.cpp
    endpoints/system/system.cpp
    endpoints/system/system_reset.cpp
    endpoints/system/virtual_media.cpp
    endpoints/system/virtual_media_collection.cpp
    endpoints/system/virtual_media_insert.cpp
    endpoints/system/virtual_media_eject.cpp

    endpoints/account_service/account_service.cpp
    endpoints/account_service/account.cpp
    endpoints/account_service/account_collection.cpp
    endpoints/account_service/role.cpp
    endpoints/account_service/role_collection.cpp

    endpoints/endpoint_builder.cpp

    model/handlers/id_memoizer.cpp
    model/resource_handler.cpp
    model/model.cpp

    registries/config/registry_configurator.cpp
    registries/managers/message_registry_file_manager.cpp
    registries/managers/message_registry_manager.cpp
    registries/model/attributes/location.cpp
    registries/model/message_registry_file.cpp
    registries/model/message_registry.cpp

    utils/time_utils.cpp

    validators/json_validator.cpp
    validators/schemas/common.cpp
    validators/schemas/session_collection.cpp
    validators/schemas/session_service.cpp
    validators/schemas/system.cpp
    validators/schemas/reset.cpp
    validators/schemas/simple_update.cpp
    validators/schemas/virtual_media_insert.cpp
)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set_source_files_properties(
        server/error/error_factory.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-suggest-attribute=format"
    )
    set_source_files_properties(
        utils/time_utils.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-format-nonliteral"
    )
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    set_source_files_properties(
        endpoints/simple_update.cpp
        COMPILE_FLAGS "-Wno-disabled-macro-expansion"
    )
    set_source_files_properties(
        server/error/error_factory.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-format-nonliteral"
    )
endif()

add_library(application-rest STATIC ${SOURCES})

# Since app also uses generated metadata, it should wait for the headers
# to be generated
add_dependencies(application-rest metadata_generate_headers)

if(IPU)
    target_compile_definitions(application-rest PUBLIC INTEL_IPU)
endif()

target_link_libraries(application-rest
    PRIVATE
    agent-framework-discovery
    net
    generic
)
