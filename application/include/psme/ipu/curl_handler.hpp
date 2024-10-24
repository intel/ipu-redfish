/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/utils/optional_field.hpp"
#include "curl/curl.h"
#include "logger/logger.hpp"
#include <cstdio>
#include <memory>
#include <string>

namespace psme {
namespace ipu {

/*!
 * @brief Structure to keep download state
 */
struct CurlFile {
    std::unique_ptr<FILE, std::function<int(FILE*)>> handle; // file we are writing to
    curl_off_t size = 0;                                     // its size
    size_t chunks = 0;                                       // number of chunks the file is broken into (calculated as `size / CHUNK_SIZE`)
    size_t current_chunk = 0;                                // chunk we are currently processing
    size_t extra_bytes = 0;                                  // calculated as `size % CHUNK_SIZE` - non-zero for files that have sizes not multiple of CHUNK_SIZE
    size_t downloaded = 0;                                   // bytes downloaded so far
    CurlFile() : handle(nullptr, [](FILE* file) { if(file) fclose(file); return 0; }) {}
};

/*!
 * @brief Curl Handler declaration
 */
class CurlHandler {
public:
    CurlHandler();
    ~CurlHandler() = default;
    CurlHandler& set_url(const std::string& url);
    CurlHandler& set_file_name(const std::string& file);
    CurlHandler& set_max_file_size(uint64_t file_size);
    CurlHandler& set_progress_report();
    CurlHandler& set_credentials(const OptionalField<std::string>& username,
                                 const OptionalField<std::string>& password);
    void run_request();
private:
    void get_file_size();
    void perform_curl_request();

    template <typename ParamT>
    void try_curl_setopt(CURLoption opt, ParamT param);
    static size_t progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow,
                                    curl_off_t ultotal, curl_off_t ulnow);
    static size_t write_data_callback(void* data, size_t size, size_t nmemb, CurlFile* curl_file);

    std::unique_ptr<CURL, void (*)(CURL*)> m_curl_handle;
    size_t m_progress;
    CurlFile m_curl_file;
    bool m_chunk_mode = false;
};

template <typename ParamT>
void CurlHandler::try_curl_setopt(CURLoption opt, ParamT param) {
    auto code = curl_easy_setopt(m_curl_handle.get(), opt, param);
    if (code != CURLE_OK) {
        log_error("ipu", "Curl setopt error: " << curl_easy_strerror(code));
        throw std::runtime_error("Curl setopt error: " + std::string(curl_easy_strerror(code)));
    }
}

} // namespace ipu
} // namespace psme
