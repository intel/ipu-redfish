# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

set(CPCHNL_CMD_HANDLER cpchnl_cmd_handler_stub.cpp)
set(IPU_UPDATE_HANDLER ipu_update_handler_stub.cpp)
if(IPU)
    set(CPCHNL_CMD_HANDLER cpchnl_cmd_handler.cpp)
    set(IPU_UPDATE_HANDLER ipu_update_handler.cpp)
endif()

add_library(ipu STATIC
    acc_boot_override_handler.cpp
    base_service.cpp
    ${CPCHNL_CMD_HANDLER}
    curl_handler.cpp
    firmware_build_getter.cpp
    imc_reset_handler.cpp
    ipu_constants.cpp
    ${IPU_UPDATE_HANDLER}
    loader.cpp
    simple_update_handler.cpp
    service.cpp
    virtual_media_eject_handler.cpp
    virtual_media_insert_handler.cpp
    )

target_include_directories(ipu
    PUBLIC
    ${PSME_APPLICATION_DIR}/include/psme
    PRIVATE
    # TODO: fix cyclic dependency
    ${PSME_APPLICATION_DIR}/include/psme/rest
    )

set(IPU_LINK_LIST)
list(APPEND IPU_LINK_LIST agent-framework)

if(IPU)
    list(APPEND IPU_LINK_LIST dcqlxx)
    list(APPEND IPU_LINK_LIST ipuupdate_library)
    target_compile_definitions(ipu PUBLIC INTEL_IPU)
endif()

target_link_libraries(
    ipu
    PRIVATE
    ${IPU_LINK_LIST}
    )


