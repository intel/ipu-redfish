/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright
 * Copyright (c) 2015-2019 Intel Corporation
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
 * */

#pragma once

#include "message_registry.hpp"
#include "message_registry_file.hpp"
#include "message_registry_file_collection.hpp"
#include "metadata.hpp"
#include "metadata_root.hpp"
#include "odata_service_document.hpp"
#include "psme/rest/endpoints/manager/manager.hpp"
#include "psme/rest/endpoints/manager/manager_collection.hpp"
#include "psme/rest/endpoints/manager/manager_reset.hpp"
#include "psme/rest/endpoints/task_service/monitor.hpp"
#include "psme/rest/endpoints/task_service/task.hpp"
#include "psme/rest/endpoints/task_service/task_collection.hpp"
#include "psme/rest/endpoints/task_service/task_service.hpp"
#include "redfish.hpp"
#include "root.hpp"
#include "session.hpp"
#include "session_collection.hpp"
#include "session_service.hpp"
#include "simple_update.hpp"
#include "simple_update_action_info.hpp"
#include "update_service.hpp"

#include "system/system.hpp"
#include "system/system_reset.hpp"
#include "system/systems_collection.hpp"
#include "system/virtual_media.hpp"
#include "system/virtual_media_collection.hpp"
#include "system/virtual_media_eject.hpp"
#include "system/virtual_media_insert.hpp"

#include "account_service/account.hpp"
#include "account_service/account_collection.hpp"
#include "account_service/account_service.hpp"
#include "account_service/role.hpp"
#include "account_service/role_collection.hpp"
