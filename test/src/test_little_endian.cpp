// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <endian/little_endian.hpp>

#include <cstdint>

#include <endian/is_big_endian.hpp>

#include <gtest/gtest.h>


TEST(test_little_endian, convert)
{
    // Indicate endianness for debugging purposes
    SCOPED_TRACE(testing::Message() << "big_endian: " <<
                 endian::is_big_endian());

    // Test 8-bit integer
    {
        uint8_t data[1];
        uint8_t input = 0x11U;

        // No change will occur for a single-byte value
        endian::little_endian::put_bytes<1>(input, data);
        EXPECT_EQ(0x11U, data[0]);

        // No change will occur for a single-byte value
        uint8_t out = 0;
        endian::little_endian::get_bytes<1>(out, data);
        EXPECT_EQ(input, out);
    }

    // Test 16-bit integer
    {
        uint8_t data[2];
        uint16_t input = 0x1122U;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        endian::little_endian::put_bytes<2>(input, data);
        EXPECT_EQ(0x22U, data[0]);
        EXPECT_EQ(0x11U, data[1]);

        // Get should swap the value back (no change for big endian)
        uint16_t out = 0;
        endian::little_endian::get_bytes<2>(out, data);
        EXPECT_EQ(input, out);
    }

    // Test 24-bit integer
    {
        uint8_t data[3];
        uint32_t input = 0x112233U;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        endian::little_endian::put_bytes<3>(input, data);
        EXPECT_EQ(0x33U, data[0]);
        EXPECT_EQ(0x22U, data[1]);
        EXPECT_EQ(0x11U, data[2]);

        // Get should swap the value back (no change for big endian)
        uint32_t out = 0;
        endian::little_endian::get_bytes<3>(out, data);
        EXPECT_EQ(input, out);
    }

    // Test 32-bit integer
    {
        uint8_t data[4];
        uint32_t input = 0x11223344U;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        endian::little_endian::put_bytes<4>(input, data);
        EXPECT_EQ(0x44U, data[0]);
        EXPECT_EQ(0x33U, data[1]);
        EXPECT_EQ(0x22U, data[2]);
        EXPECT_EQ(0x11U, data[3]);

        // Get should swap the value back (no change for big endian)
        uint32_t out = 0;
        endian::little_endian::get_bytes<4>(out, data);
        EXPECT_EQ(input, out);
    }

    // Test 40-bit integer
    {
        uint8_t data[5];
        uint64_t input = 0x1122334455ULL;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        endian::little_endian::put_bytes<5>(input, data);
        EXPECT_EQ(0x55U, data[0]);
        EXPECT_EQ(0x44U, data[1]);
        EXPECT_EQ(0x33U, data[2]);
        EXPECT_EQ(0x22U, data[3]);
        EXPECT_EQ(0x11U, data[4]);

        // Get should swap the value back (no change for big endian)
        uint64_t out = 0;
        endian::little_endian::get_bytes<5>(out, data);
        EXPECT_EQ(input, out);
    }

    // Test 48-bit integer
    {
        uint8_t data[6];
        uint64_t input = 0x112233445566ULL;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        endian::little_endian::put_bytes<6>(input, data);
        EXPECT_EQ(0x66U, data[0]);
        EXPECT_EQ(0x55U, data[1]);
        EXPECT_EQ(0x44U, data[2]);
        EXPECT_EQ(0x33U, data[3]);
        EXPECT_EQ(0x22U, data[4]);
        EXPECT_EQ(0x11U, data[5]);

        // Get should swap the value back (no change for big endian)
        uint64_t out = 0;
        endian::little_endian::get_bytes<6>(out, data);
        EXPECT_EQ(input, out);
    }

    // Test 56-bit integer
    {
        uint8_t data[7];
        uint64_t input = 0x11223344556677ULL;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        endian::little_endian::put_bytes<7>(input, data);
        EXPECT_EQ(0x77U, data[0]);
        EXPECT_EQ(0x66U, data[1]);
        EXPECT_EQ(0x55U, data[2]);
        EXPECT_EQ(0x44U, data[3]);
        EXPECT_EQ(0x33U, data[4]);
        EXPECT_EQ(0x22U, data[5]);
        EXPECT_EQ(0x11U, data[6]);

        // Get should swap the value back (no change for big endian)
        uint64_t out = 0;
        endian::little_endian::get_bytes<7>(out, data);
        EXPECT_EQ(input, out);
    }

    // Test 64-bit integer
    {
        uint8_t data[8];
        uint64_t input = 0x1122334455667788ULL;

        // If the host is little endian, the put function
        // should change the byte order (no change for big endian)
        endian::little_endian::put_bytes<8>(input, data);
        EXPECT_EQ(0x88U, data[0]);
        EXPECT_EQ(0x77U, data[1]);
        EXPECT_EQ(0x66U, data[2]);
        EXPECT_EQ(0x55U, data[3]);
        EXPECT_EQ(0x44U, data[4]);
        EXPECT_EQ(0x33U, data[5]);
        EXPECT_EQ(0x22U, data[6]);
        EXPECT_EQ(0x11U, data[7]);

        // Get should swap the value back (no change for big endian)
        uint64_t out = 0;
        endian::little_endian::get_bytes<8>(out, data);
        EXPECT_EQ(input, out);
    }
}
