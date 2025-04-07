/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @brief General database interface
 *
 * @copyright Copyright (c) 2016-2019 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file file_database.cpp
 */

#include "database/file_database.hpp"

#include "generic/assertions.hpp"
#include "logger/logger.hpp"
#include "logger/logger_factory.hpp"

#include "safe_lib.h"

#include <cstring>
#include <filesystem>
#include <memory>
#include <mutex>

extern "C" {
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
}

using namespace generic;
using namespace database;

namespace {

/*! @brief Extension of db files */
static const std::string EXT = ".db";

/*! @brief "Even" number for maximal (serialized) value size */
constexpr static unsigned VALUE_LENGTH = 65536;

void fclose_conditional(FILE* file) {
    if (file) {
        fclose(file);
    }
}

} // namespace

std::recursive_mutex FileDatabase::mutex{};

FileDatabase::Locations FileDatabase::locations{};

FileDatabase::FileDatabase(const std::string& _name, bool with_policy, const std::string& location) : Database(_name), m_with_policy(with_policy), path(check_directory(location)), iterating_state(IteratingState::NOT_STARTED) {}

FileDatabase::~FileDatabase() {
    if (iterating_state != IteratingState::NOT_STARTED) {
        FileDatabase::end();
    }
}

bool FileDatabase::start() {
    /*! @todo Double locking on iterating state */
    std::lock_guard<std::recursive_mutex> lock(mutex);

    if (iterating_state != IteratingState::NOT_STARTED) {
        log_error("db", "Iterating in progress");
        assert(FAIL("In progress"));
        return false;
    }
    iterating_state = IteratingState::STARTED;
    return true;
}

bool FileDatabase::next(Serializable& key, Serializable& value) {
    std::lock_guard<std::recursive_mutex> lock(mutex);

    switch (iterating_state) {
    case IteratingState::NOT_STARTED:
        log_error("db", "Iterating not started");
        assert(FAIL("Not iterating"));
        return false;
    case IteratingState::STARTED:
        iterated_files.clear();
        foreach (key, [this](const std::string& stripped_name) -> bool {
            iterated_files.push_back(stripped_name);
            return true;
        })
            ;
        current_file = iterated_files.begin();
        iterating_state = IteratingState::ITERATE;
        break;
    case IteratingState::ITERATE:
        break;
    case IteratingState::NO_MORE_DATA:
        return false;
    default:
        assert(FAIL("Unreachable code"));
        return false;
    }

    while (iterated_files.end() != current_file) {
        const std::string& stripped_name = *current_file;
        current_file++;

        /* parse key, it was checked just before, so it is proper */
        key.unserialize(stripped_name);

        /* parse value, if not valid, then log error and check next one */
        std::string data{};
        if (!read_file(full_name(stripped_name), data)) {
            log_error("db", "Cannot read data for " << stripped_name);
            continue;
        } else if (!value.unserialize(data)) {
            log_error("db", "Incorrect data for " << stripped_name << ":: " << data);
            continue;
        }

        /* key/value are filled without any issue */
        return true;
    }
    /* no more data */
    iterating_state = IteratingState::NO_MORE_DATA;
    return false;
}

void FileDatabase::end() {
    std::lock_guard<std::recursive_mutex> lock(mutex);

    switch (iterating_state) {
    case IteratingState::NOT_STARTED:
        log_error("db", "Not iterating");
        assert(FAIL("Not iterating"));
        return;
    case IteratingState::STARTED:
        log_error("db", "Iterating started but not proceeded");
        assert(FAIL("Not iterating"));
        break;
    case IteratingState::ITERATE:
        log_warning("db", "Not all files were checked while iterating");
        break;
    case IteratingState::NO_MORE_DATA:
        break;

    default:
        assert(FAIL("Unreachable code"));
        return;
    }
    iterating_state = IteratingState::NOT_STARTED;
}

bool FileDatabase::get(const Serializable& key, Serializable& value) {
    std::lock_guard<std::recursive_mutex> lock(mutex);
    std::string data{};
    if (!read_file(full_name(key.serialize()), data)) {
        return false;
    }
    return value.unserialize(data);
}

