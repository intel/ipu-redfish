/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "json-wrapper/json-wrapper.hpp"

namespace psme {
namespace app {

static json::Json DEFAULT_VALIDATOR_JSON = R"~(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "Intel IPU Redfish Service Configuration",
    "type": "object",
    "properties": {
        "service": {
            "type": "string",
            "description": "Name of the service"
        },
        "server": {
            "type": "object",
            "properties": {
                "restricted-to-interface": {
                    "type": ["string", "null"]
                },
                "certs-directory": {
                    "type": "string",
                    "description": "Path to directory containing certificates"
                },
                "port": {
                    "type": "integer",
                    "description": "Port number",
                    "minimum": 0,
                    "maximum": 65535
                },
                "thread-mode": {
                    "type": "string",
                    "description": "Thread mode",
                    "enum": ["select", "thread-per-connection"]
                },
                "thread-pool-size": {
                    "type": "integer",
                    "description": "Thread pool size used by connector in SELECT thread-mode.",
                    "minimum": 1
                },
                "client-cert-required": {
                    "type": "boolean",
                    "description": "Whether client certificate is required (mutual TLS)"
                },
                "authentication-type": {
                    "type": "string",
                    "description": "Authentication type",
                    "enum": ["none", "basic", "session", "basic-or-session"]
                }
            },
            "required": ["restricted-to-interface", "certs-directory", "port", "thread-mode", "client-cert-required", "authentication-type"]
        },
        "authentication": {
            "type": "object",
            "properties": {
                "username": {
                    "type": "string",
                    "description": "Username"
                },
                "password": {
                    "type": "string",
                    "description": "Password hash"
                }
            },
            "required": ["username", "password"]
        },
        "session-service": {
            "type": "object",
            "properties": {
                "service-enabled": {
                    "type": "boolean",
                    "description": "Whether the session service is enabled"
                },
                "session-timeout": {
                    "type": "integer",
                    "description": "Session timeout in seconds",
                    "minimum": 30,
                    "maximum": 86400
                }
            },
            "required": ["service-enabled", "session-timeout"]
        },
        "ssdp-service": {
            "type": "object",
            "properties": {
                "enabled": {
                    "type": "boolean",
                    "description": "Whether SSDP service is enabled"
                },
                "announce-interval-seconds": {
                    "type": "integer",
                    "description": "Announce interval in seconds"
                },
                "ttl": {
                    "type": "integer",
                    "description": "Time to live - the number of routers a packet can pass through to transfer SSDP service status",
                    "minimum": 1,
                    "maximum": 255
                }
            },
            "required": ["enabled", "announce-interval-seconds", "ttl"]
        },
        "database": {
            "type": "object",
            "properties": {
                "location": {
                    "type": "string",
                    "description": "Database location"
                }
            },
            "required": ["location"]
        },
        "loggers": {
            "type": "array",
            "items": {
                "type": "object",
                "properties": {
                    "name": {
                        "type": "string",
                        "description": "Logger name"
                    },
                    "level": {
                        "type": "string",
                        "description": "Logging level",
                        "enum": [
                            "EMERGENCY",
                            "ALERT",
                            "CRITICAL",
                            "ERROR",
                            "WARNING",
                            "NOTICE",
                            "INFO",
                            "DEBUG"
                        ]
                    },
                    "timeformat": {
                        "type": "string",
                        "description": "Time format",
                        "enum" : [
                            "NONE",
                            "DATE_SEC",
                            "DATE_MS",
                            "DATE_US",
                            "DATE_NS"
                        ]
                    },
                    "color": {
                        "type": "boolean",
                        "description": "Whether to use color in logs"
                    },
                    "output": {
                        "type": "boolean",
                        "description": "Whether to output logs"
                    },
                    "tagging": {
                        "type": "boolean",
                        "description": "Whether to use tagging in logs"
                    },
                    "moredebug": {
                        "type": "boolean",
                        "description": "Whether to enable more debug information"
                    },
                    "streams": {
                        "type": "array",
                        "items": {
                            "type": "object",
                            "properties": {
                                "type": {
                                    "type": "string",
                                    "description": "Stream type",
                                    "enum": [ 
                                        "STDOUT",
                                        "STDERR",
                                        "FILE",
                                        "UDP",
                                        "TCP",
                                        "DIRECT"
                                    ]
                                }
                            },
                            "required": ["type"]
                        }
                    }
                },
                "required": ["name", "level", "timeformat", "color", "output", "tagging", "moredebug", "streams"]
            }
        }
    },
    "required": ["service", "server", "authentication", "session-service", "ssdp-service", "database", "loggers"]
})~"_json;

} // namespace app
} // namespace psme
