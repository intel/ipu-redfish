/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

#pragma once

#include "json-wrapper/json-wrapper.hpp"

namespace psme {
namespace app {

static json::Json DEFAULT_VALIDATOR_JSON = R"(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "PSME Rest Server",
    "description": "Detailed JSON schema documentation used for PSME REST server configuration file. Configuration file is pure JSON format.",
    "name": "/",
    "type": "object",
    "properties": {
        "service": {
            "description": "Name of DB entry with service UUID.",
            "name": "service",
            "type": "string"
        },
        "server": {
            "description": "Information about REST server communication. Port numbers, addresses.",
            "name": "server",
            "type": "object",
            "properties": {
                "network-interface-name": {
                    "description": "List of network interface names used to send and receive packets.",
                    "name": "network-interface-name",
                    "type": "array",
                    "items": {
                        "type": "string"
                    }
                },
                "connectors": {
                    "description": "Connectors server listens on",
                    "name": "connectors",
                    "type": "array",
                    "items": [
                        {
                            "type": "object",
                            "properties": {
                                "certs-directory": {
                                    "description": "Path to directory containing files for ssl connector.",
                                    "name": "certs-directory",
                                    "type": "string"
                                },
                                "port": {
                                    "description": "Port on which connector listens on",
                                    "name": "port",
                                    "type": "integer",
                                    "minimum": 0,
                                    "maximum": 65535
                                },
                                "thread-mode": {
                                    "description": "Enumeration of available threading modes of connector",
                                    "name": "thread-mode",
                                    "type": "string",
                                    "enum": [
                                        "select",
                                        "thread-per-connection"
                                    ]
                                },
                                "thread-pool-size": {
                                    "description": "Thread pool size used by connector in SELECT thread-mode. THREAD_PER_CONNECTION does not use this property.",
                                    "name": "thrad-pool-size",
                                    "type": "integer"
                                },
                                "client-cert-required": {
                                    "description": "Client is required to send certificate when connecting",
                                    "name": "client-cert-required",
                                    "type": "boolean"
                                },
                                "authentication-type": {
                                    "description": "Authentication type for the connector",
                                    "name": "authentication-type",
                                    "type": "string",
                                    "enum": [
                                        "none",
                                        "basic",
                                        "session",
                                        "basic-or-session"
                                    ]
                                }
                            }
                        }
                    ],
                    "required": [
                        "network-interface-name",
                        "connectors"
                    ]
                }
            },
            "authentication" : {
                "description": "Configuration of authentication for REST server.",
                "name": "authentication",
                "type": "object",
                "properties": {
                    "username": {
                        "description": "User name of the administrator account in REST server.",
                        "name": "username",
                        "type": "string"
                    },
                    "password" : {
                        "description": "Password hash of the administrator account in REST server.",
                        "name": "password",
                        "type": "string"
                    }
                }
            },
            "session-service": {
                "description": "Configuration of SessionService.",
                "name": "session-service",
                "type": "object",
                "properties": {
                    "service-enabled": {
                        "description": "This is boolean value used to enable (true) and disable (false) SessionService.",
                        "name": "service-enabled",
                        "type": "boolean"
                    },
                    "session-timeout": {
                        "description": "This represents the number of seconds which passes from last session usage before session become invalid.",
                        "name": "session-timeout",
                        "type": "integer",
                        "minimum": 30,
                        "maximum": 86400
                    }
                },
                "required": [
                    "service-enabled",
                    "session-timeout"
                ]
            },
            "task-service": {
                "description": "Configuration of Task service.",
                "name": "task-service",
                "type": "object",
                "properties": {
                    "enabled": {
                        "description": "Specifies if Task service should be enabled.",
                        "name": "enabled",
                        "type": "boolean"
                    }
                }
            },
            "ssdp-service": {
                "description": "Configuration of SSDP service.",
                "name": "ssdp-service",
                "type": "object",
                "properties": {
                    "enabled": {
                        "description": "Specifies if SSDP service should be enabled.",
                        "name": "enabled",
                        "type": "boolean"
                    },
                    "announce-interval-seconds": {
                        "description": "This represents the number of seconds between announcement notifications. Zero value means no notifications will be send.",
                        "name": "announce-interval-seconds",
                        "type": "integer"
                    },
                    "ttl": {
                        "description": "This represents time to live value of notifying multicast sockets.",
                        "name": "ttl",
                        "type": "integer"
                    }
                },
                "required": [
                    "enabled"
                ]
            },
            "rest": {
                "description": "REST specific configuration.",
                "name": "rest",
                "type": "object",
                "properties": {
                    "service-root-name": {
                        "description": "Value of Name property on ServiceRoot resource",
                        "name": "service-root-name",
                        "type": "string"
                    }
                },
                "required": [
                    "service-root-name"
                ]
            },
            "loggers": {
                "description": "Logger configuration.",
                "name": "loggers",
                "type": "array",
                "items": {
                    "description": "Settings of a specific logger.",
                    "type": "object",
                    "properties": {
                        "name": {
                            "description": "Set the name of the logger.",
                            "name": "name",
                            "type": "string"
                        },
                        "default": {
                            "description": "Set the logger as default. Only one can be default.",
                            "name": "default",
                            "type": "boolean"
                        },
                        "level": {
                            "description": "Choose severity level compatible with syslog.",
                            "name": "level",
                            "type": "string"
                        },
                        "timeformat": {
                            "description": "Define format used for timestamps in log file.",
                            "name": "timeformat",
                            "type": "string"
                        },
                        "color": {
                            "description": "Enable or disable colors in log file.",
                            "name": "color",
                            "type": "boolean"
                        },
                        "output": {
                            "description": "Turn on, off logging.",
                            "name": "output",
                            "type": "boolean"
                        },
                        "tagging": {
                            "description": "Turn on/turn off tagging in application.",
                            "name": "tagging",
                            "type": "boolean"
                        },
                        "moredebug": {
                            "description": "Enable/disable additional debug info in log file.",
                            "name": "moredebug",
                            "type": "boolean"
                        },
                        "streams": {
                            "description": "Configuration of output methods for logger.",
                            "name": "streams",
                            "type": "array",
                            "items": {
                                "type": "object",
                                "properties": {
                                    "type": {
                                        "description": "Choose one of the output methods. Like FILE of STDOUT.",
                                        "name": "type",
                                        "type": "string"
                                    },
                                    "file": {
                                        "description": "Path to the file, if stream type is set to FILE.",
                                        "name": "file",
                                        "type": "string"
                                    }
                                },
                                "required": [
                                    "type"
                                ]
                            }
                        }
                    }
                }
            }
        }
    },
    "required": [
        "service",
        "server",
        "loggers"
    ]
})"_json;

} // namespace app
} // namespace psme