bool FileDatabase::put(const Serializable& key, const Serializable& value) {
    std::lock_guard<std::recursive_mutex> lock(mutex);
    return save_file(full_name(key.serialize()), value.serialize(), m_with_policy);
}

bool FileDatabase::remove(const Serializable& key) {
    std::lock_guard<std::recursive_mutex> lock(mutex);
    return remove_file(full_name(key.serialize()));
}

bool FileDatabase::invalidate(const Serializable& key) {
    std::lock_guard<std::recursive_mutex> lock(mutex);
    return invalidate_file(full_name(key.serialize()));
}

Database::EntityValidity FileDatabase::get_validity(const Serializable& key, std::chrono::seconds interval) {
    std::lock_guard<std::recursive_mutex> lock(mutex);
    return file_validity(full_name(key.serialize()), interval, m_with_policy);
}

unsigned FileDatabase::cleanup(Serializable& key, std::chrono::seconds interval) {
    return foreach (key, [this, interval](const std::string& stripped_name) -> bool {
        std::string file_name = full_name(stripped_name);
        switch (file_validity(file_name, interval, m_with_policy)) {
        case EntityValidity::ERROR:
        case EntityValidity::NOENTRY:
        case EntityValidity::INVALID:
            return false;
        case EntityValidity::VALID:
            return invalidate_file(file_name);
        case EntityValidity::OUTDATED:
            return remove_file(file_name);
        default:
            assert(FAIL("Unreachable code"));
            return false;
        }
    });
}

unsigned FileDatabase::wipe_outdated(Serializable& key, std::chrono::seconds interval) {
    return foreach (key, [this, interval](const std::string& stripped_name) -> bool {
        std::string file_name = full_name(stripped_name);
        switch (file_validity(file_name, interval, m_with_policy)) {
        case EntityValidity::ERROR:
        case EntityValidity::NOENTRY:
        case EntityValidity::VALID:
        case EntityValidity::INVALID:
            return false;
        case EntityValidity::OUTDATED:
            return remove_file(file_name);
        default:
            assert(FAIL("Unreachable code"));
            return false;
        }
    });
}

unsigned FileDatabase::drop(Serializable& key) {
    return foreach (key, [this](const std::string& stripped_name) -> bool {
        std::string file_name = full_name(stripped_name);
        return remove_file(file_name);
    });
}

unsigned FileDatabase::foreach (Serializable& key, ForeachFunction function) {
    std::lock_guard<std::recursive_mutex> lock(mutex);

    DIR* directory = opendir(path.c_str());
    if (nullptr == directory) {
        log_error("db", "Cannot open directory " << path
                                                 << " for reading:: " << strerror(errno));
        return 0;
    }

    /* appoint all names to be processed */
    IteratedFiles names;
    while (nullptr != directory) {
        errno = 0;
        struct dirent* dir_entry = readdir(directory);

        /* no more entries in the directory */
        if (nullptr == dir_entry) {
            if (errno) {
                log_warning("db", "Reading directory " << path
                                                       << ":: " << strerror(errno));
            }
            closedir(directory);
            directory = nullptr;
            break;
        }

        /* only plain files contains DB entries */
        if (DT_REG != dir_entry->d_type) {
            continue;
        }

        std::string stripped_name = strip_name(dir_entry->d_name);
        /* file from other db.. or even non-db file */
        if (stripped_name.empty()) {
            continue;
        }

        /* parse key */
        if (!key.unserialize(stripped_name)) {
            continue;
        }
        names.push_back(std::move(stripped_name));
    }

    /* process all names */
    unsigned num = 0;
    for (IteratedFiles::const_iterator it = names.begin(); it != names.end(); it++) {
        if (function(*it)) {
            num++;
        }
    }
    return num;
}

std::string FileDatabase::full_name(const std::string& stripped_name) const {
    if (stripped_name.empty()) {
        return EMPTY;
    }
    /* stripped name cannot contain any special characters: no '/' and ':' allowed */
    if (stripped_name.find_first_of("/:") != std::string::npos) {
        return EMPTY;
    }
    std::string ret = path + "/";
    if (!get_name().empty()) {
        ret.append(get_name() + ".");
    }
    ret.append(stripped_name);
    ret.append(EXT);
    return ret;
}

