#!/usr/bin/env python3

# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

import json
import sys
import os

def convert_old_to_new_config(config_path):
    with open(config_path, 'r') as file:
        config = json.load(file)
    
    # Check if the configuration contains the "connectors" field
    if "connectors" in config.get("server", {}):
        # Extract the first connector's details
        connector_details = config["server"]["connectors"][0]
        
        # Remove the "connectors" field
        del config["server"]["connectors"]
        
        # Flatten the "network-interface-name" array to a single string
        if isinstance(config["server"].get("network-interface-name"), list):
            config["server"]["network-interface-name"] = config["server"]["network-interface-name"][0]
        
        # Move the connector details to the top level of the "server" object
        config["server"].update(connector_details)
        
        # Save the new configuration format to the same file
        with open(config_path, 'w') as file:
            json.dump(config, file, indent=4)
        
        print(f"Converted old config to new format and saved to {config_path}")
    else:
        print("The configuration file is already in the new format.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: python {os.path.basename(__file__)} <config_path>")
        sys.exit(1)
    
    config_path = sys.argv[1]
    
    if not os.path.exists(config_path):
        print(f"Error: {config_path} does not exist")
        sys.exit(1)
    
    convert_old_to_new_config(config_path)
