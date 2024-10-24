/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#include "ipu/cpchnl_cmd_handler.hpp"
#include "agent-framework/exceptions/gami_exception.hpp"

#include <dcqlxx.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic" // ISO C++ forbids zero-size array
#include <cpchnl2_platform.h>
#pragma GCC diagnostic pop

using namespace agent_framework::model;

namespace psme {
namespace ipu {

/*!
 * @brief Custom DCQL logger
 */
class DCQLLogDebug : public dcqlxx::Log {
public:
    dcqlxx::Log& operator<<(const std::string& str) {
        log_debug("ipu", "DCQL: " << str);
        return *this;
    }
};

class DCQLLogError : public dcqlxx::Log {
public:
    dcqlxx::Log& operator<<(const std::string& str) {
        log_error("ipu", "DCQL: " << str);
        return *this;
    }
};

class DcqlxxInitializer {
public:
    static dcqlxx::Dcqlxx& dcqlxx_init() {
        dcqlxx::Dcqlxx::set_deb_logger(std::make_unique<DCQLLogDebug>());
        dcqlxx::Dcqlxx::set_err_logger(std::make_unique<DCQLLogError>());
        dcqlxx::Dcqlxx::set_init_params(dcql::CQ_ID_CLI_MBX2, false, true, true, {.mask = DCQL_LOG_ALL});
        return dcqlxx::Dcqlxx::get();
    }
    DcqlxxInitializer() = delete;
};

CpchnlCmdHandler::CpchnlCmdHandler() : m_dcqlxx(DcqlxxInitializer::dcqlxx_init()) {
}

void CpchnlCmdHandler::trigger_acc_reset(enums::ResetType reset_type) {
    uint32_t cphnl_reset_type = 0;
    switch (reset_type) {
    case enums::ResetType::GracefulShutdown:
        cphnl_reset_type = CPCHNL2_ACC_RESET_REQUEST_SHUTDOWN;
        break;
    case enums::ResetType::GracefulRestart:
        cphnl_reset_type = CPCHNL2_ACC_RESET_REQUEST_WARM_RESET;
        break;
    default:
        // should not happen - allowed reset types are validated before calling this function
        throw std::runtime_error(std::string("Unsupported reset type: ") + reset_type.to_string());
    }

    try {
        dcqlxx::CpChnlCommand comm;
        comm.with_opcode(CPCHNL2_ACC_RESET_REQUEST)
            .with_param_0(cphnl_reset_type)
            .with_timeout_ms(1000);

        Result res = m_dcqlxx.sendrcv(comm);

        if (IsResultError(res)) {
            log_error("ipu", "Failed to send command or receive response: " << GetResultDescription(res));
            throw agent_framework::exceptions::GamiException(
                agent_framework::exceptions::ErrorCode::UNKNOWN_ERROR, // need a new error code
                GetResultDescription(res));                            // not sure if returning this string is a good idea
        }
    }
    catch (const std::runtime_error& ex) {
        log_error("ipu", "Error running DCQL command: " << ex.what());
        throw agent_framework::exceptions::GamiException(
            agent_framework::exceptions::ErrorCode::UNKNOWN_ERROR, // need a new error code
            ex.what());
    }
    catch (const std::exception& ex) {
        log_error("ipu", "Error running DCQL command: " << ex.what());
        throw agent_framework::exceptions::GamiException(
            agent_framework::exceptions::ErrorCode::UNKNOWN_ERROR, // need a new error code
            ex.what());
    }
}

} // namespace ipu
} // namespace psme
