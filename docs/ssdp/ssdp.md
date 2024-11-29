# SSDP based psme service discovery #

## Overview ##

While the service entry resides at a well-known URI, the host must be discovered.
Use of SSDP (Simple Service Discovery Protocol) is one of possible approaches.
While SSDP is a part of UPnP protocol stack, and is described in [UPnP standards
documents](http://upnp.org/specs/arch/UPnP-arch-DeviceArchitecture-v2.0.pdf),
this implementation follows [dmtf redfish specification](http://www.dmtf.org/sites/default/files/standards/documents/DSP0266_1.0.2.pdf)
and [IETF internet draft](http://tools.ietf.org/html/draft-cai-ssdp-v1-03).

Some differences with UPnP specification:

- Use of SSDP is optional and may be disabled (via configuration)
- Service does not respond to M-SEARCH queries searching for Search Target (ST)
  of "upnp:rootdevice".
- Response does not contain LOCATION header pointing to the UPnP XML descriptor.
- Response use AL header pointing to the Service Root URL.
- Sending announcement messages (ssdp:alive and ssdp:byebye) is implemented,
  but may be disabled (via configuration) separately from the M-SEARCH
  response functionality. This allows users to utilize the discovery functionality
  with minimal amounts of network traffic generated.

## Configuration options ##

By default SSDP service is enabled, but outgoing notifications are disabled.
This means the service is waiting for M-SEARCH requests without sending
ssdp::alive and ssdp:byebye messages.

psme-rest-server-configuration.json excerpt for SSDP service:

        {
            "server" : {
                "network-interface-name" : "enp0s20f0.4094"
                ...
            }
            ...
            "ssdp-service" : {
                "enabled" : true,
                "announce-interval-seconds" : 0,
                "ttl" : 2
            }
            ...
        }

| Option name  | Description | Default value |
|---|---|---|
|server.<b>network-interface-name</b> |  Specifies on which interface(s) the SSDP traffic is listened on. Option is **required**. | "enp0s20f0.4094" |
|ssdp-service.<b>enabled</b> | **true** if SSDP service should be enabled, **false** otherwise. | true |
|ssdp-service.<b>announce-interval-seconds</b>| Number of seconds between announcement notifications. **Zero value (default)** means notifications are disabled | 0 (zero) |
|ssdp-service.<b>ttl</b>| Time to live value of notifying multicast sockets. | 2 |
|rest.service-root-name| Not strictly SSDP related and may be removed in future. Used by PODM to distinguish type of service. Supported values: "PSME Service Root", "RSS Service Root", "LUI Service Root", "RMM Service Root".| "PSME Service Root" |

## Some technical details ##

To be lightweight, all SSDP requests are processed by SsdpService on single thread
with non-blocking sockets.
SsdpService opens listening multicast socket and waits for M-SEARCH requests.
When a valid request arrives it's processed and response is send to requester.

## SSDP based service discovery ##

@startuml
!include ssdp/ssdp_based_service_discovery.puml
@enduml
