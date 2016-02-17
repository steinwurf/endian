// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>

#include <endian/convert_endian.hpp>

// Checks if the platform is big- or little-endian
// From a test proposed here:
// http://stackoverflow.com/questions/1001307/
static bool is_big_endian()
{
    union
    {
        uint32_t i;
        uint8_t c[4];
    } test = {0x01020304};

    return test.c[0] == 1;
}

int main()
{
    std::cout << "Are we little endian " <<  is_big_endian() << std::endl;
    uint8_t data[4];
    uint32_t in = 0x11223344U;

    std::cout << "in is: " << std::hex << in << std::endl;
    // If the host is little endian, the put function
    // should change the byte order (no change for big endian)
    endian::convert_endian::put32(in, data);
    uint32_t out = endian::convert_endian::get32(data);

    if(0x11u == data[0] && 0x22u == data[1] && 0x33u == data[2]
       && 0x44u == data[3])
    {
        std::cout << "sucess in conveting endian" << std::endl;
    } else
    {
        std::cout << "Something went wrong" << std::endl;
    }

    return 0;
}
