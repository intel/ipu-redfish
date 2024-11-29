# Setting up the Redfish server

## Configuration file

You should get the configuration file from [here](../application/config/imc-redfish-configuration.json)
and transfer it to the IMC.

The plan is to have it installed in `/etc/` by default.

Set `"network-interface-name"` to the Linux name of the network interface on which
you want Redfish server to respond. By default, it points to the I211 1G adapter.

In `"server"` section:

Set `"port"` to the TCP port where you want Redfish to respond.

`"certs-directory"` should point to the directory where the server private key
and X.509 certificate for TLS (HTTPS) are placed.

The plan is to have a default self-signed certificate installed in the IMC.

For user-defined TLS files, the user should create a custom directory
in `/work/` (partition 10, `/dev/nmve0n1p10`), set this location in `"certs-directory"`,
and place the files there. This way they will be preserved over IMC reboot.
The certificate should be named `"server.crt"` and the private key `"server.key"`.

*TODO* document required certificate format: should it be DER, PEM ..?

If `"client-cert-required"` is `true`, mTLS mode is enabled. The certificates directory
should contain an additional certificate file `"ca.crt"`, which will be
used by the Redfish server to verify the client certificate.

The `"authentication"` section stores the username and the *hash* of the password
of the server's Administrator user - meaning, the credentials necessary to
access the Redfish server APIs.

The default values are `root : root`, however, that may change in the future.

To set your credentials, generate the hash of your password using the
`ipu-redfish-encrypt-utility` tool, which is also provided by this project,
and place the output in the configuration file.

`$ ipu-redfish-encrypt-utility <your password>`

## Running the Redfish server

Obtain the Redfish server binary `ipu-redfish-server`.

The plan is to have it installed in `/usr/bin/` by default.

Run the binary, providing the configuration file path as a command line argument:

`$ ipu-redfish-server /etc/psme/imc-redfish-configuration.json`
or
`$ ipu-redfish-server <your config>`

If no configuration file is provided, the Redfish server uses internal defaults.

The plan is to have Redfish registered as a service in the IMC Linux.
