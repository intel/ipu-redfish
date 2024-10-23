|ipu-redfish| Supported APIs and use cases
==========================================

Terms
-----

Image Repository - web server serving IMC PLDM files and/or ACC ISO images.

List of Supported Redfish Endpoints
-----------------------------------

The following table contains the list of endpoints supported by the
server, as well as which HTTP methods are supported for each of them.

+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| Endpoint                                                                          | GET | PATCH | POST | DELETE |
+===================================================================================+=====+=======+======+========+
| /redfish                                                                          | Yes |       |      |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1                                                                       | Yes |       |      |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1/TaskService                                                           | Yes |       |      |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1/TaskService/Tasks                                                     | Yes |       |      |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1/TaskService/Tasks/{id}                                                | Yes |       |      |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1/TaskService/TaskMonitors/{id}                                         | Yes |       |      |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1/UpdateService                                                         | Yes |       |      |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1/UpdateService/Actions/UpdateService.SimpleUpdate                      |     |       | Yes  |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1/Systems/{id}/VirtualMedia/{id}/Actions/VirtualMedia.InsertMedia       |     |       | Yes  |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1/Systems/{id}/Actions/ComputerSystem.Reset                             |     |       | Yes  |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1/Managers/{id}/Actions/Manager.Reset                                   |     |       | Yes  |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1/SessionService                                                        | Yes | Yes   |      |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1/SessionService/Sessions                                               |     |       | Yes  |        |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+
| /redfish/v1/SessionService/Sessions/{id}                                          | Yes | Yes   |      | Yes    |
+-----------------------------------------------------------------------------------+-----+-------+------+--------+


Supported Endpoints in Detail
-----------------------------

Systems
~~~~~~~

The Systems endpoint represents the computer systems managed by the Redfish server.
It provides operations for querying and modifying the system's properties and status.
The following operations are supported for the Systems endpoint:

Override boot source
   - Endpoint: ``/redfish/v1/Systems/{id}``
   - Method: PATCH

+--------------------------+--------+----------+-----------------------------------+
| Parameter                | Type   | Required | Comment                           |
+==========================+========+==========+===================================+
| BootSourceOverrideEnabled| String | Yes      | "Disabled" / "Once" / "Continuous"|
+--------------------------+--------+----------+-----------------------------------+
| BootSourceOverrideTarget | String | Yes      | "Cd" or "None"                    |
+--------------------------+--------+----------+-----------------------------------+


Reset the system
   - Endpoint: ``/redfish/v1/Systems/{id}/Actions/ComputerSystem.Reset``
   - Method: POST

+---------------------+--------+----------+------------------------------------+
| Parameter           | Type   | Required | Comment                            |
+=====================+========+==========+====================================+
| ComputerSystem.Reset| String | Yes      | GracefulShutdown or GracefulRestart|
+---------------------+--------+----------+------------------------------------+


Insert ACC ISO image into virtual media
   - Endpoint: ``/redfish/v1/Systems/{id}/VirtualMedia/{id}/Actions/VirtualMedia.InsertMedia``
   - Method: POST

+---------------+--------+----------+-------------------------------------------------------------+
| Parameter     | Type   | Required | Comment                                                     |
+===============+========+==========+=============================================================+
| Image         | String | Yes      | The URI of the software image to install                    |
+---------------+--------+----------+-------------------------------------------------------------+
| TransferMethod| String | Yes      | Only "Upload" is supported                                  |
+---------------+--------+----------+-------------------------------------------------------------+
| UserName      | String | No       | The username to access the URI specified by the `Image`     |
|               |        |          | parameter                                                   |
+---------------+--------+----------+-------------------------------------------------------------+
| Password      | String | No       | The password to access the URI specified by the `Image`     |
|               |        |          | parameter                                                   |
+---------------+--------+----------+-------------------------------------------------------------+


Session Management
~~~~~~~~~~~~~~~~~~

