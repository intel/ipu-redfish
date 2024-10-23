#!/usr/bin/python3

# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

# Parse Redfish xml files and extract all enumerations in enum_builder.hpp format

import xml.etree.ElementTree as ET
import sys

# Check if filename is given as command-line argument
if len(sys.argv) != 2:
    print("Usage: python3 generate_enums.py <filename>")
    sys.exit(1)

filename = sys.argv[1]

tree = ET.parse(filename)
root = tree.getroot()

# Namespace map to find elements
namespaces = {'edm': 'http://docs.oasis-open.org/odata/ns/edm'}

# Find all EnumType elements in the XML
enum_types = root.findall('.//edm:EnumType', namespaces)

for enum_type in enum_types:
    enum_name = enum_type.attrib['Name']
    enum_members = [member.attrib['Name'] for member in enum_type.findall('edm:Member', namespaces)]

    enum_string = 'ENUM(' + enum_name + ', uint32_t, ' + ', '.join(enum_members) + ');'
    print(enum_string)
