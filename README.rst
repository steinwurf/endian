======
endian
======

endian is a simple C++ library for conversion between big and little endian
data representations and provide stream-like interface for accessing a
fixed-size buffer.

.. image:: http://buildbot.steinwurf.dk/svgstatus?project=endian
    :target: http://buildbot.steinwurf.dk/stats?projects=endian

.. contents:: Table of Contents:
   :local:

Usage
-----

The ``endian::little_endian`` and ``endian::big_endian`` structures acts
like function containers for conversion to little/big endian. In both
structures there are two types of functions `put` and `get`, each have
specializations for different integer types.

The ``endian::endian_stream`` class is stream-like interface used for
writing and reading data either to or from a stream or storage object.

Examples for using endian can be found in ``test/src`` and in ``example/src``

Header-only
...........

The library itself is header-only so essentially to use it you just have to
clone the repository and setup the right include paths in the project where
you would like to use it.

Unit testing
------------

The unit tests for the endian library are located in the ``test/src``
folder.

We use the Google Unit Testing Framwork (gtest) to drive the unit tests. To
build and run the test run:

::

    python waf configure
    python waf

Depending on the platform you should see a test binary called
``endian_test`` in (extension also depends on operating system e.g `.exe`
for Windows):

::

   build/platform/test

Where ``platform`` is typically either linux, win32 or darwin depending on
your operating system

License
-------

endian is available under the BSD license.
