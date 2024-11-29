#!/bin/bash

# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

# This script is used to generate a password hash for the Redfish service

redfish_encrypt_utility="/usr/bin/ipu-redfish-encrypt-utility"
redfish_config="/work/redfish/imc-redfish-configuration.json"

if [ ! -f "$redfish_encrypt_utility" ]; then
    echo "Error: /usr/bin/ipu-redfish-encrypt-utility does not exist"
    exit 1
fi

if [ ! -f "$redfish_config" ]; then
    echo "Error: $redfish_config does not exist"
    exit 1
fi

# Extract the current password value from the configuration file
current_password=$(grep -oP '(?<="password" : ")[^"]*' "$redfish_config")

if [ -n "$current_password" ] && [ "$current_password" != "<placeholder>" ]; then
    echo "Password is already set."
    echo "Do you want to overwrite it? (Y/N)"
    read -r overwrite
    if [[ "$overwrite" != "Y" && "$overwrite" != "y" ]]; then
        echo "Operation cancelled by user."
        exit 0
    fi
fi

echo "Enter the password for the Redfish service:"
read -s password

# Generate password hash
encrypted_password=$($redfish_encrypt_utility "$password")

if [ $? -ne 0 ]; then
    echo "Error: Failed to generate password hash"
    exit 1
fi

# Update the configuration file with the new hash
# Cannot use 'jq' on IMC - use sed instead
sed -i.bak -E "s/\"password\" : \"[^\"]*\"/\"password\" : \"$encrypted_password\"/" "$redfish_config"

if [ $? -ne 0 ]; then
    echo "Error: Failed to update password in $redfish_config"
    exit 1
fi

echo "Password updated in $redfish_config"
