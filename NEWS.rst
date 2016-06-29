News for endian
===============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* Major: Renamed ``remaining`` to ``remaining_size``.
* Minor: Added ``data``, and ``remaining_data`` functions to ``stream_reader``
  and ``stream_reader`` and writer.

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
