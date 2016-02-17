// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <endian/convert_endian.hpp>
#include <gtest/gtest.h>

namespace
{
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
}

TEST(ConvertLittleEndian, Convert)
{
    // Indicate endianness for debugging purposes
    SCOPED_TRACE(testing::Message() << "big_endian:" << is_big_endian());

    // Test 8-bit integer
    {
        uint8_t data[1];
        uint8_t in = 0x11U;

        // No change will occur for a single-byte value
        endian::convert_endian::put8(in, data);
        EXPECT_TRUE(0x11U == data[0]);

        // No change will occur for a single-byte value
        uint8_t out = endian::convert_endian::get8(data);
        EXPECT_TRUE(out == in);
    }

    // Test 16-bit integer
    {
        uint8_t data[2];
        uint16_t in = 0x2211U;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        endian::convert_endian::put16(in, data);
        EXPECT_TRUE(0x22U == data[0]);
        EXPECT_TRUE(0x11U == data[1]);

        // Get should swap the value back (no change for big endian)
        uint16_t out = endian::convert_endian::get16(data);
        EXPECT_TRUE(out == in);
    }

    // Test 32-bit integer
    {
        uint8_t data[4];
        uint32_t in = 0x44332211U;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        endian::convert_endian::put32(in, data);
        EXPECT_TRUE(0x44U == data[0]);
        EXPECT_TRUE(0x33U == data[1]);
        EXPECT_TRUE(0x22U == data[2]);
        EXPECT_TRUE(0x11U == data[3]);

        // Get should swap the value back (no change for big endian)
        uint32_t out = endian::convert_endian::get32(data);
        EXPECT_TRUE(out == in);
    }

    // Test 64-bit integer
    {
        uint8_t data[8];
        uint64_t in = 0x8877665544332211ULL;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        endian::convert_endian::put64(in, data);
        EXPECT_TRUE(0x88U == data[0]);
        EXPECT_TRUE(0x77U == data[1]);
        EXPECT_TRUE(0x66U == data[2]);
        EXPECT_TRUE(0x55U == data[3]);
        EXPECT_TRUE(0x44U == data[4]);
        EXPECT_TRUE(0x33U == data[5]);
        EXPECT_TRUE(0x22U == data[6]);
        EXPECT_TRUE(0x11U == data[7]);

        // Get should swap the value back (no change for big endian)
        uint64_t out = endian::convert_endian::get64(data);
        EXPECT_TRUE(out == in);
    }
}
