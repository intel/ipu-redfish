PSME Development environment
============================

== This document is out of date ==

The PSME software depends on several libraries and specific OS settings.
This chapter describes precise software versions required in order to
compile and run the PSME software.

Requirements
------------

The PSME Software was developed in **C++14** language targeting Linux
based systems. The whole build process is managed by the **CMake** tool.

[Table CAPTION_NUMBER(#software-versions-to-compile-the-psme-on-linux)](#software-versions-to-compile-the-psme-on-linux) shows the software versions required to compile the PSME on Linux based
systems.

CAPTION(Table, Software Versions to Compile the PSME on Linux)
<table>
    <tr>
        <th><div custom-style="TableHeader">Software</div></th>
        <th><div custom-style="TableHeader">Version</div></th>
    </tr>
    <tr>
        <td>CMake</td>
        <td>$\ge$ 3.4.3</td>
    </tr>
    <tr>
        <td>gcc</td>
        <td>5.3.1 $\le$ $\le$ 5.4.0</td>
    </tr>
</table>

The libraries shown in [Table CAPTION_NUMBER(#libraries-to-install-prior-to-psme-compilation)](#libraries-to-install-prior-to-psme-compilation) must be installed prior to the PSME compilation.


CAPTION(Table, Libraries to Install Prior to PSME Compilation)
<table>
    <tr>
        <th><div custom-style="TableHeader">Software</div></th>
        <th><div custom-style="TableHeader">Version</div></th>
    </tr>
    <tr>
        <td>curl</td>
        <td>default repository version</td>
    </tr>
    <tr>
        <td>microhttpd</td>
        <td>default repository version</td>
    </tr>
    <tr>
        <td>LVM2</td>
        <td>default repository version</td>
    </tr>
</table>

If any of the libraries are missing, the CMake script attempts to download the source package from public software
repositories on the Internet and automatically compile it. Confirm that the server network, firewall, and proxy
configurations allow the appropriate server access to external software vendor sites.


Compilation
-----------

Decompress the PSME source code package. Make sure that you have an Internet connection with proxy servers set if needed for http, https and ftp protocols.
Then download all external dependencies to the `third_party` folder:

    cd third_party
    ./download.sh
    cd ..

One dependency needs to be downloaded manually. To obtain it, create a user account at the [official Total Phase website](https://www.totalphase.com).
When you're able to log in, navigate to [https://www.totalphase.com/products/aardvark-software-api](https://www.totalphase.com/products/aardvark-software-api), download **Aardvark* Software API v5.30 (Linux 64-bit)**
and put it in the `third_party` folder. It's important to do it **after** calling download.sh script to avoid overwriting the content of the correct archive.

All PSME modules must be built from the main directory using
**make [target]**. First, prepare the build directory using CMake.
Creating a build directory with CMake is a one-time operation.

Building release version:

    mkdir build.release
    cd build.release
    cmake ..

Building debug version:

    mkdir build.debug
    cd build.debug
    cmake -DCMAKE_BUILD_TYPE=Debug ..

CMake enables you to pass additional parameters like target
architecture, compiler, and many others. For more information, refer to
**man cmake**.


<div custom-style="Note">Note: All PSME modules must be built from the previously prepared build directory (`build.debug` or `build.release`).</div>

Perform the following steps to build the PSME Rest Server, all associated agents, stubs, and examples:

    cd <PSME root>/<build directory>
    make all -j8

<div custom-style="Note">Note: Specify the number of parallel jobs `n` for faster compilation using the `j` flag.</div>

To build only a subset of modules, for example
only psme-rest-server and psme-chassis, use the following alternative command:

    make psme-rest-server psme-chassis -j8

To get a list of all possible targets to build, run command in the build directory:

    make -qp | awk -F':' '/^[a-zA-Z0-9][^$#\/\t=]*:([^=]|$)/ {split($1,A,/ /);for(i in A)print A[i]}'

Running unit testing (all build types):

    ctest

Generating documentation:

    make doc-generate

Reading documentation:

    YOUR_WEB_BROWSER doc/html/index.html

