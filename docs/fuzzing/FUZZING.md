# Fuzzing using CATS

## CATS

[CATS](https://endava.github.io/cats/) is a REST API Fuzzer and negative testing tool for OpenAPI endpoints.

CATS automatically generates, runs and reports tests with minimum configuration and no coding effort.

## Prerequisites

Install CATS for your OS from the [releases page](https://github.com/Endava/cats/releases).

cats-11.0.0 / Linux was used in these tests.

Copy `<redfish repo>/fuzzing/schema/*.yaml` files next to the `cats` binary.

## Running the tool

`cats --contract=openapi.yaml --basicauth=user:password --server=https://server-address:8443`

or

`cats --contract=openapi.yaml --basicauth=user:password --server=https://server-address:8443 --blackbox`

Most of the parameters are self-explanatory, `--contract` takes a file name that lists endpoints for fuzzing.
With `--blackbox` cats will only report 500 HTTP response codes as errors.

Available contracts and supported endpoints:
| File Name                   | Fuzzed Endpoints                                        |
|-----------------------------|---------------------------------------------------------|
| openapi.yaml                | /redfish/v1, /redfish/v1/$metadata & /redfish/v1/odata  |
| openapi-AccountService.yaml | /redfish/v1/AccountService                              |
| openapi-Registries.yaml     | /redfish/v1/Registries                                  |
| openapi-UpdateService.yaml  | /redfish/v1/UpdateService                               |

## Test results

CATS generates its test report in HTML & JSON formats in cats-report directory.
Every test lists the request that was sent and the response received.
For convenience, requests are also available as cURL commands.

Not all failures are critical; special attention needs to be paid to those that result in server crashes.

## Preparing the schema files

CATS uses YAML spec as an input - and Redfish already provides [one](http://redfish.dmtf.org/schemas/v1/openapi.yaml).
The issue is that spec lists _all_ possible endpoints while we only implement a small subset of them.
Fuzzing all optional endpoints would take a lot of time, so it was decided to trim down openapi.yaml to only include the endpoints we support.
Another issue with CATS is that it does not support `$ref: http://redfish.dmtf.org/schemas/v1/File.yaml` syntax, assuming all referenced files are local.
This was resolved by copying the referenced YAML files next to openapi.yaml and doing a simple search & replace removing the `http://redfish.dmtf.org/schemas/v1/` prefix.
The following command is useful when you need to get the complete list of YAML files referenced in the spec:

`grep -hoP '(?<=\$ref: ).*?(?=\.yaml)' *.yaml | awk '{print $0".yaml"}' | sort | uniq`

The process is iterative and can probably be scripted:

- Edit openapi.yaml, trimming unnecessary endpoints
- Remove all `http://redfish.dmtf.org/schemas/v1/` references
- Get the referenced YAML files list, and copy them
- Remove all `http://redfish.dmtf.org/schemas/v1/` references from the new files
- Get the referenced YAML files list again, make sure no new files are needed, or repeat previous steps

### Other changes

CATS parser didn't like ManagerAccount_v1_12_0_SNMPUserInfo (regex part of it) - either because the regex is not Java-compatible, or maybe even wrong.
Since SNMP is not used in IPU Redfish Server, ManagerAccount_v1_12_0_SNMPUserInfo and all references to it has been removed from `ManagerAccount*.yaml` files.
