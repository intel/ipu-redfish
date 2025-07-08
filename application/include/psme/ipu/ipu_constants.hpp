/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/enum/compute.hpp"
#include "agent-framework/module/enum/enum_builder.hpp"

namespace psme {
namespace ipu {

namespace enums {
ENUM(State, uint32_t, ContinuousOverride, OneTimeOverride, OneTimeOverrideSynced, OneTimeOverrideUpdated);
ENUM(BootType, uint32_t, DramBoot, LocalIscsiBoot, LocalIscsiBoot_ISO, InternalUefiShell, Pxe, Http, Other);
ENUM(BootOption, uint32_t, ram, ramdisk, iscsi);
} // namespace enums

namespace constants {
extern const char* STATE;
extern const char* BOOT_TYPE;
extern const char* ERROR;
extern const char* IP_VERSION;
extern const char* VPORT_ID;
extern const char* NONE;
extern const char* ACTIVE_BOOT_OPTION;
extern const char* CURRENT_BOOT_OPTION;
extern const char* DESTINATION_PLDM_FILEPATH;
extern const char* PLDM_FILE_FOLDER;
extern const char* DOWNLOADED_IMAGE_NAME;
extern const char* ACC_BOOT_OVERRIDE_FILEPATH;
extern const char* ACC_BOOT_OPTION_FILEPATH;
extern const char* RESERVED_MEMORY_DIRECTORY;
extern const char* RESERVED_MEMORY_FILEPATH;
extern const char* IMAGE_FOLDER;
extern const char* IMAGE_TEMP_FOLDER;
extern const char* IMAGE_PATH;
extern const char* IMAGE_TEMP_PATH;
extern const char* IMAGE_SYMLINK_DIRECTORY;
extern const char* IMAGE_SYMLINK;
extern const char VIRTUAL_MEDIA_ENTITY_NAME[];
extern const char* FIRMWARE_VERSION_FILEPATH;
extern const uint64_t MAX_IMAGE_SIZE;
extern const uint64_t MAX_PLDM_IMAGE_SIZE;
extern const uint64_t CHUNK_SIZE;

} // namespace constants
} // namespace ipu
} // namespace psme
