/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/ipu_constants.hpp"

namespace psme {
namespace ipu {
namespace constants {

const char* STATE = "State";
const char* BOOT_TYPE = "BootType";
const char* ERROR = "Error";
const char* IP_VERSION = "IpVersion";
const char* VPORT_ID = "VPortId";
const char* DOWNLOADED_IMAGE_NAME = "DownloadedImageName";
const char* IMAGE_TEMP_FOLDER = "/tmp/";
const char VIRTUAL_MEDIA_ENTITY_NAME[] = "VirtualMedia";
const uint64_t MAX_IMAGE_SIZE = 10'000'000'000L;
const uint64_t MAX_PLDM_IMAGE_SIZE = 1'500'000'000; // /tmp on IMC has 1.8G
const uint64_t CHUNK_SIZE = 500'000'000;            // Size of chunks to use when downloading the image from HTTP (500MB)

#if defined __aarch64__
const char* ACC_BOOT_OPTION_FILEPATH = "/mnt/imc/acc_variable/acc-uefi-boot-config.json";
const char* RESERVED_MEMORY_DIRECTORY = "/work/cfg/memory/";
const char* RESERVED_MEMORY_FILEPATH = "/work/cfg/memory/reserved-memory.json";
const char* IMAGE_FOLDER = "/mnt/imc/";
const char* IMAGE_PATH = "/mnt/imc/acc-os.iso";
const char* IMAGE_TEMP_PATH = "/tmp/acc-os.iso.tmp";
const char* IMAGE_SYMLINK_DIRECTORY = "/mnt/imc/acc/ramdisk/";
const char* IMAGE_SYMLINK = "/mnt/imc/acc/ramdisk/acc-os-image.bin";
const char* FIRMWARE_VERSION_FILEPATH = "/etc/issue.net";
const char* DESTINATION_PLDM_FILEPATH = "/work/image.pldm";
const char* PLDM_FILE_FOLDER = "/tmp/";
#else
const char* ACC_BOOT_OPTION_FILEPATH = "acc-uefi-boot-config.json";
const char* RESERVED_MEMORY_DIRECTORY = "/tmp/";
const char* RESERVED_MEMORY_FILEPATH = "/tmp/reserved-memory.json";
const char* IMAGE_FOLDER = "/tmp/";
const char* IMAGE_PATH = "/tmp/acc-os.iso";
const char* IMAGE_TEMP_PATH = "/tmp/acc-os.iso.tmp";
const char* IMAGE_SYMLINK_DIRECTORY = "/tmp/";
const char* IMAGE_SYMLINK = "/tmp/acc-os-image.bin";
const char* DESTINATION_PLDM_FILEPATH = "/work/image.pldm";
const char* PLDM_FILE_FOLDER = "/work/";
#endif

} // namespace constants
} // namespace ipu
} // namespace psme
