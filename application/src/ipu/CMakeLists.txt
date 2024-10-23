# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

set(CPCHNL_CMD_HANDLER cpchnl_cmd_handler_stub.cpp)
if(CMAKE_CROSSCOMPILING)
    set(CPCHNL_CMD_HANDLER cpchnl_cmd_handler.cpp)
endif()

add_library(ipu
    acc_boot_option_getter.cpp
    acc_boot_option_handler.cpp
    acc_boot_option_setter.cpp
    base_service.cpp
    ${CPCHNL_CMD_HANDLER}
    curl_handler.cpp
    firmware_build_getter.cpp
    imc_reset_handler.cpp
    ipu_constants.cpp
    ipu_update.cpp
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

target_link_libraries(
    ipu
    PRIVATE
    agent-framework
    dcqlxx
    ipuupdate_library
    )