std::string FileDatabase::strip_name(const std::string& d_name) const {
    /* there's no path in the name to strip */
    std::string stripped = d_name;

    if (!get_name().empty()) {
        /* strip database name */
        if (stripped.substr(0, get_name().length()) != get_name()) {
            return EMPTY;
        }
        stripped.erase(0, get_name().length());

        /* strip '.' */
        if (stripped.substr(0, 1) != ".") {
            return EMPTY;
        }
        stripped.erase(0, 1);
    }

    /* strip extension */
    if ((stripped.length() < EXT.length()) || (stripped.substr(stripped.length() - EXT.length()) != EXT)) {
        return EMPTY;
    }
    stripped.erase(stripped.length() - EXT.length());

    return stripped;
}

bool FileDatabase::read_file(const std::string& file_name, std::string& data) {
    if (file_name.empty()) {
        log_error("db", "No file name given");
        return false;
    }

    char buffer[VALUE_LENGTH];
    std::shared_ptr<FILE> file{fopen(file_name.c_str(), "rb"), fclose_conditional};
    if (!file) {
        log_debug("db", "Cannot read file " << file_name << ":: " << strerror(errno));
        return false;
    }

    size_t bytes = fread(&buffer, sizeof(char), sizeof(buffer), file.get());
    bool ok;
    if (ferror(file.get())) {
        log_error("db", "Cannot read file " << file_name << ":: " << strerror(errno));
        ok = false;
    } else if (!feof(file.get())) {
        log_error("db", "File " << file_name << " is longer than allowed");
        ok = false;
    } else {
        data = std::string(buffer, bytes);
        ok = true;
    }

    return ok;
}

bool FileDatabase::save_file(const std::string& file_name, const std::string& data, bool with_policy) {
    if (file_name.empty()) {
        log_error("db", "No file name given");
        return false;
    }

    std::shared_ptr<FILE> file{fopen(file_name.c_str(), "wb"), fclose_conditional};
    if (!file) {
        log_error("db", "Cannot write file " << file_name);
        return false;
    }
    int file_descriptor = fileno(file.get());

    if (!data.empty()) {
        size_t bytes = fwrite(data.data(), sizeof(char), data.length(), file.get());
        if (data.length() != bytes) {
            log_error("db", "Cannot write whole file " << file_name << ", only " << bytes << " written");
            return false;
        }
    }

    /* check current mode of the file */
    struct stat stats {};
    if (0 != fstat(file_descriptor, &stats)) {
        log_error("db", "Cannot set mode for " << file_name << ":: " << strerror(errno));
        return false;
    }
    /* check file mode flags: keep in mind W for group is "reserved" for retention policed entries! */
    const mode_t flags = (S_ISVTX | (with_policy ? S_IWGRP : 0));
    if ((stats.st_mode & (S_ISVTX | S_IWGRP)) == flags) {
        return true;
    }

    /* set sticky bit for the file, this alter stats.st_ctim as well */
    return 0 == fchmod(file_descriptor, (stats.st_mode & (~(S_ISVTX | S_IWGRP))) | flags);
}

bool FileDatabase::remove_file(const std::string& file_name) {
    if (file_name.empty()) {
        log_error("db", "No file name given");
        return false;
    }

    /* POSIX compatible.. */
    return (::remove(file_name.c_str()) == 0);
}

Database::EntityValidity FileDatabase::file_validity(const std::string& file_name, std::chrono::seconds interval, bool with_policy) {
    struct stat stats;
    errno = 0;
    if (0 != stat(file_name.c_str(), &stats)) {
        log_error("db", "Cannot check mode for " << file_name << ":: " << strerror(errno));
        return EntityValidity::ERROR;
    }

    /* if file is "created" by the database without retention policy assigned,
     * file is not considered as one to be touched by the database for retention policy.
     */
    if ((with_policy) && (S_IWGRP != (stats.st_mode & S_IWGRP))) {
        return EntityValidity::NOENTRY;
    }

    /* still marked as valid */
    if (S_ISVTX == (stats.st_mode & S_ISVTX)) {
        return EntityValidity::VALID;
    }

    /* If times match.. then file is outdated! */
    if (stats.st_ctim.tv_sec + interval.count() <= time(nullptr)) {
        return EntityValidity::OUTDATED;
    }

    return EntityValidity::INVALID;
}

