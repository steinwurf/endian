News for endian
===============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* tbd

10.0.0
------
* Major: stream class in detail namespace and folder
* Major: Removed stream_writer(std::vector<uint8_t>&) and
  stream_reader(const std::vector<uint8_t>&)
* Minor: Added ``size_type`` to the API.

9.0.0
-----
* Major: Writing to/from half or less of the bytes of a provided type is now
  disallowed, use a smaller type instead and make an explicit cast is needed.
* Patch: Recursive implementation of conversion.

8.2.0
-----
* Minor: Added ``ValueType get()`` and ``ValueType get_bytes()``.
* Minor: Added support for Float type and Double types.

8.1.0
-----
* Minor: Added ``ValueType read()`` and ``ValueType peek()``.

8.0.1
-----
* Patch: Fixed ``stream_reader.read``.

8.0.0
-----
* Major: Removed ``types.hpp``.
* Major: Renamed ``stream_reader.read`` to ``stream_reader.read_bytes`` which
  now takes a ``uint8_t`` template argument specifying the number of bytes.
* Major: Renamed ``stream_reader.peek`` to ``stream_reader.peek_bytes`` which
  now takes a ``uint8_t`` template argument specifying the number of bytes.
* Major: Renamed ``stream_writer.write`` to ``stream_writer.write_bytes`` which
  now takes a ``uint8_t`` template argument specifying the number of bytes.
* Major: Added ``stream_writer.write`` which allows writing ``uint8_t``,
  ``uint16_t``, ``uint32_t``, and ``uint64_t``.
* Major: Added ``stream_reader.read`` which allows reading ``uint8_t``,
  ``uint16_t``, ``uint32_t``, and ``uint64_t``.
* Major: Added ``stream_reader.peek`` which allows peeking ``uint8_t``,
  ``uint16_t``, ``uint32_t``, and ``uint64_t``.

7.0.0
-----
* Major: Renamed ``get`` and ``put`` to ``get_bytes`` and ``put_bytes``.
* Minor: Added possibility to read native types with ``get`` and ``put``.

6.2.0
-----
* Minor: Made ``peek`` const.
* Minor: ``peek`` now takes an offset parameter for peeking further
  ahead in the stream. This value is defaulted to 0 so the API hasn't changed.

6.1.0
-----
* Minor: Added `peek` which allows reading without moving the read position.

6.0.1
-----
* Patch: Allow streams of size 0.

6.0.0
-----
* Major: Removed put* and get* static functions.
* Major: Added support for additional byte sized fields. Instead of
  directly using the native types. endian now uses the types defined in
  ``src/endian/types.hpp`` such as ``u8``, ``u16``, ``u24``, ``u32``, etc.

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