Session management in Redfish allows clients to establish and maintain sessions with the Redfish server.
Sessions provide a way to authenticate and authorize clients to access and interact with the server's resources.

To manage sessions in Redfish, the following endpoints are available:

- ``/redfish/v1/SessionService`` - This endpoint represents the session service and provides operations for
  creating, querying, and deleting sessions.

- ``/redfish/v1/SessionService/Sessions`` - This endpoint allows clients to query the list of active sessions.

- ``/redfish/v1/SessionService/Sessions/{id}`` - This endpoint represents an individual session and provides
  operations for querying and deleting a specific session.

The ``SessionService`` endpoint allows clients to modify the state of the Redfish session service by sending a PATCH request.

+----------------+-------+----------+-----------------------------------------+
| Parameter      | Type  | Required | Comment                                 |
+================+=======+==========+=========================================+
| ServiceEnabled | Bool  | No       | "true" or "false"                       |
+----------------+-------+----------+-----------------------------------------+
| SessionTimeout | Int64 | No       | Session duration in seconds (30-86400)  |
+----------------+-------+----------+-----------------------------------------+


To create a new session, clients can send a POST request to the ``/redfish/v1/SessionService/Sessions`` endpoint
with the necessary authentication credentials. The server will validate the credentials and if successful,
create a new session and return a session token in the response.

Clients can then use the session token to include it in subsequent requests as an ``X-Auth-Token`` header to
authenticate and authorize their access to protected resources. The server will validate the session token
and allow or deny access based on the client's privileges.

To query the list of active sessions, clients can send a GET request to the ``/redfish/v1/SessionService/Sessions``
endpoint. The server will return a list of active sessions along with their details such as session ID, user account,
and expiration time.

To delete a session, clients can send a DELETE request to the ``/redfish/v1/SessionService/Sessions/{id}`` endpoint,
where ``{id}`` is the session ID. The server will invalidate the session and remove it from the list of active sessions.

It's important for clients to manage their sessions properly, including handling session expiration and securely
storing session tokens to prevent unauthorized access to their resources.

SimpleUpdate Action
~~~~~~~~~~~~~~~~~~~

The Action endpoint is available at the ``/redfish/v1/UpdateService/Actions/UpdateService.SimpleUpdate`` URI.

The action can be used to request the server to download a PLDM image
for IMC update from some image repository and install it. HTTP and HTTPS
URLs are supported. For HTTPS, you must make sure that the IMC is able
to verify the certificate of your image repository. The new IMC firmware
will become active after IMC reboot.

.. Note:: IMC Recovery image is not updated.


The endpoint accepts the POST method. The request body should contain
the following parameters:

+-----------+--------+----------+------------------------------------------------------------------+
| Parameter | Type   | Required | Comment                                                          |
+===========+========+==========+==================================================================+
| ImageURI  | String | Yes      | The URI of the software image to install                         |
+-----------+--------+----------+------------------------------------------------------------------+
| Username  | String | No       | The username to access the URI specified by the `ImageURI`       |
|           |        |          | parameter                                                        |
+-----------+--------+----------+------------------------------------------------------------------+
| Password  | String | No       | The password to access the URI specified by the `ImageURI`       |
|           |        |          | parameter                                                        |
+-----------+--------+----------+------------------------------------------------------------------+


Example:

.. code:: json

   {"ImageURI" : "https://100.0.0.1/imc/intel-ipu-pldm.bin"}

Response:

The Action is asynchronous - it creates a Task resource. The server
respons with HTTP code ``202 Accepted``, a Location header pointing to the
Task Monitor for the created task, and the JSON representation of the
Task in the response body.

Manager Reset Action
~~~~~~~~~~~~~~~~~~~~

The Action is available at the
``/redfish/v1/Managers/{id}/Actions/Manager.Reset`` URI.

In our case, there is only one Manager resource `/redfish/v1/Managers/1`,
representing the IMC.

.. Note:: Triggering this action makes the Redfish server unresponsive
   as it itself resides on the IMC.

