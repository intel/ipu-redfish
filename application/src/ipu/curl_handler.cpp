/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/curl_handler.hpp"
#include "agent-framework/module/enum/common.hpp"
#include "ipu/ipu_constants.hpp"
#include "psme/rest/server/certs/cert_loader.hpp"
#include <filesystem>

using namespace agent_framework::model::enums;
using namespace psme::ipu::constants;

namespace psme {
namespace ipu {

CurlHandler::CurlHandler() : m_curl_handle(curl_easy_init(), curl_easy_cleanup),
                             m_progress(0),
                             m_curl_file{} {
    if (!m_curl_handle) {
        throw std::runtime_error("Curl initialization failed.");
    }
    try_curl_setopt(CURLOPT_PROTOCOLS, CURLPROTO_HTTPS);
    try_curl_setopt(CURLOPT_FAILONERROR, 1L);
    try_curl_setopt(CURLOPT_CAINFO, psme::rest::server::CertLoader::CA_BUNDLE_PATH);
}

CurlHandler& CurlHandler::set_url(const std::string& url) {
    try_curl_setopt(CURLOPT_URL, url.c_str());
    return *this;
}

CurlHandler& CurlHandler::set_file_name(const std::string& file_name) {
    // if the file name starts with /mnt/imc - use "chunk" mode
    // - download it to /tmp using 500MB chunks and move to /mnt/imc/ as it is much faster
    m_chunk_mode = (file_name.find("/mnt/imc", 0) == 0);

    // check if there is enough space on /tmp
    if (m_chunk_mode) {
        std::filesystem::space_info si = std::filesystem::space(IMAGE_TEMP_FOLDER);
        if (si.available < CHUNK_SIZE) {
            throw std::runtime_error("There is not enough space left on " + std::string(IMAGE_TEMP_FOLDER));
        }
    }

    FILE* file = fopen((m_chunk_mode ? IMAGE_TEMP_PATH : file_name.c_str()), "wb+");
    if (!file) {
        throw std::runtime_error("Cannot open file " + file_name + " for writing.");
    }
    m_curl_file.handle.reset(file);

    // Set write_data callback
    if (m_chunk_mode) {
        try_curl_setopt(CURLOPT_WRITEDATA, static_cast<void*>(&m_curl_file));
        try_curl_setopt(CURLOPT_WRITEFUNCTION, write_data_callback);
    } else {
        // use default curl write_data() implementation - writing to /tmp this way is fast
        try_curl_setopt(CURLOPT_WRITEDATA, static_cast<void*>(m_curl_file.handle.get()));
        try_curl_setopt(CURLOPT_WRITEFUNCTION, nullptr);
    }

    return *this;
}

CurlHandler& CurlHandler::set_max_file_size(uint64_t file_size) {
    try_curl_setopt(CURLOPT_MAXFILESIZE_LARGE, file_size);
    return *this;
}

CurlHandler& CurlHandler::set_progress_report() {
    m_progress = 0;
    try_curl_setopt(CURLOPT_XFERINFODATA, static_cast<void*>(&m_progress));
    try_curl_setopt(CURLOPT_XFERINFOFUNCTION, progress_callback);
    try_curl_setopt(CURLOPT_NOPROGRESS, 0L);
    return *this;
}

CurlHandler& CurlHandler::set_credentials(const OptionalField<std::string>& username,
                                          const OptionalField<std::string>& password) {
    if (username.has_value()) {
        try_curl_setopt(CURLOPT_USERNAME, username.value().c_str());
        if (password.has_value()) {
            try_curl_setopt(CURLOPT_PASSWORD, password.value().c_str());
        }
    }
    return *this;
}

void CurlHandler::get_file_size() {
    // do the request without getting the body
    // needed to get file size without downloading it
    try_curl_setopt(CURLOPT_NOBODY, 1);
    perform_curl_request();

    curl_off_t file_size = 0;
    CURLcode res = curl_easy_getinfo(m_curl_handle.get(), CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &file_size);
    if (CURLE_OK != res) {
        log_debug("ipu", "Failed to get file size: " << curl_easy_strerror(res));
        throw std::runtime_error("Failed to get file size: " + std::string(curl_easy_strerror(res)));
    }
    log_debug("ipu", "File size: " << file_size);

    m_curl_file.size = file_size;
    m_curl_file.chunks = file_size / CHUNK_SIZE;
    m_curl_file.extra_bytes = file_size % CHUNK_SIZE;
    // re-enable request body
    try_curl_setopt(CURLOPT_NOBODY, 0);
}

void CurlHandler::perform_curl_request() {
    CURLcode res = curl_easy_perform(m_curl_handle.get());
    if (CURLE_OK != res) {
        log_debug("ipu", "Data transfer failed: " << curl_easy_strerror(res));
        throw std::runtime_error("Data transfer failed: " + std::string(curl_easy_strerror(res)));
    }
}

void CurlHandler::run_request() {
    if (m_chunk_mode) {
        // get requested file size first - needed for write_data_callback() to work properly
        get_file_size();
    }
    // download the file
    perform_curl_request();
}

size_t CurlHandler::progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow,
                                      curl_off_t ultotal, curl_off_t ulnow) {
    (void)ultotal;
    (void)ulnow;
    if (!dltotal || !clientp) {
        return 0;
    }
    size_t* progress = static_cast<size_t*>(clientp);
    size_t progress_now = dlnow * 100 / dltotal;
    if (*progress < progress_now && (0 == (progress_now % 10))) {
        *progress = progress_now;
        log_info("ipu", *progress << " % of image downloaded.");
    }
    return 0;
}

size_t CurlHandler::write_data_callback(void* data, size_t size, size_t nmemb, CurlFile* curl_file) {
    // TODO: experiment with setvbuf() here
    size_t bytes_written = fwrite(data, size, nmemb, curl_file->handle.get());
    if (bytes_written != size * nmemb) {
        log_error("ipu", "Failed to write to the file.");
        return 0;
    }
    curl_file->downloaded += bytes_written;

    // Check if chunk is full or if we're on the last chunk and it's full
    if (curl_file->downloaded >= CHUNK_SIZE * (curl_file->current_chunk + 1) ||
        ((curl_file->current_chunk == curl_file->chunks) && (curl_file->downloaded >= static_cast<size_t>(curl_file->size)))) {
        curl_file->current_chunk++;
        log_debug("ipu", "Moving chunk " << curl_file->current_chunk << " of the downloaded file to flash.");
        fflush(curl_file->handle.get());
        // Append temporary file contents to IMAGE_PATH
        // `cat` is much faster than all other methods explored (at least on IMC)
        std::string cmd = "/usr/bin/cat";
        cmd += " " + std::string(IMAGE_TEMP_PATH) + " >> " + std::string(IMAGE_PATH);
        int ret = std::system(cmd.c_str());
        if (ret != 0) {
            log_error("ipu", "Failed to run " << cmd);
        }
        // reset temporary file to 0
        rewind(curl_file->handle.get());
        ret = ftruncate(fileno(curl_file->handle.get()), 0);
        if (ret != 0) {
            log_error("ipu", "Failed to truncate input file.");
        }
    }

    return bytes_written;
}

} // namespace ipu
} // namespace psme
