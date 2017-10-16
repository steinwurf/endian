News for endian
===============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* Major: Removed put* and get* static functions.
* Major: Changed API to use endian::u* structs which defines the type to read.
  This extends the capabilities of the library to also support reads of values
  which consists of 24, 40, 48, and 56 bits.

5.0.0
-----
* Minor: Added functions for reading and writing signed integers.
* Major: Changed size to be of type uint64_t instead of uint32_t.

4.2.0
-----
* Minor: Added ``skip`` function to stream.

4.1.0
-----
* Minor: Added ``stream_writer::constructor(std::vector<uint8_t>&)`` and
  ``stream_reader::constructor(std::vector<uint8_t>&)``.

4.0.0
-----
* Major: Upgrade to waf-tools 4
* Minor: Upgrade to gtest 4

3.0.0
-----
* Major: Renamed ``remaining`` to ``remaining_size``.
* Minor: Added ``data`` and ``remaining_data`` to ``stream_reader``
  and ``stream_writer``.

2.1.0
-----
* Minor: Added ``remaining`` function to ``stream``.

2.0.0
-----
* Major: Removed ``storage`` dependency.
* Major: Renamed use flag ``endian`` to ``endian_includes``.
* Major: Renamed redundant names
  ``endian_stream`` to ``stream``,
  ``endian_stream_writer`` to ``stream_writer``, and
  ``endian_stream_reader`` to ``stream_reader``.

1.0.0
-----
* Major: Initial release.
