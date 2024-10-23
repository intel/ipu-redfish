Overview
========

== This document is out of date ==

The interfaces specified in this document are based on the Distributed
Management Task Force’s Redfish™ Interface Specification and schema
(Refer to [Table CAPTION_NUMBER(#reference-documents)](#reference-documents).

Document Scope
--------------

This document is a full and detailed documentation of the Pooled
System Management Engine (PSME) Software. Information below
covers minimal requirements for hardware and software during
compilation and runtime. The document contains instructions
for compilation, installation, deployment, and configuration
of the PSME Software on various supported system environments.

The following topics will be covered in this documentation:

-   PSME Software overview

-   Hardware requirements and software prerequisites

-   PSME Software Installation and deployment

-   Hardware and PSME Software Configuration

Intended Audience
-----------------

Introduction
------------

The PSME Software consists of:

-   **PSME REST server** - HTTP server with Representational State Transfer (REST) API and JavaScript* Object
       Notation (JSON*) data containers responsible for gathering and presenting information about assets and
       available operations on these assets.


Definition of Terms
-------------------

CAPTION(Table, Terminology)

<table>
    <tr>
        <th><div custom-style="TableHeader">Term</div></th>
        <th><div custom-style="TableHeader">Definition</div></th>
    </tr>
    <tr>
        <td>API</td>
        <td>Application Program Interface</td>
    </tr>
    <tr>
        <td>BIOS</td>
        <td>Basic Input/Output System</td>
    </tr>
    <tr>
        <td>CA</td>
        <td>Certificate Authority</td>
    </tr>
    <tr>
        <td>HTTPS</td>
        <td>Hypertext Transfer Protocol Secure</td>
    </tr>
    <tr>
        <td>iSCSI</td>
        <td>Internet Small Computer Systems Interface</td>
    </tr>
    <tr>
        <td>NIC</td>
        <td>Network Interface Controller</td>
    </tr>
    <tr>
        <td>NTP</td>
        <td>Network Timer Protocol</td>
    </tr>
    <tr>
        <td>OS</td>
        <td>Operating System</td>
    </tr>
    <tr>
        <td>PXE</td>
        <td>Pre-boot eXecution Environment</td>
    </tr>
    <tr>
        <td>REST</td>
        <td>Representational state transfer</td>
    </tr>
    <tr>
        <td>TLS</td>
        <td>Transport Layer Security</td>
    </tr>
    <tr>
        <td>UUID</td>
        <td>Universally unique identifier</td>
    </tr>
    <tr>
        <td>XML</td>
        <td>Extensible Markup Language</td>
    </tr>
</table>

References
----------

CAPTION(Table, Reference documents)

<table>
    <tr>
    	<td>Doc ID</td>
    	<td>Title</td>
    	<td>Location</td>
    </tr>
    <tr>
        <td>DSP8010</td>
        <td>Redfish Schema v2018.3</td>
        <td>https://www.dmtf.org/sites/default/files/standards/documents/DSP8010_2018.3.zip</td>
    </tr>
    <tr>
        <td>ISO 8601</td>
        <td>Date and time format - ISO 8601</td>
        <td>https://www.iso.org/iso-8601-date-and-time-format.html</td>
    </tr>
    <tr>
        <td>N/A</td>
        <td>Hypertext transfer Protocol - HTTP/1.1
            Obsoletes IETF 2145, 2616, and Updates IETF 2817, 2818</td>
        <td>https://tools.ietf.org/html/rfc7230
            See RFC 7230-7235.</td>
    </tr>
    <tr>
        <td>RFC2119</td>
        <td>Key Words for Use in RFCs to Indicate Requirement Levels, March 1997</td>
        <td>https://ietf.org/rfc/rfc2119.txt</A></td>
    </tr>
</table>


Conventions
-------------------------

The key words/phrases "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD", "SHOULD NOT",
"RECOMMENDED", "MAY", and "OPTIONAL" in this document are to be interpreted as described in Key Words for
Use in RFCs to Indicate Requirement Levels, March 1997, RFC 2119, refer to [Table CAPTION_NUMBER(#reference-documents)](#reference-documents).

Notes and Symbol Convention
-------------------------

Symbol and note convention are similar to typographical conventions used in the Cloud Infrastructure Management Interface
(CIMI) specification, refer to [Table CAPTION_NUMBER(#reference-documents)](#reference-documents). The notation used in JSON serialization description:

-   Values in italics indicate data types instead of literal values.

-   Characters are appended to items to indicate cardinality:
    -   "?" (0 or 1)
    -   "*" (0 or more)
    -   "+" (1 or more)

-   Vertical bars, "|", denote choice. For example, "a|b" means a choice between "a" and "b".

-   Parentheses, "(" and ")", are used to indicate the scope of the operators "?", "*", "+" and "|".

-   Ellipses (i.e. "...") indicate points of extensibility. Note that the lack of an ellipses does not mean no
extensibility point exists, rather it is just not explicitly called out.
