#!/bin/bash

# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

redfish_config="/work/redfish/imc-redfish-configuration.json"
backup_config="/work/redfish/imc-redfish-configuration.backup.json"

if [ ! -f "$redfish_config" ]; then
    echo "Error: $redfish_config does not exist"
    echo "Copy the configuration file template from /etc/imc-redfish-configuration.json"
    echo "And run /usr/bin/ipu-redfish-generate-password-hash.sh to provision the file with the hash of the Redfish admin password"
    exit 1
fi

# Check if the password is set correctly
password=$(grep -oP '(?<="password" : ")[^"]*' "$redfish_config")
if [ "$password" == "<placeholder>" ]; then
    echo "Error: The password hash is not provisioned in $redfish_config."
    echo "Run /usr/bin/ipu-redfish-generate-password-hash.sh to set it."
    exit 1
fi

# Check if the config is in old format (contains "connectors" section)
if grep -q '"connectors"' "$redfish_config"; then
    echo "Old configuration format detected. Converting to new format..."
    cp "$redfish_config" "$backup_config"
    if [ $? -ne 0 ]; then
        echo "Error: Failed to create backup of the configuration file."
        exit 1
    fi
    python3 /usr/bin/ipu-redfish-upgrade-config.py "$redfish_config"
    if [ $? -ne 0 ]; then
        echo "Error: Failed to convert configuration to new format."
        echo "Please check the configuration file at $redfish_config"
        exit 1
    fi
fi

/usr/bin/ipu-redfish-server "$redfish_config"
