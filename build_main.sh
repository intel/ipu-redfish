#!/usr/bin/env bash

# SPDX-License-Identifier: BSD-3-Clause
# Copyright (C) 2024 Intel Corporation

# Copyright (c) 2015-2019 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -u

TIME=$SECONDS

# Find if we are in the main directory by looking for "scripts" directory and the script file.
# Then compare the contents of _this_ script and the one in "scripts".
# We cannot assume it's a symbolic link because QuickBuild doesn't really work well with them.
STARTING_DIR="$(pwd)"
MAIN_BUILD_DIR=$(dirname "$0")

TIMEFORMAT="%0lR"

RESULTS_CONFIGURATIONS=()
RESULTS_CMAKE_STATUSES=()
RESULTS_MAKE_STATUSES=()
RESULTS_UNIT_TEST_STATUSES=()
RESULTS_CMAKE_TIMES=()
RESULTS_MAKE_TIMES=()
RETVAL=0

# CONSTANTS
RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
COLOR_END="\e[0m"
STATUS_SUCCESS="SUCCESS"
STATUS_FAILURE="FAILURE"
STATUS_SKIPPED="SKIPPED"
NOT_FOUND="no"
FOUND="yes"
BUILD_ALL="all"
CLEAN=false
LIGHT_CLEAN=false
TARGET_SUBDIRECTORIES=("application" "encrypter" "libs")

# POSSIBLE CONFIGURATIONS
POSSIBLE_BUILD_TYPES=("debug" "release" "coverage" "asanitize" "tsanitize")
POSSIBLE_COMPILERS=("gcc" "clang")
POSSIBLE_ARCHITECTURES=("64" "arm")