..

.. Note:: No further requests should be sent to the server after
   requesting IMC reboot.

The endpoint accepts the POST method. The request body should contain
the following parameters:

+-----------+--------+----------+-------------------------------------------+
| Parameter | Type   | Required | Comment                                   |
+===========+========+==========+===========================================+
| ResetType | String | Yes      | The only supported type is "ForceRestart" |
+-----------+--------+----------+-------------------------------------------+


Example:

.. code:: json

   {"ResetType" : "ForceRestart"}

Response:

The Action returns a ``204 No Content`` response.

AccountService
~~~~~~~~~~~~~~

The AccountService endpoint is available at the ``/redfish/v1/AccountService`` URI. It provides operations for managing user accounts and authentication settings.

Supported Endpoints:

* ``/redfish/v1/AccountService`` - represents the account service and provides operations for creating, querying, and modifying user accounts.
* ``/redfish/v1/AccountService/Accounts`` - allows clients to query the list of user accounts.
* ``/redfish/v1/AccountService/Accounts/{id}`` - represents an individual user account and provides operations for querying and modifying a specific account.
* ``/redfish/v1/AccountService/Roles`` - allows clients to query the list of roles available in the AccountService.

AccountService Endpoints
~~~~~~~~~~~~~~~~~~~~~~~~

The IPU Redfish Server provides the following endpoints for managing user accounts and authentication settings; all endpoints support GET method with no request body.

``/redfish/v1/AccountService``

Returns a ``200 OK`` response along with the AccountService resource.

``/redfish/v1/AccountService/Accounts``

Returns a ``200 OK`` response along with the ManagerAccountCollection resource listing all existing user accounts.

``/redfish/v1/AccountService/Accounts/{id}``

If the user account exists, returns a ``200 OK`` response along with the ManagerAccount resource (with the password replaced by null JSON value).
If the user account does not exist, returns a ``404 Not Found`` response along with an appropriate error body.

``/redfish/v1/AccountService/Roles``

Returns a ``200 OK`` response along with the RoleCollection resource listing all existing Roles.

``/redfish/v1/AccountService/Roles/Administrator``

Returns a ``200 OK`` response along with the Administrator Role resource.

``/redfish/v1/AccountService/Roles/{other role}``

Returns a ``404 Not Found`` response along with an appropriate error body if the specified role does not exist.

Use cases
---------

IMC Update
~~~~~~~~~~

To update the IMC using the server, execute the following steps:

#. Set up an Image Repository in a network location that will be reachable by your IMC.
#. Trigger the SimpleUpdate Action.
#. Poll the Task Monitor URL until the response code changes to ``204 No Content``.
#. Reset the IMC using the Manager Reset Action on the singular Manager resource representing the IMC.
#. After the reset, the IMC is updated. Verify by checking the contents of the ``/etc/issue`` file.

ACC Provisioning
~~~~~~~~~~~~~~~~

To provision ACC using the Redfish API, the following steps can be followed:

#. Insert ACC ISO image into virtual media
#. Set boot source to CD
#. Reset the IMC.

Example using curl

.. code:: bash

     curl -X POST https://imc:8443/redfish/v1/Systems/1/VirtualMedia/1/Actions/VirtualMedia.InsertMedia \
     -d '{ "Image": "https://100.0.0.1/some_acc.iso", "TransferMethod": "Upload", "UserName": "user", "Password": "pass" }' \
     -H "Content-Type: application/json"

     curl -X PATCH https://imc:8443/redfish/v1/Systems/1 \
     -d '{"Boot":{"BootSourceOverrideEnabled": "Once", "BootSourceOverrideTarget": "Cd" }}' \
     -H "Content-Type: application/json"

     curl -X POST https://imc:8443/redfish/v1/Managers/1/Actions/Manager.Reset \
     -d '{"ResetType": "ForceRestart"}' \
     -H "Content-Type: application/json"
