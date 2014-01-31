
Data Core
---------

The data core provides facilities to support the definition of protocol
messages, and their reading and writing to buffers for transmission.

Buffer Management
=================

A Buffer is simply a block of uninterpreted memory. While reading and
writing directly to buffers is possible, it is not the preferred method.
Instead, we use a View of a Buffer to ensure that reads and writes do not 
exceed the Buffer's bounds.

Data Abstractions
=================

There are a number of data abstractions common to all protocol 
implementations.

* Sequence -- A sequence of elements
* String -- A fixed-size sequence of characters
* ...

Over time, we expect to add abstractions based on the way that protocols
read or interpret different kinds of data.

