/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "agent-framework/module/enum/compute.hpp"
#include "agent-framework/module/utils/optional_field.hpp"
#include "ipu/ipu_constants.hpp"

#include <optional>

namespace psme {
namespace ipu {

class AccBootOverrideHandler {
public:
    /*!
     * @brief Reads the boot override configuration and sets the view.
     * Intended to be called on ipu::Service startup, hence the name.
     * */
    void read_initial_state();

    /*!
     * @brief Reads the boot override configuration and detects new changes.
     * Intended to be called periodically by ipu::Service.
     * */
    void check_for_updates();

    /*!
     * @brief Sets the boot override settings requested by the user
     * @param[in] New boot override setting, or an empty optional if none was provided
     * @param[in] New boot override target, or an empty optional if none was provided
     * */
    void set(OptionalField<agent_framework::model::enums::BootOverride>,
             OptionalField<agent_framework::model::enums::BootOverrideTarget>);
private:
    /*!
     * @brief Struct representing the contents of ACC_BOOT_OVERRIDE_FILEPATH file ACC boot override configuration
     * */
    typedef struct {
        enums::State state{enums::State::ContinuousOverride};
        enums::BootType boot_type{enums::BootType::DramBoot};
        std::string error{""};
    } OverrideConfig;

    /*!
     * @brief Maps ACC boot override state to Redfish
     * @param[in] Boot override state
     * @return the equivalent Redfish BootOverride
     * */
    agent_framework::model::enums::BootOverride state_to_redfish(enums::State);

    /*!
     * @brief Maps ACC boot override target to Redfish
     * Note: this function does not attempt to distinguish DramBoot types
     * and returns Utilities for this boot type.
     * @param[in] Boot override target
     * @return the equivalent Redfish BootOverrideTarget
     * */
    agent_framework::model::enums::BootOverrideTarget boot_type_to_redfish(enums::BootType);

    /*!
     * @brief Reads the contents of ACC_BOOT_OVERRIDE_FILEPATH file .
     * The file may be absent or malformed. In these cases, the function returns
     * an empty optional.
     * @return ACC boot override configuration
     * */
    std::optional<OverrideConfig> read_override_config();

    /*!
     * @brief Reads the contents of ACC_BOOT_OPTION_FILEPATH file to check
     * if the configured DramBoot type is "ram" boot (MFG ACC boot, mapped to Redfish Utilities overide target) or "ramdisk" boot (ISO, mapped to Cd target).
     * @return ACC DramBoot subtype as Redfish enum, or None Target in case of failures
     * */
    agent_framework::model::enums::BootOverrideTarget detect_dram_boot_type();

    /*!
     * @brief Checks the ACC boot override config for error. If the Error field is an empty string or is "None",
     * there is no error.
     * @param[in] ACC boot override configuration
     * @return true if an error was reported by ACC UEFI in the config file; false otherwise
     * */
    bool has_error(const OverrideConfig& config);

    /*!
     * @brief Updates the View accoring to the ACC boot override config
     * @param[in] ACC boot override configuration
     * */
    void update_view(const std::optional<OverrideConfig>& config);

    /*!
     * @brief When checking for updates, this method checks if the override config indicates that a previously created memory reservation for ISO should be reverted.
     * @param[in] ACC boot override configuration
     * */
    void update_memory_reservation_for_iso(const std::optional<OverrideConfig>& config);

    /*!
     * @brief Creates a configuration file reserving memory for ACC ISO boot.
     * */
    void create_memory_reservation_for_iso();

    /*!
     * @brief Reverts memory reservation for ISO.
     * */
    void clear_memory_reservation_for_iso();

    /*!
     * @brief Ensures that FW will notice the memory reservation for ACC ISO boot.
     * This function implemnts a workaround for the fact that FW ignores
     * the BMD configuration override if the "current" (requested) boot option
     * is the same as the "active" (previously requested) boot option.
     * */
    void ensure_memory_reservation_active();

    /*!
     * @brief Removes ISO symlink.
     * */
    void remove_iso_symlink();

    /*!
     * @brief Creates ISO symlink if it didn't exist.
     * */
    void ensure_iso_symlink_presence();

    /*!
     * @brief Compares the View with the ACC boot override configuration.
     * This funciton is used to detect if the IPU service should update the View.
     * @param[in] ACC boot override configuration
     * @return true if the View is not up to date with the configuration; false otherwise
     * */
    bool view_is_outdated(const std::optional<OverrideConfig>& config);

    /*!
     * @brief Checks if the boot override settings requested by the user are allowed.
     * Unlike the validation done in the System endpoint, this function checks for supported
     * combinations of boot override settings, not single settings.
     * If only one of the parameters is provided, the other part of the combination
     * is taken from the current settings.
     * If the settings are not valid, an exception is thrown.
     * @param[in] New boot override setting, or an empty optional if none was provided
     * @param[in] New boot override target, or an empty optional if none was provided
     * */
    void check_request_validity(const OptionalField<agent_framework::model::enums::BootOverride>&,
                                const OptionalField<agent_framework::model::enums::BootOverrideTarget>&);
};

} // namespace ipu
} // namespace psme
