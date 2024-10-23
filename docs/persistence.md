# Tree stability in REST application #

== This document is out of date ==

To keep IDs consistent with resource UUIDs there is a database to keep mapping between these two.
This mapping follows IdPolicies rules:
- each resource is identified by resource type
- resources with hierarchical dependencies are grouped with parent identification.

Additionally it the database there are stored last assigned IDs. These are used to compute IDs for new
resources: first free ID bigger than last is assumed.

Each existing resource ID might be in two states:
- valid, when resource is reported
- invalid, when resource is removed or is not reported.
Invalid entries older than configured time ("outdated") are removed permamently.

There are "two sides" of maintaining IDs:
- handling events (to add and remove resources)
- retention policy to keep database compact during runtime and between application runs.

## Events ##

Application handles events received from agents (either on change or on polling).

Stored resource IDs are not removed immediatelly from the database, they are stored for same
resources (with identical UUIDs). The only case when resources are removed imediatelly is
"outdate" interval configured to 0.


@startuml
database Manager
actor Agents
actor Task
control IdPolicy
database Persistence

== New resource ==
hnote over Agents: new resource
note right
    If (invalid) entity for given UUID exists, the ID is assigned to the resource.
    Otherwise new ID is assigned.
    Last assigned ID for the parent and resource type is stored
end note
Agents -> IdPolicy: get resource ID
group Check UUID
    IdPolicy -> Persistence: get parent's last assigned ID
    IdPolicy -> Persistence: get (invalid) resource ID
    Persistence --> IdPolicy: assume resource ID or last + 1 if resource ID doesn't exist
    rnote over IdPolicy: find first unallocated ID
    note over IdPolicy #ffaaaa: last assigned ID grows only
    IdPolicy -> Persistence: make resource ID and last assigned ID valid
end
IdPolicy --> Manager: resource ID to memoize

== Resource update ==
hnote over Agents: resource update
Agents -> IdPolicy: get resource ID
... Identical as for new resource ...

== Resource removal ==
hnote over Agents: resource removal
Agents -> IdPolicy: purge resource ID
rnote over IdPolicy #ffaaaa: fail if resource not allocated
IdPolicy -> Persistence: invalidate last assigned ID for all subresources
IdPolicy -> Persistence: invalidate resource ID
IdPolicy --> Manager: resource removed

@enduml

## Retention policy ##

Retention policy is a periodic task to keep the database compact.

@startuml
database Manager
actor Agents
actor Task
control IdPolicy
database Persistence


== Task initialization ==
hnote over Task: start()
note right
    Retention policy task is initialized with other tasks as very first at the start of application.
end note
Task -> IdPolicy: invalidate all
IdPolicy -> Persistence: invalidate all, remove outdated
note right
All outdated IDs are removed immediatelly.
Invalidate all (both entities and last assigned) entries.
end note
Persistence --> Task: number of invalidated and removed entries

== Task loop ==
hnote over Task : wait for task
... configured interval ...
note right
    Invalid entries are removed after they are older than configured time.
    Number of removed entries is logged.
end note
Task -> IdPolicy: remove outdated
IdPolicy -> Persistence: remove outdated
Persistence --> Task: number of removed entries

== Task stopped ==
hnote over Task: stop()
note right
    All existing entries are invalidated.
    If task is (re)started after long time, they will be removed immediatelly.
    If entries are not reported again, they will be removed after outdated time.
end note

Task -> IdPolicy: invalidate all
IdPolicy -> Persistence: invalidate all
Persistence --> Task: number of invalidated entries

@enduml

## Database ##

Persistence DB is organized in the form of directory with files.
File name is the resource key, it is resource type followed by parent UUID (for "local" policies) and resource UUID.
All parts are separated with '.' (dot).
File extension is always 'db'.
There is "special" resource UUID 'last', where last assigned is stored.

To indicate valid entries, sticky bit is set.
@verbatim
    -rw-rw-r-T 1 jposwiata jposwiata  1 Jun 13 12:27 System.74f30be0-3151-11e6-8b2f-fcaa1429e144.db
    -rw-rw-r-T 1 jposwiata jposwiata  1 Jun 13 12:27 System.last.db
@endverbatim

All invalid entries have sticky bit cleared.
@verbatim
    -rw-rw-r-- 1 jposwiata jposwiata  1 Jun 13 12:27 System.74f30be0-3151-11e6-8b2f-fcaa1429e144.db
    -rw-rw-r-- 1 jposwiata jposwiata  1 Jun 13 12:27 System.last.db
@endverbatim

In the file content resource ID is stored (string representation or long integer).

File modification time equals the entity state changed time. It might be very usefull to detect issues related to
UUID to ID mapping.
