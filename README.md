# Redfish for Internet Processing Unit SoC E2100

## General info
The repo contains the Redfish code for Intel® Internet Processing Unit (IPU) SoC E2100 software. This is an official public release mirror repo. 

## Build
The current `main` branch contains the original code of IPU Redfish code. It has the proprietary components
and it is not compilable by default. If you would like to build the original code, you need to obtain the copy the Intel IPU SDK,
available under SDK License.

To build the code, just clone it and run `build_main.sh` script

```
./build_main.sh -a arm # release build for aarch64
./build_main.sh -a 64  # release build for x86_64

./build_main.sh -a arm -b debug # debug build for aarch64
./build_main.sh -a 64  -b debug # debug build for x86_64
```

After the build has been completed, the `ipu-redfish-server` application file can be found in `build.*/bin` folder

If you do not have the IPU SDK, you can build and test the reduced version of Redfish code without the proprietary components. 
You can find it at `compilable-main` branch.

## Run server

To start the Redfish launch the `ipu-redfish-server` from `build.*/bin` with addition of the path to the config file.
By default it is located in `application/config/imc-redfish-configuration.json`.

The `imc-redfish-configuration.json` should be adjusted according to the host settings.
In particular, set the network interface and provide the hash of the password that you would like to use. Then you can
launch the Redfish server application with the following command.

```
GNUTLS_FORCE_FIPS_MODE=1 ./ipu-redfish-server ./application/config/imc-redfish-configuration.json
```

## Send request

When the server is running, you can try to send the following request to Redfish server:

```
curl -u root:<password> -k -X GET https://localhost:8443/redfish/v1/Managers/1/ | jq
```

If everything is correct, you will see the response from the Redish.

## Unit tests
To run unit tests, add -u parameter to `./build_main.sh` script.

./build_main.sh -a arm -u # build for aarch64 and run unit tests
./build_main.sh -a 64 -u  # build for x86_64 and run unit tests

## Contribution
We do not accept pull requests.
