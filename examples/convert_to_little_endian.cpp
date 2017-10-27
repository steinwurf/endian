// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>

#include <endian/is_big_endian.hpp>
#include <endian/little_endian.hpp>

int main()
{
    if (endian::is_big_endian())
    {
        std::cout << "This machine is big endian." << std::endl;
    }
    else
    {
        std::cout << "This machine is little endian." << std::endl;
    }
    uint8_t data[4];
    uint32_t input = 0x11223344U;

    std::cout << "input is: " << std::hex << input << std::endl;

    // If the host is big endian, the put function
    // should change the byte order (no change for little endian)
    endian::little_endian::put(input, data);

    // This operation could also have been performed with:
    // endian::little_endian::put_bytes<4>(input, data);

    if (0x11u == data[3] && 0x22u == data[2] && 0x33u == data[1] &&
        0x44u == data[0])
    {
        std::cout << "Success input converted" << std::endl;
    }
    else
    {
        std::cout << "Something went wrong" << std::endl;
    }

    return 0;
}
