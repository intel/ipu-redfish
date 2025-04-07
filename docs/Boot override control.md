# ACC Boot Override management using Redfish including installing a custom OS for the ACC

## Intro

This document explains the implementation introduced for MEV-TS 2.0.

## Boot targets in scope
* MFG - a form of DramBoot
* ISO - a form of DramBoot - Redfish distinguishes between these two by checking additional configuration files
* Eval - a form of LocalIscsiBoot
* OS installed from ISO - labeled "Other" by ACC UEFI
* EFI Shell - InternalUefiShell

## Boot targets out of scope
* LocalIscsiBoot_ISO - until we have a use case that involves it
* PXE - missing API for network settings in Redfish
* HTTP - missing API for network settings in Redfish

## Initial conditions

The primary source of information is the ACC boot override config file `/mnt/imc/acc_variable/acc-uefi-boot-config.json` .

For IPU with MFG image, the override config file shall initially contain
```json
{
  "BootType": "DramBoot",
  "Error": "",
  "IpVersion": "n/a",
  "State": "ContinuousOverride",
  "VPortId": 0
}
```

For IPU with Eval image, the override config file shall initially contain
```json
{
  "BootType": "LocalIscsiBoot",
  "Error": "",
  "IpVersion": "n/a",
  "State": "ContinuousOverride",
  "VPortId": 0
}
```

## Discovery of the current settings
Applies to both Redfish startup and detecting config changes made by ACC UEFI in runtime, eg. using a discovery loop.

```mermaid
stateDiagram-v2
state "Config exists?" as has_config
state "Config contains error?" as is_error
state is_error_choice <<choice>>
state "Store Critical Health in Redfish" as critical_health
state "Store OK Health in Redfish" as ok_health
state "Is override configured?" as is_override
state is_override_choice <<choice>>
state "Store Disabled/None in Redfish" as disabled_none
state "Store Override type in Redfish" as override
state "What Boot Target?" as what_target
state what_target_choice <<choice>>
state "Check current_boot_option in /mnt/imc/acc_variable/acc-boot-option.json" as dram_boot
state dram_boot_choice <<choice>>
state "Store RemoteDrive in Redfish" as iscsi
state "Store Cd in Redfish" as iso
state "Store Utilities for MFG boot in Redfish" as mfg
state "Store UefiShell for InternalUefiShell boot in Redfish" as efi_shell

[*] --> has_config
note right of has_config: by Config we mean the ACC boot override config file /mnt/imc/acc_variable/acc-uefi-boot-config.json
has_config --> disabled_none: No
has_config --> is_error: Yes
is_error --> is_error_choice
is_error_choice --> ok_health: No
is_error_choice --> critical_health: Yes
critical_health --> is_override
ok_health --> is_override
is_override --> is_override_choice
is_override_choice --> disabled_none: if State is OneTimeOverrideSynced or OneTimeOverrideUpdated
is_override_choice --> override: if State is OneTimeOverride  or ContinuousOverride
override --> what_target
what_target --> what_target_choice
what_target_choice --> iscsi: if BootType is LocalIscsiBoot
what_target_choice --> dram_boot: if BootType is DramBoot
what_target_choice --> efi_shell: if BootType is InternalUefiShell
dram_boot --> dram_boot_choice
dram_boot_choice --> iso: option is "ramdisk"
dram_boot_choice --> mfg: option is "ram"
```

## Update loop
Redfish shall refresh its View every two minutes by re-reading the ACC boot override config file and checking for changes.
The interval is selected as a compromise between noticing changes quickly and overusing the resources of the IMC.
We assume that no changes are done by the user using other means than Redfish API. However, the configuration may be changed by ACC UEFI.
During the update, Redfish shall follow the logic from the previous section, with one caveat: if there was an ISO boot override configured previously,
and now it got disabled or changed to another boot target, Redfish shall clear the BMD configuration override needed to store the ISO in RAM.

## Applying user settings
Redfish shall not validate if the Eval (RemoteDrive) or Mfg (Utilities) boots are possible (eg. if the ACC partition was overwritten with something else).
In the same spirit, it shall not check if an ISO image is inserted for DramBoot from ISO. It shall, however, create a symlink to the ISO, if it is already inserted.

Redfish standard does not have any semantics (e.g. error messages in Base registry) for requiring that two parameters be PATCHed at the same time.
For this reason, the server allows sending a PATCH with no keys or just one setting - in the spirit of RESTful services.
If the PATCH request is empty, it's a valid no-op.
If it contains just one parameter (boot override target or override setting), the Redfish server shall use current View as the second parameter to validate the combination.

```mermaid
stateDiagram-v2
state "Reject request with error" as error
state "What request?" as what_request
state what_request_choice <<choice>>
state "empty request" as empty_request
state "request with one parameter" as one_parameter
state "request with both parameters" as both_parameters
state "use current View for the second parameter" as use_current_setting
state "What are the requested settings?" as what_settings
state what_settings_choice <<choice>>
state "configure OneTime ISO boot" as one_time_iso
state "configure Continuous ISO boot" as continuous_iso
state "configure continuous iSCSI boot; store Continuous + RemoteDrive" as continuous_iscsi
state "configure Continuous MFG boot; store Continuous + Utilities" as continuous_mfg
state "configure Continuous EFI Shell boot; store Continuous + InternalUefiShell" as continuous_efi_shell
state "Is ACC System in Critical Health? (There was an Error reported?)" as is_critical
state is_critical_choice <<choice>>
state "Clear Error. Set ACC System Health to OK" as clear_error

[*] --> what_request
what_request --> what_request_choice
what_request_choice --> empty_request: if empty request
empty_request --> [*]
what_request_choice --> one_parameter: if request only contains boot override or target parameter, but not both
one_parameter --> use_current_setting
use_current_setting --> what_settings
what_request_choice --> two_parameters: if request contains both parameters
two_parameters --> what_settings
what_settings --> what_settings_choice
what_settings_choice --> one_time_iso: if Once + Cd
what_settings_choice --> continuous_iso: if Continuous + Cd
what_settings_choice --> continuous_iscsi: if Continuous + RemoteDrive
what_settings_choice --> continuous_mfg: if Continuous + Utilities
what_settings_choice --> continuous_efi_shell: if Continuous + InternalUefiShell
what_settings_choice --> error: any other
one_time_iso --> is_critical
continuous_iso --> is_critical
continuous_iscsi --> is_critical
continuous_mfg --> is_critical
continuous_efi_shell --> is_critical
is_critical --> is_critical_choice
is_critical_choice --> clear_error: Yes
is_critical_choice --> skip: No
clear_error --> [*]
skip --> [*]
```
