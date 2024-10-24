#!/bin/bash

# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

# Get version from sdk_version.json
# Path to sdk_version.json is specified by the environment variable PLDM_VERSION_SHA_JSON
# PLDM_VERSION_SHA_JSON is set in ipu_platform/Makefile
get_version_from_json() {
    if [ -z "$PLDM_VERSION_SHA_JSON" ]; then
        echo "Unknown"
        return
    fi

    # Check if the file exists at the path specified by the environment variable
    if [ -f "$PLDM_VERSION_SHA_JSON" ]; then
        # intentionally not using `jq` to avoid additional dependencies
        BUILD_VERSION=$(grep -oP '"build"\s*:\s*"\K[^"]+' "$PLDM_VERSION_SHA_JSON")
        echo "$BUILD_VERSION"
    else
        echo "Unknown"
    fi
}

# Check if the current directory is a Git repository
if git rev-parse --is-inside-work-tree > /dev/null 2>&1; then
    # Get the latest git tag
    GIT_TAG=$(git describe --tags `git rev-list --tags --max-count=1` 2>/dev/null)
    
    if [ -z "$GIT_TAG" ]; then
        # If no tag is found, use the short SHA of the latest commit
        GIT_TAG=$(git rev-parse --short HEAD)
    fi
    
    VERSION_STRING="${GIT_TAG}"
else
    # Get the version from sdk_version.json
    JSON_VERSION=$(get_version_from_json)
    VERSION_STRING="${JSON_VERSION}"
fi

# Define the template content directly in the script
TEMPLATE_CONTENT='/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

namespace psme {
namespace app {

const char* VERSION = "Unknown";

} // namespace app
} // namespace psme
'

# Replace the placeholder with the version string
NEW_CONTENT=$(echo "$TEMPLATE_CONTENT" | sed "s/const char\* VERSION = \"Unknown\";/const char\* VERSION = \"${VERSION_STRING}\";/g")
OUTPUT_FILE="version.hpp"

# Check if the new content is different from the current content
if [ ! -f "$OUTPUT_FILE" ] || [ "$NEW_CONTENT" != "$(cat $OUTPUT_FILE)" ]; then
    echo "$NEW_CONTENT" > "$OUTPUT_FILE"
    echo "Updated $OUTPUT_FILE with new version: $VERSION_STRING"
else
    echo "$OUTPUT_FILE is already up to date."
fi
