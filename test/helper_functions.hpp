// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst

#include <endian/big_endian.hpp>
#include <endian/little_endian.hpp>
#include <endian/endian_stream.hpp>

#include <gtest/gtest.h>

/// Test Helper functions
template<class ValueType, class EndianType>
void write_read_test()
{
    const uint32_t elements = 1024;               ///no. of elements
    const uint32_t size = 1024 * sizeof(ValueType); ///size in bytes
    std::vector<uint8_t> buffer;
    buffer.resize(size);
    endian::endian_stream<EndianType> stream(buffer.data(), size);

    ValueType lowest_value = 0;
    ValueType highest_value = std::numeric_limits<ValueType>::max();
    ValueType last_value = 0;

    for (uint32_t i = 0; i < elements; i++)
    {
        stream.write(highest_value);
    }

    // Go back to the beginning of the stream
    stream.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        stream.read(last_value);
        EXPECT_EQ(highest_value, last_value);
    }

    stream.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        stream.write(lowest_value);
    }

    stream.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        stream.read(last_value);
        EXPECT_EQ(lowest_value, last_value);
    }
}

template<class ValueType, class EndianType>
void random_write_read_test(bool pseudorandom)
{
    const uint32_t elements = 1024;               /// no. of elements
    const uint32_t size = 1024 * sizeof(ValueType); /// size in bytes
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::endian_stream<EndianType> stream(buffer.data(), size);

    ValueType highest_value = std::numeric_limits<ValueType>::max();

    std::vector<ValueType> values;
    values.resize(elements);

    if (pseudorandom)
    {
        srand(1337);
    }
    else
    {
        srand(static_cast<uint32_t>(time(0)));
    }

    for (uint32_t i = 0; i < elements; i++)
    {
        values[i] = rand() % (highest_value);
        stream.write(values[i]);
    }

    ValueType last_value = 0;
    // Go back to the beginning of the stream
    stream.seek(0);
    // Read values in FIFO order
    for (uint32_t i = 0; i < elements; i++)
    {
        stream.read(last_value);
        EXPECT_EQ(values[i], last_value);
    }
}

template<class EndianType>
void various_write_read_test(bool pseudorandom)
{
    const uint32_t elements = 1024;
    const uint32_t size = 1024 * sizeof(uint64_t);
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::endian_stream<EndianType> stream(buffer.data(), size);

    std::vector<uint64_t> values;
    values.resize(elements);

    if (pseudorandom)
    {
        srand(1337);
    }
    else
    {
        srand(static_cast<uint32_t>(time(0)));
    }

    for (uint32_t i = 0; i < elements; i++)
    {
        switch (i % 4)
        {
            case 0:
                values[i] = rand() % std::numeric_limits<uint8_t>::max();
                stream.write((uint8_t)values[i]);
                break;
            case 1:
                values[i] = rand() % std::numeric_limits<uint16_t>::max();
                stream.write((uint16_t)values[i]);
                break;
            case 2:
                values[i] = rand() % std::numeric_limits<uint32_t>::max();
                stream.write((uint32_t)values[i]);
                break;
            case 3:
                values[i] = rand() % std::numeric_limits<uint64_t>::max();
                stream.write((uint64_t)values[i]);
                break;
        }
    }

    uint8_t last_u8 = 0;
    uint16_t last_u16 = 0;
    uint32_t last_u32 = 0;
    uint64_t last_u64 = 0;
    // Go back to the beginning of the stream
    stream.seek(0);
    // Read values in FIFO order
    for (uint32_t i = 0; i < elements; i++)
    {
        switch (i % 4)
        {
            case 0:
                stream.read(last_u8);
                EXPECT_EQ(values[i], last_u8);
                break;
            case 1:
                stream.read(last_u16);
                EXPECT_EQ(values[i], last_u16);
                break;
            case 2:
                stream.read(last_u32);
                EXPECT_EQ(values[i], last_u32);
                break;
            case 3:
                stream.read(last_u64);
                EXPECT_EQ(values[i], last_u64);
                break;
        }
    }
}
