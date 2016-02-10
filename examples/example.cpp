// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <endian/some.hpp>

#include <iostream>

int main()
{
    endian::some s;

    if (s.some_method())
    {
        std::cout << "Hello endian!" << std::endl;
    }

    return 0;
}
