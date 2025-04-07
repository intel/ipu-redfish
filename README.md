# Redfish® Server for Intel® Infrastructure Processing Unit SoC E2100

This repository contains the source code for the Redfish Server for the Intel® Infrastructure Processing Unit (IPU) SoC E2100.
This is an official public release mirror repository.

## Build

There are two build options available: cross-compilation for IPU and native compilation for x86_64 and ARM. 

To build the code for IPU, you need to obtain a copy of the Intel IPU software development kit (SDK), available
under a non-disclosure agreement (NDA). The Intel IPU SDK contains additional components, the build environment,
and guidelines on how to build and run the IPU Redfish Server. For more information, please refer to
[this link](https://www.intel.com/content/www/us/en/content-details/822607/enable-intel-ipu-e2100-with-intel-ipu-soc-e2100-software.html).

The native compilation excludes the IPU proprietary dependencies, allowing compilation and testing of the project without IPU SDK.
To compile the code natively, execute the following steps:

* Install a Linux distribution, for example, Ubuntu.

* Install dependency packages: `build-essential`, `cmake`, `pkg-config`, `m4-dev`, `zlib1g-dev`, `libzstd-dev`, `libbrotli-dev`.
Depending on the OS version, additional packages may be required. 

* Clone the Redfish code:

```sh
git clone https://github.com/intel/ipu-redfish redfish
cd redfish
```

* To run the compilation, execute the `build_main.sh` script:

```sh
./build_main.sh -a <arch>
```

where `<arch>` is `64`, `arm` or `ipu`. The last option requires the IPU SDK as the build enviroment.

If there are no compilation errors, the `ipu-redfsh-server` binary will appear in `build.*/bin` folder.

## Run

* Generate the password hash using the `ipu-redfish-encrypt-utility`:

```sh
build.*/bin/ipu-redfish-encrypt-utility <password>
```

where `<password>` is the password that should be hashed. The password length must be at least 16 symbols.

* Update the `application/config/imc-redfish-configuration.json` file:

1. Update the `"password"` field with the password hash.
2. Set `"restricted-to-interface"` to `null` to allow Redfish server to listen to all interfaces.
3. Set  `"certs-directory"` to `"."`,
4. Set  `"location"` to `"."`,

* Generate the server's private key and certificate and place them in the `build.*/bin` folder.
The following steps can be used to generate a self-signed Redfish certificate to try out the project.
They should *only* be used to test the project, not in any production capacity.

```sh
cd build.*/bin
openssl genpkey -algorithm RSA -out server.key -pkeyopt rsa_keygen_bits:4096 -pkeyopt rsa_keygen_pubexp:65537
openssl req -new -x509 -key server.key -out server.crt -days 90 -sha384 -subj "/C=PL/ST=Test/L=Test/O=Test/OU=Do Not Use In Production"
cd ..
```

* To run the Redfish server, execute the following command:

```sh
cd build.*/bin
./ipu-redfish-server ../../application/config/imc-redfish-configuration.json
```

## Test

To test the Redfish server, send an HTTPS request using `curl`:

```sh
curl -u root:<password> -k -X GET https://localhost:8443/redfish/v1/Systems/1/ | jq
```

where `<password>` is the password used for generating the hash for the `imc-redfish-configuration.json` file.

## Unit tests

To run the unit tests, run the `build_main.sh` with `-u` option:

```sh
./build_main.sh -a <arch> -u
```

where `<arch>` is `64` or `arm`.

## Known issues

The build for ARM platforms has not been tested.