# FIRST OPTION
# Find all possible targets by scanning through CMakeLists.txt for "add_executable/add_library", strip unnecessary elements,
# get unique items and sort them. Some folders (build) are skipped, so if you want to compile
# them using this script make sure to update --exclude-dir. This option has the problem of not looking at what's
# actually enabled in CMakeLists.txt files - folder or files might be skipped with comments or if's which may result
# in holding inactive elements. It also ignored cmake's variables. It's still better than nothing and doesn't make us
# use cmake before listing all targets.
#
for SUBDIRECTORY in ${TARGET_SUBDIRECTORIES[*]} ; do
    SUBDIRECTORY_UPPER="${SUBDIRECTORY^^}"
    SUBDIRECTORY_UPPER="${SUBDIRECTORY_UPPER/-/_}"
    read POSSIBLE_EXECUTABLES_${SUBDIRECTORY_UPPER} <<< "$(grep -Ir --include="CMakeLists.txt" --exclude-dir="*build*" "add_executable(" $SUBDIRECTORY | sed 's/.*add_executable(//' | sed 's/ .*//g' | sort --unique | tr '\n' ' ')"
    read POSSIBLE_LIBRARIES_${SUBDIRECTORY_UPPER} <<< "$(grep -Ir --include="*CMakeLists.txt" --exclude-dir="*build*" "add_library(" $SUBDIRECTORY | sed 's/.*add_library(//' | sed 's/ .*//g' | sort --unique | tr '\n' ' ')"
    NEW_POSSIBLE_EXECUTABLES="POSSIBLE_EXECUTABLES_${SUBDIRECTORY_UPPER}"
    POSSIBLE_EXECUTABLES+=("${!NEW_POSSIBLE_EXECUTABLES}")
    NEW_POSSIBLE_LIBRARIES="POSSIBLE_LIBRARIES_${SUBDIRECTORY_UPPER}"
    POSSIBLE_LIBRARIES+=("${!NEW_POSSIBLE_LIBRARIES}")
done

# Additional hard-coded targets
ADDITIONAL_TARGETS="doc-generate"

# We can build executables, libraries or simply all
TARGETS="$(echo "$BUILD_ALL ${POSSIBLE_EXECUTABLES[*]}" "${POSSIBLE_LIBRARIES[*]}" "$ADDITIONAL_TARGETS" | sort -u)"

POSSIBLE_TARGETS=()
for TARGET in $TARGETS ; do
    POSSIBLE_TARGETS+=("$TARGET")
done
#
# SECOND OPTION
# We can get possible targets from Makefile but we need to run cmake first. This is NOT IMPLEMENTED YET.
# We could get a list of all targets from the following command:
# $ make -qp | awk -F':' '/^[a-zA-Z0-9][^$#\/\t=]*:([^=]|$)/ {split($1,A,/ /);for(i in A)print A[i]}'
# Then find the location of corresponding Makefiles in order to get the target's folder.
#
# THIRD OPTION
# Add targets in CMakeLists.txt using another function which exports cache variables with paths associated
# with given targets.
#

# DEFAULTS - configuration
DEFAULT_BUILD_TYPES=("release")
DEFAULT_COMPILERS=("gcc")
DEFAULT_TARGETS=("$BUILD_ALL")
DEFAULT_ARCHITECTURES=("arm")
DEFAULT_INSTALL_PREFIX="rootfs"

# DEFAULTS - others
DEFAULT_INSTALL="no"
DEFAULT_JOBS=$((($(getconf _NPROCESSORS_ONLN))))
DEFAULT_PRINT="no"
DEFAULT_QUIET="no"
DEFAULT_STYLE_CHECK="no"
DEFAULT_UNIT_TESTS="no"

function usage() {
    echo "Usage: $0 [OPTIONS], where parameters to flags are separated by commas. Example: $0 -a arm -b debug,release -c gcc,clang -t ipu-redfish-server,psme-compute"
    echo "Options:"
    echo "   Build configuration:"
    echo "      -A | --all: Builds every possible combination. If other options are used after this one, they will override it. For instance, it's possible to specify 'all' and later choose only '64' bit architecture."
    echo "      -a | --arch [NUMBER]: Specify target architecture, possible choices: [${POSSIBLE_ARCHITECTURES[*]}]. Default: ${DEFAULT_ARCHITECTURES[*]}."
    echo "      -b | --build-type [BUILD_TYPES]: Choose the build type. Possible choices: [${POSSIBLE_BUILD_TYPES[*]}]. Default: ${DEFAULT_BUILD_TYPES[*]}."
    echo "      -c | --compiler [COMPILERS]: Choose compiler. Possible choices: [${POSSIBLE_COMPILERS[*]}]. Default: ${DEFAULT_COMPILERS[*]}."
    echo "      -p | --prefix [PREFIX]: Install prefix. Default: ${DEFAULT_INSTALL_PREFIX}."
    echo "      -t | --targets [TARGETS]: Specify which TARGETS to build. This parameter makes sense only if you are in the main directory, otherwise it will only build the current folder and its TARGET_subdirectories. If $BUILD_ALL is chosen, other targets are ignored. Default: all."
    echo "   Other:"
    echo "      -h | --help : Displays this help and exits."
    echo "      -i | --install : Runs the 'install' target. Default: ${DEFAULT_INSTALL}"
    echo "      -j | --jobs [NUMBER]: Specify number of threads for make. Default: number of online processors."
    echo "      -l | --list: Lists all targets and exits."
    echo "      -n | --nop: Doesn't actually build anything, only prints what would be done. Default: ${DEFAULT_PRINT}."
    echo "      -q | --quiet: Surpresses output and only prints the build results. Default: ${DEFAULT_QUIET}."
    echo "      -s | --style: Run style check. Default: ${DEFAULT_STYLE_CHECK}."
    echo "      -x | --suffix : Adds a suffix to the build path. By default, path is as follows: BUILD_PATH=build.\${BUILD_TYPE}.\${COMPILER}.\${ARCHITECTURE}bit. The suffix is added at the end: \${BUILD_PATH}.\${SUFFIX}."
    echo "      -u | --unit-tests: Run unit tests associated with the given targets. Default: ${DEFAULT_UNIT_TESTS}."
    echo "      -D | --define [OPTION]: add a flag/option to pass to cmake. Options from \${OPTIONS} env variable are passed as well"
    echo "      -C | --clean: rebuild from the scratch"
    echo "      -L | --light-clean: remove Makefile & CMakeCache.txt and rebuild"
}

function set_default_values {
    BUILD_TYPES=("${DEFAULT_BUILD_TYPES[@]}")
    COMPILERS=("${DEFAULT_COMPILERS[@]}")
    TARGETS=("${DEFAULT_TARGETS[@]}")
    ARCHITECTURES=("${DEFAULT_ARCHITECTURES[@]}")
    INSTALL="${DEFAULT_INSTALL}"
    INSTALL_PREFIX="$DEFAULT_INSTALL_PREFIX"
    JOBS="$DEFAULT_JOBS"
    PRINT="$DEFAULT_PRINT"
    QUIET="$DEFAULT_QUIET"
    STYLE_CHECK="$DEFAULT_STYLE_CHECK"
    SUFFIX=""
    UNIT_TESTS="$DEFAULT_UNIT_TESTS"
    CMAKE_OPTIONS=""
    if [ -n "${OPTIONS:-}" ]; then
        echo "Environment options passed to make: ${OPTIONS}"
    else
        OPTIONS=""
    fi
}

function list_targets {
    echo "Listing targets for every subdirectory:"
    for SUBDIRECTORY in ${TARGET_SUBDIRECTORIES[*]} ; do
        SUBDIRECTORY_UPPER="${SUBDIRECTORY^^}"
        SUBDIRECTORY_UPPER="${SUBDIRECTORY_UPPER/-/_}"
        SUBDIRECTORY_POSSIBLE_EXECUTABLES="POSSIBLE_EXECUTABLES_${SUBDIRECTORY_UPPER}"
        SUBDIRECTORY_POSSIBLE_LIBRARIES="POSSIBLE_LIBRARIES_${SUBDIRECTORY_UPPER}"
        echo "   ${SUBDIRECTORY}:"
        echo "      Executables: ${!SUBDIRECTORY_POSSIBLE_EXECUTABLES}"
        echo "      Libraries: ${!SUBDIRECTORY_POSSIBLE_LIBRARIES}"
    done
    echo "   Additional targets: $ADDITIONAL_TARGETS"
}

function parse_input {
    while [ $# -ge 1 ] ; do
        case "$1" in
            -A|--all)
                BUILD_TYPES=("${POSSIBLE_BUILD_TYPES[@]}")
                COMPILERS=("${POSSIBLE_COMPILERS[@]}")
                TARGETS=("$BUILD_ALL")
                ARCHITECTURES=("${POSSIBLE_ARCHITECTURES[@]}")
                ;;
            -a|--arch)
                shift
                ARCHITECTURES=()
                for ARCHITECTURE in ${1//,/ } ; do
                    ARCHITECTURES+=("$ARCHITECTURE")
                done
                ;;
            -b|--build-type)
                shift
                BUILD_TYPES=()
                for BUILD_TYPE in ${1//,/ } ; do
                    BUILD_TYPES+=("$BUILD_TYPE")
                done
                ;;
            -c|--compiler)
                shift
                COMPILERS=()
                for COMPILER in ${1//,/ } ; do
                    COMPILERS+=("$COMPILER")
                done
                ;;
            -h|--help)
                usage
                exit 0
                ;;
            -i|--install)
                INSTALL="yes"
                ;;
            -j|--jobs)
                shift
                JOBS="$1"
                ;;
            -l|--list)
                list_targets
                exit 0
                ;;
            -n|--nop)
                PRINT="yes"
                ;;
            -o|--cmake-options)
                CMAKE_OPTIONS="$2"
                echo "CMAKE_OPTIONS = $CMAKE_OPTIONS"
                ;;
            -p|--prefix)
                shift
                INSTALL_PREFIX="$1"
                ;;
            -q|--quiet)
                QUIET="yes"
                ;;
            -s|--style)
                STYLE_CHECK="yes"
                ;;
            -x|--suffix)
                shift
                SUFFIX="$1"
                ;;
            -u|--unit-tests)
                UNIT_TESTS="yes"
                ;;
            -t|--targets)
                shift
                TARGETS=()
                for TARGET in ${1//,/ } ; do
                    TARGETS+=("$TARGET")
                done
                ;;
            -C|--clean)
                CLEAN=true
                ;;
            -L|--light-clean)
                LIGHT_CLEAN=true
                ;;
            -D|--define) # -D option=value form
                shift
                OPTIONS="${OPTIONS} -D$1"
                ;;
            -D*) # -Doption=value form
                OPTIONS="${OPTIONS} $1"
                ;;
            *)
                echo "Unknown parameter $1"
                usage
                exit 1
                ;;
        esac
        shift
    done
}