bool FileDatabase::invalidate_file(const std::string& file_name) {
    if (file_name.empty()) {
        return false;
    }

    std::shared_ptr<FILE> file{fopen(file_name.c_str(), "r+b"), fclose_conditional};
    if (!file) {
        log_error("db", "Cannot read file" << file_name);
        return false;
    }
    int file_descriptor = fileno(file.get());

    /* check current mode of the file */
    struct stat stats;
    errno = 0;
    if (0 != fstat(file_descriptor, &stats)) {
        log_error("db", "Cannot check mode for " << file_name << ":: " << strerror(errno));
        return false;
    }
    /* already marked as invalid */
    if (S_ISVTX != (stats.st_mode & S_ISVTX)) {
        return false;
    }
    /* Remove sticky bit from the file, this alter stats.st_ctim as well.
     * All files are threated equally, retention policy does not change the behaviour.
     */
    return 0 == fchmod(file_descriptor, stats.st_mode & (~S_ISVTX));
}

/**
 * @brief Generates unique file path by creating temp file, then closing and removing it
 *
 * @param pathTmpl path template. The last six characters of template must be "XXXXXX" and these
       are replaced with a string that makes the filename unique
 * @param out output string
 * @return true on success
 */
bool FileDatabase::generate_unique_file_name(const std::string& pathTmpl, std::string& out) {
    constexpr uint bufSize = 128;
    char file_path[bufSize] = {};

    auto copiedCharCnt = pathTmpl.copy(file_path, bufSize - 1);
    if (copiedCharCnt != pathTmpl.size()) {
        log_warning("db", "Path " << pathTmpl << " has been trimmed on copy: " << file_path);
    }

    mode_t umaskOld = umask(S_IREAD | S_IWRITE);

    errno = 0;
    int fd = mkstemp(file_path);
    auto tmpErrno = errno;
    umask(umaskOld);
    if (fd < 0) {
        log_error("db", "Failed to create temp file " << file_path << ", error " << strerror(tmpErrno));
        return false;
    }

    errno = 0;
    int ret = close(fd);
    if (ret != 0) {
        log_error("db", "Failed to close file descriptor for " << file_path << ", error " << strerror(errno));
        return false;
    }

    errno = 0;
    ret = ::remove(file_path);
    if (ret != 0) {
        log_error("db", "Failed to remove temp file " << file_path << ", error " << strerror(errno));
        return false;
    }

    out = std::string(file_path);
    return true;
}

/**
 * @brief Checks if directory exists and tries to create it. Function shall not create full tree.
 *
 * @param dirPath directory path
 * @param mode directory permissions
 * @return true on success
 */
bool FileDatabase::mkdir_p(const std::string& dirPath, mode_t mode) {
    // create_directory() returns false if directory already existed - it doesn't matter to us,
    // so we don't check that.
    // It throws if path is a file or in case of errors.
    // permissions() doesn't return anyting - instead it throws on error.
    try {
        std::filesystem::path path(dirPath);
        std::filesystem::create_directory(path);
        std::filesystem::permissions(path, std::filesystem::perms(mode));
        return true;
    }
    catch (...) {
        return false;
    }
}

std::string FileDatabase::check_directory(const std::string& dir) {
    /* if directory already checked */
    if (locations.find(dir) != locations.end()) {
        return locations[dir];
    }

    const std::string defaultPath = "/tmp";
    std::string tmp = dir;
    bool success = true;

    if (tmp.empty()) {
        success = generate_unique_file_name(std::string("/tmp/database_XXXXXX"), tmp);
        if (!success) {
            log_error("db", "Cannot get tmp directory name: " << tmp);
            return defaultPath;
        }
    }

    log_debug("db", "Creating db directory at " << tmp);
    success = mkdir_p(tmp, 01777);
    if (!success) {
        log_error("db", "Cannot find or create directory: " << tmp);
        return defaultPath;
    }

    tmp.append("/check_XXXXXX");
    std::string file_path;
    success = generate_unique_file_name(tmp, file_path);
    if (!success) {
        log_error("db", "Cannot get tmp file name: " << tmp);
        return defaultPath;
    }

    file_path.erase(file_path.rfind('/'));
    locations[dir] = file_path;
    return file_path;
}
