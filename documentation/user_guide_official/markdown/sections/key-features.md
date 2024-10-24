Key features
============

== This document is out of date ==

This section explains some of the key features of the Intel® RSD PSME software. Use of the features requires a
correct setup configuration. Instructions are provided in TBD


Simple Service Discovery Protocol (SSDP)
---------------

The PSME REST server can be configured to announce its presence over the network
using IP multicast datagrams carrying Simple Service Discovery Protocol (SSDP) presence
announcements. In the PSME REST server's configuration, the **ssdp-service**
section determines the behaviour of PSME's SSDP service and it contains the
following options:

CAPTION(Table, Configuration options for SSDP service)

<table>
    <tr>
        <th>option</th>
        <th>type</th>
        <th>description</th>
    </tr>
    <tr>
        <td>enabled</td>
        <td>boolean</td>
        <td>specifies if SSDP service should be enabled</td>
    </tr>
    <tr>
        <td>announce-interval-seconds</td>
        <td>integer</td>
        <td>if the interval is non-zero, it specifies the number of seconds between SSDP presence<br/>
        announcements. Zero interval means no announcements will be sent.</td>
    </tr>
    <tr>
        <td>ttl</td>
        <td>integer</td>
        <td>specifies the time to live value of notifying multicast datagrams</td>
    </tr>
</table>


Security features
-----------------

This section describes the security features of the Intel® RSD PSME software.

### Secure communication over TLS

*Certificates described in this section can be easily generated, or you may use
existing certificates - refer to [Table CAPTION_NUMBER(#reference-documents)](#reference-documents) Intel® RSD POD Manager User Guide, Section 3.2.2,
Key and Certificate management.

Every PSME instance can be configured to only accept HTTPS connections secured
with the TLS protocol. The PSME implements mutual (bi-directional) authentication.
The following points present the details of this feature:

1.  All certificates (and/or private keys) must be stored in a secure
directory. The directory must be placed in the local file system, and must be
available for the `root` user only. No symlinks are allowed.

    Default directory `/etc/psme/certs` may be changed in the appropriate `psme-rest-server-configuration.json` file:
        "server": {
            "connectors" : [
                {
                    "certs-directory" : "/etc/psme/certs",
                    ...

2.  For the PSME Rest Server, the appropriate private key (`server.key`) and
certificate signed by an authorized CA (`server.crt`, to authenticate the PSME
Rest-Server service in the POD Manager) must be manually stored in certificate directory.

3.  The certificat used to authenticate the client must be manually stored on as `ca.crt` file.
    The `ca.crt` file must be placed in the certificates directory.

    The stored certificate file must be in PEM format.

4.  The PSME allows for disabling client authentication. To do this, change
the SSL connector configuration (in the `psme-rest-server-configuration.json` file):

        "client-cert-required": false

    In this mode any client would be able to connect to the PSME Rest Server service without authentication.

6. Intel® RSD $software_version$ offers no mechanisms for certificate renewal or expiration. Thus, certificate management
is left to the administrator. PSME software should be restarted if any of the files with certificates or private
keys are updated.

### Binding the REST Server to a network interface

The PSME REST Server configuration should be adjusted to list the interfaces on
which the service should listen for requests. In the standard scenario, the list should
contain only the management interface - the one on the same subnet as the PODM software
managing the service. To set the interface, change the following section of
`/etc/psme/psme-rest-server-configuration.json`:

        "network-interface-name": ["enp0s20f0.4094"] -> "network-interface-name": ["your_management_interface"]

#### Limitations

If the IP of one interfaces changes while the service is operational, it will not respond to
requests on the new IP address. As a workaround, disable the service before changing the
network configuration and re-enable it when the changes are complete.

### Redfish Authentication

This section describes how to setup Redfish Authentication in Intel® RSD PSME software.

#### Introduction

Intel® RSD $software_version$ PSME software supports configuring an administrator user account for
the software. By default, access to the API requires authenticating using the administrator user's
credentials. Two modes of authentication are supported:

-   HTTP Basic Authentication - authentication by providing username and password in request header.

-   Redfish session token authentication - sending credentials in a `POST` request to create a session token,
then providing that token in request header in subsequent requests.

#### Configuration of the PSME Rest Server

The following sections in the `psme-rest-server-configuration.json` should be adjusted to
configure authentication:

-   `"username": "root"` - should be filled with the username for administrator account

-   `"password": "put_password_hash_here"` - should be filled with a hash of the password
    for the administrator account generated using the `encrypt` binary. The binary can be compiled
    by following the instructions in the "PSME Development environment" chapter. To generate the hash
    for a given password, run `ipu-redfish-encrypt-utility`:

        $./ipu-redfish-encrypt-utility example_password

    Then, copy the output to the configuration file.

-   `"authentication-type": "basic-or-session"` - this flag can be modified to disable one
    or both modes of authentication. The accepted values are `basic`, `session` and `none`.

    <div custom-style="Note">Note: It is recommended that authentication not be disabled.
    `none` flag should be used for testing purposes only.</div>