function build_dir {
    echo -n "build.${BUILD_TYPE}.${COMPILER}"
    case ${ARCHITECTURE} in
        64)
            echo -n ".${ARCHITECTURE}bit"
            ;;
        *)
            echo -n ".${ARCHITECTURE}"
            ;;
    esac
    if [[ -n "${SUFFIX-}" ]] ; then
        echo -n ".${SUFFIX}"
    fi
    echo ""
}

function prepare {
    # Validate parameters
    if [[ $JOBS -lt 1 ]] ; then
        echo "Incorrect jobs number - $JOBS"
        exit 1
    fi

    for BUILD_TYPE in "${BUILD_TYPES[@]}" ; do
        BUILD_TYPE_FOUND=$NOT_FOUND
        for POSSIBLE_BUILD_TYPE in "${POSSIBLE_BUILD_TYPES[@]}" ; do
            if [[ $BUILD_TYPE == "$POSSIBLE_BUILD_TYPE" ]] ; then
                BUILD_TYPE_FOUND=$FOUND
            fi
        done
        if [[ $BUILD_TYPE_FOUND == "$NOT_FOUND" ]] ; then
            echo "Unknown build type $BUILD_TYPE detected, exiting..."
            exit 1
        fi
    done

    for COMPILER in "${COMPILERS[@]}" ; do
        CMP_FOUND=$NOT_FOUND
        for POSSIBLE_COMPILER in "${POSSIBLE_COMPILERS[@]}" ; do
            if [[ $COMPILER == "$POSSIBLE_COMPILER" ]] ; then
                CMP_FOUND=$FOUND
            fi
        done
        if [[ $CMP_FOUND == "$NOT_FOUND" ]] ; then
            echo "Unknown compiler $COMPILER detected, exiting..."
            exit 1
        fi
    done

    for ARCHITECTURE in "${ARCHITECTURES[@]}" ; do
        ARCH_FOUND=$NOT_FOUND
        for POSSIBLE_ARCHITECTURE in "${POSSIBLE_ARCHITECTURES[@]}" ; do
            if [[ $ARCHITECTURE == "$POSSIBLE_ARCHITECTURE" ]] ; then
                ARCH_FOUND=$FOUND
            fi
        done
        if [[ $ARCH_FOUND == "$NOT_FOUND" ]] ; then
            echo "Unknown target architecture $ARCHITECTURE detected, exiting..."
            exit 1
        fi
    done

    for TARGET in "${TARGETS[@]}" ; do
        TARGET_FOUND=$NOT_FOUND
        for POSSIBLE_TARGET in "${POSSIBLE_TARGETS[@]}" ; do
            if [[ $TARGET == "$POSSIBLE_TARGET" ]] ; then
                TARGET_FOUND=$FOUND
            fi
        done
        if [[ $TARGET_FOUND == "$NOT_FOUND" ]] ; then
            echo "Unknown target $TARGET detected, exiting..."
            exit 1
        fi
    done

    # Print summary
    echo ""
    echo "BUILD TYPES:  ${BUILD_TYPES[*]}"
    echo "COMPILERS:    ${COMPILERS[*]}"
    echo "TARGET ARCH:  ${ARCHITECTURES[*]}"
    echo "TARGETS:      ${TARGETS[*]}"
    echo "JOBS:         ${JOBS}"
    echo "QUIET:        ${QUIET}"
    echo "STYLE_CHECK:  ${STYLE_CHECK}"
    echo "UNIT TESTS:   ${UNIT_TESTS}"
    echo ""

    if [[ -z "${BUILD_TYPES[@]}" || -z "${COMPILERS[@]}" || -z "${ARCHITECTURES[@]}" || -z "${TARGETS[@]}" ]] ; then
        echo "At least one of the required options is empty, exiting..."
        exit 1
    fi

    # Exit if the script was launched only to print
    if [[ $PRINT == 'yes' ]] ; then
        echo "Exiting due to --print set..."
        exit 0
    fi

    # Setup results configurations
    for ARCHITECTURE in "${ARCHITECTURES[@]}" ; do
        for COMPILER in "${COMPILERS[@]}" ; do
            for BUILD_TYPE in "${BUILD_TYPES[@]}" ; do
                # Coverage only for GCC
                if [[ $BUILD_TYPE == "coverage" && $COMPILER != "gcc" ]] ; then continue ; fi
                # Sanitize only for GCC
                if [[ $BUILD_TYPE == "asanitize" && $COMPILER != "gcc" ]] ; then continue ; fi
                if [[ $BUILD_TYPE == "tsanitize" && $COMPILER != "gcc" ]] ; then continue ; fi

                BUILD_DIR="$(build_dir)"
                RESULTS_CONFIGURATIONS+=("${BUILD_DIR}")
            done
        done
    done
}

