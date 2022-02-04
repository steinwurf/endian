Network
=======

Many IETF RFCs use the term network order, meaning the order of transmission for bits and bytes over the wire in network
protocols. Among others, the historic RFC 1700 (also known as Internet standard STD 2) has defined the network order for
protocols in the Internet protocol suite to be big-endian, hence the use of the term "network byte order" for big-endian
byte order.

| `<https://en.wikipedia.org/wiki/Endianness#Networking>`_
| `<https://datatracker.ietf.org/doc/html/rfc1700#page-3>`_

For this reason, there is a network alias that can be used instead of having to remember what the standard is.

See the example below for how to use it, but basically, you just need to replace either ``big_endian`` or
``little_endian`` with ``network``.

.. literalinclude:: ../../examples/network.cpp
    :language: c++
    :linenos:
