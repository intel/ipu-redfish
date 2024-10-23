#!/bin/bash

redfish_config="/work/redfish/imc-redfish-configuration.json"

if [ ! -f "$redfish_config" ]; then
    echo "Error: $redfish_config does not exist"
    echo "Copy the configuration file template from /etc/imc-redfish-configuration.json"
    echo "And run /usr/bin/ipu-redfish-generate-password-hash.sh to provision the file with the hash of the Redfish admin password"
    exit 1
fi

/usr/bin/ipu-redfish-server "$redfish_config"
