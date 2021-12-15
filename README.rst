======
endian
======

|Linux make-specs| |Windows make-specs| |MacOS make-specs| |Linux CMake| |Windows CMake| |MacOS CMake| |Valgrind| |No Assertions| |Clang Format| |Cppcheck|

.. |Linux make-specs| image:: https://github.com/steinwurf/abacus/actions/workflows/linux_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/linux_mkspecs.yml
   
.. |Windows make-specs| image:: https://github.com/steinwurf/abacus/actions/workflows/windows_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/windows_mkspecs.yml

.. |MacOS make-specs| image:: https://github.com/steinwurf/abacus/actions/workflows/macos_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/macos_mkspecs.yml
   
.. |Linux CMake| image:: https://github.com/steinwurf/abacus/actions/workflows/linux_cmake.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/linux_cmake.yml

.. |Windows CMake| image:: https://github.com/steinwurf/abacus/actions/workflows/windows_cmake.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/windows_cmake.yml
   
.. |MacOS CMake| image:: https://github.com/steinwurf/abacus/actions/workflows/macos_cmake.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/macos_cmake.yml

.. |Clang Format| image:: https://github.com/steinwurf/abacus/actions/workflows/clang-format.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/clang-format.yml

.. |No Assertions| image:: https://github.com/steinwurf/abacus/actions/workflows/nodebug.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/nodebug.yml

.. |Valgrind| image:: https://github.com/steinwurf/abacus/actions/workflows/valgrind.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/valgrind.yml

.. |Cppcheck| image:: https://github.com/steinwurf/abacus/actions/workflows/cppcheck.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/cppcheck.yml

.. image:: https://raw.githubusercontent.com/steinwurf/endian/master/endian.png
   :width: 300

endian is a simple C++ library for conversion between big and little endian
data representations and provide stream-like interface for accessing a
fixed-size buffer.

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
build and run the test run::

    python waf configure
    python waf

Depending on the platform you should see a test binary called
``endian_test`` in (extension also depends on operating system e.g `.exe`
for Windows)::

   build/platform/test

Where ``platform`` is typically either linux, win32 or darwin depending on
your operating system

Use as Dependency in CMake
--------------------------

To depend on this project when using the CMake build system, add the following
in your CMake build script::

   add_subdirectory("/path/to/endian" endian)
   target_link_libraries(<my_target> steinwurf::endian)

Where ``<my_target>`` is replaced by your target.

License
-------

endian is available under the BSD license.
