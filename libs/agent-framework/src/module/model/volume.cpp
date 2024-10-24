/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2017-2019 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file volume.cpp
 * @brief Volume model implementation
 * */

#include "agent-framework/module/model/volume.hpp"
#include "agent-framework/module/managers/utils/manager_utils.hpp"

using namespace agent_framework::model;
using namespace agent_framework::model::utils;

const enums::Component Volume::component = enums::Component::Volume;

Volume::Volume(const std::string& parent_uuid, enums::Component parent_type) : Resource(parent_uuid, parent_type) {}

Volume::~Volume() {}

bool Volume::is_volume_snapshot(const Uuid& volume_uuid) {
    return Volume::is_volume_snapshot(module::get_manager<Volume>().get_entry(volume_uuid));
}

bool Volume::is_volume_snapshot(const agent_framework::model::Volume& volume) {
    auto replica_infos = volume.get_replica_infos();
    return std::any_of(replica_infos.begin(), replica_infos.end(), [](const attribute::ReplicaInfo& info) {
        return (
            info.get_replica_type() == enums::ReplicaType::Snapshot &&
            info.get_replica_role() == enums::ReplicaRole::Target);
    });
}

bool Volume::is_source_for_another_volume(const Volume& volume) {
    const auto& replica_infos = volume.get_replica_infos();
    for (const auto& replica_info : replica_infos) {
        if (replica_info.get_replica_role() == enums::ReplicaRole::Source &&
            replica_info.get_replica_type() == enums::ReplicaType::Snapshot) {
            return true;
        }
    }
    return false;
}
