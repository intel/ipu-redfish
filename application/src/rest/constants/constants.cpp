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

#include "psme/rest/constants/constants.hpp"

namespace psme {
namespace rest {
namespace constants {

namespace Metadata {
const char* METADATA_ROOT_FILE = "metadata_root.xml";
}

namespace PathParam {
const char* REDFISH = "redfish";
const char* METADATA_ROOT = "$metadata";
const char* METADATA = "metadata";
const char* BASE_URL = "/redfish/v1";
const char* BASE_URL_WITH_SLASH = "/redfish/v1/";
const char* METADATA_FILE = "metadataFile";
const char* SYSTEM_ID = "systemId";
const char* MANAGER_ID = "managerId";
const char* TASK_ID = "taskId";
const char* ODATA = "odata";
const char* MESSAGE_REGISTRY_FILE_ID = "messageRegistryFileId";
const char* SESSION_ID = "sessionId";
const char* ACCOUNT_ID = "accountId";
const char* ROLE_ID = "roleId";
const char* VIRTUAL_MEDIA_ID = "virtualMediaId";

const char PATH_SEP = '/';
const char VARIABLE_BEGIN = '{';
const char VARIABLE_END = '}';
const char REGEX_MARK = ':';
const char* ID_REGEX = "[0-9]+";
const char* XML_REGEX = ".*.xml";
const char* STRING_ID_REGEX = "[A-Za-z]+";

} // namespace PathParam

namespace Common {
const char* ODATA_CONTEXT = "@odata.context";
const char* ODATA_ID = "@odata.id";
const char* ODATA_TYPE = "@odata.type";
const char* OEM = "Oem";
const char* INTEL = "Intel";
const char* ID = "Id";
const char* MEMBER_ID = "MemberId";
const char* NAME = "Name";
const char* LINKS = "Links";
const char* MANUFACTURER = "Manufacturer";
const char* MODEL = "Model";
const char* SERIAL_NUMBER = "SerialNumber";
const char* PART_NUMBER = "PartNumber";
const char* ASSET_TAG = "AssetTag";
const char* STATUS = "Status";
const char* STATE = "State";
const char* HEALTH = "Health";
const char* HEALTH_ROLLUP = "HealthRollup";
const char* ENUM_STATUS = "EnumStatus";
const char* API_VERSION = "ApiVersion";
const char* LOCATION = "Location";
const char* CONTAINED_BY = "ContainedBy";
const char* MANAGED_BY = "ManagedBy";
const char* DESCRIPTION = "Description";
const char* VENDOR_ID = "VendorId";
const char* FIRMWARE_VERSION = "FirmwareVersion";
const char* UUID = "UUID";
const char* ACTIONS = "Actions";
const char* HASH = "#";
const char* RESET_TYPE = "ResetType";
const char* ALLOWABLE_RESET_TYPES = "ResetType@Redfish.AllowableValues";
const char* TARGET = "target";
const char* SKU = "SKU";
const char* RELATED_ITEM = "RelatedItem";
const char* MAC_ADDRESS = "MACAddress";
const char* REDUNDANCY = "Redundancy";
const char* IDENTIFIERS = "Identifiers";
const char* DURABLE_NAME = "DurableName";
const char* DURABLE_NAME_FORMAT = "DurableNameFormat";
const char* LUN = "LUN";
const char* ADMINISTRATOR = "Administrator";
const char* SERVICE_ENABLED = "ServiceEnabled";
const char* VIRTUAL_MEDIA = "VirtualMedia";
const char* VIRTUAL_MEDIA_INSERT = "VirtualMedia.InsertMedia";
const char* VIRTUAL_MEDIA_EJECT = "VirtualMedia.EjectMedia";
const char* ADDITIONAL_FIRMWARE_VERSIONS = "AdditionalFirmwareVersions";
} // namespace Common

namespace ErrorMessage {
const char* ERROR = "error";
const char* CODE = "code";
const char* MESSAGE = "message";
const char* EXTENDED_INFO = "@Message.ExtendedInfo";
} // namespace ErrorMessage

namespace MessageObject {
const char* JSON_POINTER_PREFIX = "#/";
const char* MESSAGE_ID = "MessageId";
const char* MESSAGE = "Message";
const char* MESSAGE_ARGS = "MessageArgs";
const char* RELATED_PROPERTIES = "RelatedProperties";
const char* SEVERITY = "Severity";
const char* RESOLUTION = "Resolution";
} // namespace MessageObject

namespace Inventory {
const char* BOOTLOADER = "Bootloader";
const char* IMC = "IMC";
const char* RECOVERY = "Recovery";
const char* OROM = "OROM";
const char* BOARD_ID = "BoardId";
} // namespace Inventory

namespace Collection {
const char* ODATA_COUNT = "Members@odata.count";
const char* MEMBERS = "Members";
} // namespace Collection

namespace Root {
const char* REDFISH_VERSION = "RedfishVersion";
const char* SESSION_SERVICE = "SessionService";
const char* TASKS = "Tasks";
const char* SYSTEMS = "Systems";
const char* MANAGERS = "Managers";
const char* REGISTRIES = "Registries";
const char* MESSAGE_REGISTRY = "MessageRegistry";
const char* UPDATE_SERVICE = "UpdateService";
const char* SERVICE = "Service";
const char* ACCOUNT_SERVICE = "AccountService";
} // namespace Root

namespace Redfish {
const char* V1 = "v1";
}

namespace OdataServiceDocument {
const char* VALUE = "value";
const char* NAME = "name";
const char* KIND = "kind";
const char* URL = "url";
} // namespace OdataServiceDocument

namespace Manager {
const char* MANAGER_TYPE = "ManagerType";
const char* POWER_STATE = "PowerState";
const char* DATE_TIME = "DateTime";
const char* DATE_TIME_LOCAL_OFFSET = "DateTimeLocalOffset";
const char* SERVICE_ENTRY_POINT_UUID = "ServiceEntryPointUUID";
const char* MANAGER_FOR_SERVERS = "ManagerForServers";
const char* HASH_MANAGER_RESET = "#Manager.Reset";
const char* MANAGER_RESET = "Manager.Reset";
} // namespace Manager

namespace ActionInfo {
const char* REDFISH_ACTION_INFO = "@Redfish.ActionInfo";
const char* PARAMETERS = "Parameters";
const char* REQUIRED = "Required";
const char* DATA_TYPE = "DataType";
const char* ALLOWABLE_VALUES = "AllowableValues";
} // namespace ActionInfo

namespace UpdateService {
const char* HTTP_PUSH_URI = "HttpPushUri";
const char* HASH_UPDATE_SERVICE_SIMPLE_UPDATE = "#UpdateService.SimpleUpdate";
const char* UPDATE_SERVICE_SIMPLE_UPDATE = "UpdateService.SimpleUpdate";
const char* SIMPLE_UPDATE_ACTION_INFO = "SimpleUpdateActionInfo";
const char* IMAGE_URI = "ImageURI";
const char* TRANSFER_PROTOCOL = "TransferProtocol";
const char* TARGETS = "Targets";
const char* USER_NAME = "Username";
const char* PASSWORD = "Password";
} // namespace UpdateService

namespace TaskService {
const char* TASK_SERVICE = "TaskService";
const char* DATETIME = "DateTime";
const char* COMPLETED_TASK_OVERWRITE_POLICY = "CompletedTaskOverWritePolicy";
const char* LIFE_CYCLE_EVENT_ON_TASK_STATE_CHANGE = "LifeCycleEventOnTaskStateChange";
const char* TASKS = "Tasks";
} // namespace TaskService

namespace Task {
const char* TASK_STATE = "TaskState";
const char* START_TIME = "StartTime";
const char* END_TIME = "EndTime";
const char* TASK_STATUS = "TaskStatus";
const char* MESSAGES = "Messages";
} // namespace Task

namespace Monitor {
const char* TASK_MONITORS = "TaskMonitors";
} // namespace Monitor

namespace MessageRegistryFile {
const char* LANGUAGES = "Languages";
const char* REGISTRY = "Registry";
const char* LOCATION = "Location";
const char* LANGUAGE = "Language";
const char* URI = "Uri";
const char* ARCHIVE_URI = "ArchiveUri";
const char* PUBLICATION_URI = "PublicationUri";
const char* ARCHIVE_FILE = "ArchiveFile";
} // namespace MessageRegistryFile

namespace MessageRegistry {
const char* LANGUAGE = "Language";
const char* MESSAGES = "Messages";
const char* OWNING_ENTITY = "OwningEntity";
const char* REGISTRY_PREFIX = "RegistryPrefix";
const char* REGISTRY_VERSION = "RegistryVersion";
} // namespace MessageRegistry

namespace SessionService {
const char* SESSION_TIMEOUT = "SessionTimeout";
const char* SESSIONS = "Sessions";
} // namespace SessionService

namespace Session {
const char* USER_NAME = "UserName";
const char* PASSWORD = "Password";
} // namespace Session

namespace AccountService {
const char* AUTH_FAILURE_LOGGING_THRESHOLD = "AuthFailureLoggingThreshold";
const char* MIN_PASSWORD_LENGTH = "MinPasswordLength";
const char* LOCAL_ACCOUNT_AUTH = "LocalAccountAuth";
const char* ACCOUNTS = "Accounts";
const char* ROLES = "Roles";
} // namespace AccountService

namespace Account {
const char* ENABLED = "Enabled";
const char* PASSWORD = "Password";
const char* USER_NAME = "UserName";
const char* ROLE_ID = "RoleId";
const char* LOCKED = "Locked";
const char* ROLE = "Role";
const char* ACCOUNT_TYPES = "AccountTypes";
const char* REDFISH = "Redfish";
} // namespace Account

namespace Role {
const char* IS_PREDEFINED = "IsPredefined";
const char* ASSIGNED_PRIVILEGES = "AssignedPrivileges";
const char* ROLE_ID = "RoleId";
} // namespace Role

namespace VirtualMedia {
const char* MEDIA_TYPES = "MediaTypes";
const char* IMAGE_NAME = "ImageName";
const char* INSERTED = "Inserted";
const char* HASH_VIRTUAL_MEDIA_INSERT = "#VirtualMedia.InsertMedia";
const char* HASH_VIRTUAL_MEDIA_EJECT = "#VirtualMedia.EjectMedia";
} // namespace VirtualMedia

namespace VirtualMediaInsert {
const char* IMAGE = "Image";
const char* TRANSFER_METHOD = "TransferMethod";
const char* USER_NAME = "UserName";
const char* PASSWORD = "Password";
} // namespace VirtualMediaInsert

} // namespace constants
} // namespace rest
} // namespace psme