function report
{
    CMAKE_RESULT=$1
    CMAKE_TIME=$2
    MAKE_RESULT=$3
    MAKE_TIME=$4
    UNIT_TEST_RESULT=$5
    STYLE_CHECK_RESULT=$6

    RESULTS_CMAKE_STATUSES+=("$CMAKE_RESULT")
    RESULTS_CMAKE_TIMES+=("$CMAKE_TIME")
    RESULTS_MAKE_STATUSES+=("$MAKE_RESULT")
    RESULTS_MAKE_TIMES+=("$MAKE_TIME")
    RESULTS_UNIT_TEST_STATUSES+=("$UNIT_TEST_RESULT")
    RESULTS_STYLE_CHECK_STATUSES+=("$STYLE_CHECK_RESULT")
}

function run_unit_tests
{
    RETCODE=0
    UT_TARGETS=(`cat unittest_targets.txt`)
    echo "Running tests: ${UT_TARGETS[@]}"
    for TARGET in "${UT_TARGETS[@]}" ; do
        pushd bin/tests
        ./${TARGET} --gtest_output="xml:${TARGET}.xml"
        if [ $? -ne 0 ]; then
            RETCODE=1
        fi
        popd
    done
    return $RETCODE
}

function build
{
    # Setup build directory
    BUILD_DIR="$(build_dir)"
    ${CLEAN} && rm -Rf "$BUILD_DIR"
    ${LIGHT_CLEAN} && rm -Rf "$BUILD_DIR/Makefile" && rm -Rf "$BUILD_DIR/CMakeCache.txt"
    if [ ! -d "$BUILD_DIR" ]; then
        mkdir "$BUILD_DIR"
    fi

    cd "$BUILD_DIR"
    if [[ ! -f Makefile ]] ; then
        # Generate build directories if there is no Makefile

        case "${ARCHITECTURE}" in
            64)
                CMAKE=$(which cmake)
                if [[ $COMPILER == "clang" ]] ; then
                    CC=$(which clang)
                    CXX=$(which clang++)
                elif [[ $COMPILER == "gcc" ]] ; then
                    CC=$(which gcc)
                    CXX=$(which g++)
                else
                    CC=""
                    CXX=""
                fi
                CMAKE_COMMAND="$CMAKE -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX"
                CMAKE_COMMAND="$CMAKE_COMMAND -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX -DCMAKE_CXX_FLAGS=-m64 -DCMAKE_C_FLAGS=-m64"
                ;;
            arm)
                set +u
                source /toolchain/crosstool/environment-setup-aarch64-intel-linux-gnu
                set -u
                CMAKE=$(which cmake)
                CMAKE_COMMAND="$CMAKE -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX"
                ;;
        esac

        # OPTIONS are passed to compilation, so either setting environment
        # variable or passing -D to script will work.
        export OPTIONS
        CMAKE_COMMAND="$CMAKE_COMMAND $CMAKE_OPTIONS .."
        # Run CMAKE
        if [[ $QUIET == 'yes' ]] ; then
            CMAKE_TIME=$({ time $CMAKE_COMMAND &> /dev/null ; } 2>&1)
            RESULT=$?
        else
            # The redirection is done in order to show output from cmake and be able to get execution time
            exec 3>&1 4>&2
            CMAKE_TIME=$({ time $CMAKE_COMMAND 1>&3 2>&4 ; } 2>&1)
            RESULT=$?
            # If cmake failed then just fail the build
            if [[ $RESULT != 0 ]] ; then
                report "$STATUS_FAILURE" "$CMAKE_TIME" "$STATUS_SKIPPED" 0  "$STATUS_SKIPPED" "$STATUS_SKIPPED"
                cd "$STARTING_DIR"
                return $RESULT
            fi
            # Revert changes in redirection
            exec 3>&- 4>&-
        fi
    else
        # If there is a Makefile then we can safely go with just "make"
        CMAKE_TIME=0
        RESULT=0
    fi

    # Set jobs and targets
    if [[ $INSTALL == "yes" ]] ; then
        TARGETS+=("install")
    fi

    MAKE=$(which make)
    MAKE_COMMAND="$MAKE -j$JOBS ${TARGETS[*]} VERBOSE=1"

    # Run MAKE
    if [[ $QUIET == 'yes' ]] ; then
        MAKE_TIME=$({ time $MAKE_COMMAND &> /dev/null ; } 2>&1)
        RESULT=$?
    else
        exec 3>&1 4>&2
        MAKE_TIME=$({ time $MAKE_COMMAND 1>&3 2>&4 ; } 2>&1)
        RESULT=$?
        exec 3>&- 4>&-
    fi

    # Report failed make
    if [[ $RESULT != 0 ]] ; then
        report "$STATUS_SUCCESS" "$CMAKE_TIME" "$STATUS_FAILURE" "$MAKE_TIME" "$STATUS_SKIPPED" "$STATUS_SKIPPED"
        cd "$STARTING_DIR"
        return $RESULT
    fi

    # Run unit tests
    if [[ $UNIT_TESTS == "yes" ]] ; then
        run_unit_tests
        # Mark failed unit tests
        RESULT=$?
        if [[ $RESULT -eq 255 ]]; then
            UNIT_TESTS_STATUS="$STATUS_SKIPPED"
        elif [[ $RESULT -eq 0 ]] ; then
            UNIT_TESTS_STATUS="$STATUS_SUCCESS"
        else
            UNIT_TESTS_STATUS="$STATUS_FAILURE"
        fi
    else
        UNIT_TESTS_STATUS="$STATUS_SKIPPED"
    fi

    # Run style check; mock method for now
    if [[ $STYLE_CHECK == "yes" ]] ; then
        STYLE_CHECK_STATUS="$STATUS_SUCCESS"
    else
        STYLE_CHECK_STATUS="$STATUS_SKIPPED"
    fi

    # Report status and go back to original directory
    report "$STATUS_SUCCESS" "$CMAKE_TIME" "$STATUS_SUCCESS" "$MAKE_TIME" "$UNIT_TESTS_STATUS" "$STYLE_CHECK_STATUS"

    cd "$STARTING_DIR"

    RET=0
    if [[ $UNIT_TESTS_STATUS == "$STATUS_FAILURE" ]] ; then
        RET=1
    fi

    return $RET
}

