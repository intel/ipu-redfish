/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @copyright Copyright (c) 2019 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file entry_code.hpp
 */

#pragma once

#include <cstdint>
#include <cstring>
#include <ostream>
#include <string>
#include <vector>

namespace agent_framework {
namespace model {
namespace enums {

/*!
 * @brief EntryCode enum for Log Entry
 * */
class EntryCode {
public:
    using underlying_type = std::uint32_t;

    /*!
     * @brief Enum constructor. See LogEntry.v1_0_0.EntryCode in metadata for reference
     * */

    enum EntryCodeEnum : std::uint32_t {
        Assert,
        Deassert,
        LowerNonCriticalGoingLow,
        LowerNonCriticalGoingHigh,
        LowerCriticalGoingLow,
        LowerCriticalGoingHigh,
        LowerNonRecoverableGoingLow,
        LowerNonRecoverableGoingHigh,
        UpperNonCriticalGoingLow,
        UpperNonCriticalGoingHigh,
        UpperCriticalGoingLow,
        UpperCriticalGoingHigh,
        UpperNonRecoverableGoingLow,
        UpperNonRecoverableGoingHigh,
        TransitionToIdle,
        TransitionToActive,
        TransitionToBusy,
        StateDeasserted,
        StateAsserted,
        PredictiveFailureDeasserted,
        PredictiveFailureAsserted,
        LimitNotExceeded,
        LimitExceeded,
        PerformanceMet,
        PerformanceLags,
        TransitionToOK,
        TransitionToNonCriticalFromOK,
        TransitionToCriticalFromLessSevere,
        TransitionToNonRecoverableFromLessSevere,
        TransitionToNonCriticalFromMoreSevere,
        TransitionToCriticalFromNonRecoverable,
        TransitionToNonRecoverable,
        Monitor,
        Informational,
        DeviceRemovedOrDeviceAbsent,
        DeviceInsertedOrDevicePresent,
        DeviceDisabled,
        DeviceEnabled,
        TransitionToRunning,
        TransitionToInTest,
        TransitionToPowerOff,
        TransitionToOnLine,
        TransitionToOffLine,
        TransitionToOffDuty,
        TransitionToDegraded,
        TransitionToPowerSave,
        InstallError,
        FullyRedundant,
        RedundancyLost,
        RedundancyDegraded,
        NonRedundantSufficientResourcesFromRedundant,
        NonRedundantSufficientResourcesFromInsufficientResources,
        NonRedundantInsufficientResources,
        RedundancyDegradedFromFullyRedundant,
        RedundancyDegradedFromNonRedundant,
        D0PowerState,
        D1PowerState,
        D2PowerState,
        D3PowerState,
        OEM
    };

    /*!
     * @brief to string
     *
     * @return pointer to an array of chars
     * */
    const char* to_string() const;

    /*!
     * @brief From string
     * @param string Reference to a string that is going to be processed into enum
     * @return EntryCode object
     * */
    static EntryCode from_string(const std::string& string);

    /*!
     * @brief Gets all available Enum values as a vector of strings.
     * @return Returns std::vector of std::string containing Enum values.
     * */
    static std::vector<std::string> get_values();

    /*!
     * @brief Default constructor
     * */
    EntryCode() = delete;

    /*!
     * @brief Constructor
     * @param value uint32_t value of enum
     * */
    constexpr EntryCode(EntryCodeEnum value) : m_value(value) {}

    /*!
     * @brief Operation of getting enums
     * @return Enum
     * */
    constexpr operator EntryCodeEnum() const {
        return static_cast<EntryCodeEnum>(m_value);
    }

    /*!
     * @brief Is string an allowable value of this enum
     * @param string Value to be checked
     * @return true if the string is a valid value of enum, false otherwise
     * */
    static bool is_allowable_value(const std::string& string);

    friend std::ostream& operator<<(std::ostream& out_stream, const EntryCode& in_enum) {
        out_stream << in_enum.to_string();
        return out_stream;
    }
private:
    std::uint32_t m_value{};
};
} // namespace enums
} // namespace model
} // namespace agent_framework
