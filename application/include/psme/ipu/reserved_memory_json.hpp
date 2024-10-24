/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include <string>

namespace psme {
namespace ipu {
namespace constants {

const std::string RESERVED_MEMORY = R"(
{
    "bmd_content_version":1,
    "reserved_memory": {
        "acc":{
            "_comment":"Expand ACC primary memory in region 1 to 14 GB",
            "base":"0x4400000000",
            "size":14,
            "granule":"GB",
            "sync_type":3
        },
        "acc_ramdisk": {
            "_comment":"Expand RAMDISK memory in region 1 to accommodate larger ISO boot images [up to 13760 MB], while leaving at least 256 MB of free memory in ACC primary memory",
            "base":"0x4414000000",
            "size":13760,
            "granule":"MB",
            "sync_type":3
        },
        "acc_ext1": {
            "_comment":"Set ACC EXT1 memory to 0 to free up memory in region 1",
            "base":"0x4680000000",
            "size":0,
            "granule":"MB",
            "sync_type":3
        },
        "acc_ext2": {
            "_comment":"Assign ACC EXT2 memory to spare location in region 2",
            "base":"0x4825000000",
            "size":48,
            "granule":"MB",
            "sync_type":3
        },
        "rdma": {
            "_comment":"Shrink RDMA memory from 4GB to 16MB and move to spare location in region 2 to free up memory in region 1",
            "base":"0x4813000000",
            "size":16,
            "granule":"MB",
            "sync_type":1
        }
    }
})";

} // namespace constants
} // namespace ipu
} // namespace psme