function build_all
{
    for ARCHITECTURE in "${ARCHITECTURES[@]}" ; do
        for COMPILER in "${COMPILERS[@]}" ; do
            for BUILD_TYPE in "${BUILD_TYPES[@]}" ; do
                # Coverage only for GCC
                if [[ $BUILD_TYPE == "coverage" && $COMPILER != "gcc" ]] ; then continue ; fi
               # Sanitize only for GCC
                if [[ $BUILD_TYPE == "asanitize" && $COMPILER != "gcc" ]] ; then continue ; fi
                if [[ $BUILD_TYPE == "tsanitize" && $COMPILER != "gcc" ]] ; then continue ; fi

                build $BUILD_TYPE
            done
        done
    done
}

function report_results
{
    echo ""
    echo ""
    echo "+       CONFIGURATION       +  CMAKE  +  TIME  +  MAKE   +  TIME  + UNIT TEST + STYLE CHECK +"
    for ((i = 0; i < ${#RESULTS_CONFIGURATIONS[@]}; i++)) ; do
        RESULTS_CONFIGURATION="${RESULTS_CONFIGURATIONS[$i]}"
        CMAKE_STATUS="${RESULTS_CMAKE_STATUSES[$i]}"
        MAKE_STATUS="${RESULTS_MAKE_STATUSES[$i]}"
        UNIT_TEST_STATUS="${RESULTS_UNIT_TEST_STATUSES[$i]}"
        STYLE_CHECK_STATUS="${RESULTS_STYLE_CHECK_STATUSES[$i]}"
        CMAKE_TIME="${RESULTS_CMAKE_TIMES[$i]}"
        MAKE_TIME="${RESULTS_MAKE_TIMES[$i]}"

        printf "| %-25s |" $RESULTS_CONFIGURATION

        if [[ $CMAKE_STATUS == "$STATUS_SUCCESS" ]] ; then
            printf " ${GREEN}%-7s${COLOR_END} |" "$STATUS_SUCCESS"
        elif [[ $CMAKE_STATUS == "$STATUS_FAILURE" ]] ; then
            printf " ${RED}%-7s${COLOR_END} |" "$STATUS_FAILURE"
            RETVAL=1
        elif [[ $CMAKE_STATUS == "$STATUS_SKIPPED" ]] ; then
            printf " ${YELLOW}%-7s${COLOR_END} |" "$STATUS_SKIPPED"
        fi

        printf "  %5s |" $CMAKE_TIME

        if [[ $MAKE_STATUS == "$STATUS_SUCCESS" ]] ; then
            printf " ${GREEN}%-7s${COLOR_END} |" "$STATUS_SUCCESS"
        elif [[ $MAKE_STATUS == "$STATUS_FAILURE" ]] ; then
            printf " ${RED}%-7s${COLOR_END} |" "$STATUS_FAILURE"
            RETVAL=1
        elif [[ $MAKE_STATUS == "$STATUS_SKIPPED" ]] ; then
            printf " ${YELLOW}%-7s${COLOR_END} |" "$STATUS_SKIPPED"
        fi

        printf "  %5s |" $MAKE_TIME

        if [[ $UNIT_TEST_STATUS == "$STATUS_SUCCESS" ]] ; then
            printf "  ${GREEN}%-7s${COLOR_END}  |" "$STATUS_SUCCESS"
        elif [[ $UNIT_TEST_STATUS == "$STATUS_FAILURE" ]] ; then
            printf "  ${RED}%-7s${COLOR_END}  |" "$STATUS_FAILURE"
            RETVAL=1
        elif [[ $UNIT_TEST_STATUS == "$STATUS_SKIPPED" ]] ; then
            printf "  ${YELLOW}%-7s${COLOR_END}  |" "$STATUS_SKIPPED"
        fi

        if [[ $STYLE_CHECK_STATUS == "$STATUS_SUCCESS" ]] ; then
            printf "   ${GREEN}%-7s${COLOR_END}   |" "$STATUS_SUCCESS"
        elif [[ $STYLE_CHECK_STATUS == "$STATUS_FAILURE" ]] ; then
            printf "   ${RED}%-7s${COLOR_END}   |" "$STATUS_FAILURE"
            RETVAL=1
        elif [[ $STYLE_CHECK_STATUS == "$STATUS_SKIPPED" ]] ; then
            printf "   ${YELLOW}%-7s${COLOR_END}   |" "$STATUS_SKIPPED"
        fi

        printf "\n"

    done

    TIME=$((SECONDS - TIME))
    echo ""
    echo "Total execution time: $TIME seconds"
}

function sigint_handler
{
    # Kill all children processes
    CHILD_PROCESSES=$(pstree -p $$ | grep -o '([0-9]\+)' | grep -o '[0-9]\+')
    for CHILD_PROCESS in $CHILD_PROCESSES ; do
        if [[ $CHILD_PROCESS == "$$" ]] ; then continue ; fi
        kill -9 "$CHILD_PROCESS" 2> /dev/null
    done

    # Set remaining steps to 'skipped'
    CONFIGURATIONS_COUNT=${#RESULTS_CONFIGURATIONS[@]}
    while [[ ${#RESULTS_MAKE_STATUSES[@]} -lt $CONFIGURATIONS_COUNT ]] ; do
        RESULTS_MAKE_STATUSES+=("$STATUS_SKIPPED")
    done

    while [[ ${#RESULTS_CMAKE_STATUSES[@]} -lt $CONFIGURATIONS_COUNT ]] ; do
        RESULTS_CMAKE_STATUSES+=("$STATUS_SKIPPED")
    done

    while [[ ${#RESULTS_UNIT_TEST_STATUSES[@]} -lt $CONFIGURATIONS_COUNT ]] ; do
        RESULTS_UNIT_TEST_STATUSES+=("$STATUS_SKIPPED")
    done

    # Set times to 0
    while [[ ${#RESULTS_MAKE_TIMES[@]} -lt $CONFIGURATIONS_COUNT ]] ; do
        RESULTS_MAKE_TIMES+=("0")
    done

    while [[ ${#RESULTS_CMAKE_TIMES[@]} -lt $CONFIGURATIONS_COUNT ]] ; do
        RESULTS_CMAKE_TIMES+=("0")
    done

    # Report final results
    report_results
    exit 1
}

#######################################
#          MAIN
#######################################

set_default_values
parse_input "$@"
prepare
trap sigint_handler INT
build_all
report_results
exit "${RETVAL:-0}"
