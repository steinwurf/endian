// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>

#include <storage/storage.hpp>

#include "../helper_function.syhpp"
#include <endian/little_endian.hpp>
#include <endian/endian_stream.hpp>

#include <gtest/gtest.h>

TEST(TestLittleEndianStream, create_stream)
{
    const uint32_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::endian_stream<endian::little_endian> stream(buffer.data(), size);

    EXPECT_EQ(size, stream.size());
    EXPECT_EQ(0U, stream.position());
}

TEST(TestLittleEndianStream, create_stream_from_storage)
{
    const uint32_t elements = 10;               ///no. of elements
    const uint32_t size = elements * sizeof(uint32_t);

    std::vector<uint8_t> buffer;
    buffer.resize(size);
    // Create endian stream directly from storage::storage
    endian::endian_stream<endian::little_endian>
        stream(storage::storage(buffer));

    EXPECT_EQ(size, stream.size());
    EXPECT_EQ(0U, stream.position());

    for (uint32_t i = 0; i < elements; i++)
    {
        stream.write(i);
    }

    EXPECT_EQ(size, stream.size());
    EXPECT_EQ(size, stream.position());

    // Go back to the beginning of the stream
    stream.seek(0);
    uint32_t last_value = 0;
    for (uint32_t i = 0; i < elements; i++)
    {
        stream.read(last_value);
        EXPECT_EQ(i, last_value);
    }
}


/// Write-read tests

TEST(TestLittleEndianStream, read_write_u8)
{
    write_read_test<uint8_t, endian::little_endian>();
}

TEST(TestLittleEndianStream, read_write_u16)
{
    write_read_test<uint16_t, endian::little_endian>();
}

TEST(TestLittleEndianStream, read_write_u32)
{
    write_read_test<uint32_t, endian::little_endian>();
}

TEST(TestLittleEndianStream, read_write_u64)
{
    write_read_test<uint64_t, endian::little_endian>();
}

/// Pseudorandom write-read tests

TEST(TestLittleEndianStream, pseudorandom_read_write_u8)
{
    random_write_read_test<uint8_t, endian::little_endian>(true);
}

TEST(TestLittleEndianStream, pseudorandom_read_write_u16)
{
    random_write_read_test<uint16_t, endian::little_endian>(true);
}

TEST(TestLittleEndianStream, pseudorandom_read_write_u32)
{
    random_write_read_test<uint32_t, endian::little_endian>(true);
}

TEST(TestLittleEndianStream, pseudorandom_read_write_u64)
{
    random_write_read_test<uint64_t, endian::little_endian>(true);
}

/// Random write read tests

TEST(TestLittleEndianStream, random_read_write_u8)
{
    random_write_read_test<uint8_t, endian::little_endian>(false);
}

TEST(TestLittleEndianStream, random_read_write_u16)
{
    random_write_read_test<uint16_t, endian::little_endian>(false);
}

TEST(TestLittleEndianStream, random_read_write_u32)
{
    random_write_read_test<uint32_t, endian::little_endian>(false);
}

TEST(TestLittleEndianStream, random_read_write_u64)
{
    random_write_read_test<uint64_t, endian::little_endian>(false);
}

/// Various read writes

TEST(TestLittleEndianStream, pseudorandom_various_read_write)
{
    various_write_read_test<endian::little_endian>(true);
}

TEST(TestLittleEndianStream, random_various_read_write)
{
    various_write_read_test<endian::little_endian>(false);
}

/// Test composite data types

TEST(TestLittleEndianStream, read_write_string)
{
    const uint32_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::endian_stream<endian::little_endian> stream(buffer.data(), size);

    std::string first("first first first");
    std::string second("second second");
    std::string third("third");

    // Write the strings together with their lengths
    // The length is written as 16-bit integers
    stream.write((uint16_t)first.size());
    stream.write(storage::storage(first));
    stream.write((uint16_t)second.size());
    stream.write(storage::storage(second));
    stream.write((uint16_t)third.size());
    stream.write(storage::storage(third));

    // Temp variables
    std::string current;
    uint16_t len = 0;

    // Go back to the beginning of the stream
    stream.seek(0);

    // Read the strings together with their lengths
    stream.read(len);
    EXPECT_EQ(first.size(), len);
    // Resize the current string to accommodate 'len' bytes
    current.resize(len);
    stream.read(storage::storage(current));
    EXPECT_EQ(first, current);

    stream.read(len);
    EXPECT_EQ(second.size(), len);
    current.resize(len);
    stream.read(storage::storage(current));
    EXPECT_EQ(second, current);

    stream.read(len);
    EXPECT_EQ(third.size(), len);
    current.resize(len);
    stream.read(storage::storage(current));
    EXPECT_EQ(third, current);
}

TEST(TestLittleEndianStream, read_write_vector)
{
    const uint32_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);
    endian::endian_stream<endian::little_endian> stream(buffer.data(), size);

    std::vector<uint8_t> first(100, 'a');
    std::vector<uint32_t> second(200, 1234);

    // Write the vectors together with their lengths
    // The length is written as 16-bit integers
    stream.write((uint16_t)first.size());
    stream.write(storage::storage(first));
    // The size here refers to the number of integers
    // stored in the second vector
    stream.write((uint16_t)second.size());
    stream.write(storage::storage(second));

    // Temp variables
    std::vector<uint8_t> first_out;
    std::vector<uint32_t> second_out;
    uint16_t len = 0;

    // Go back to the beginning of the stream
    stream.seek(0);

    // Read the vector length
    stream.read(len);
    EXPECT_EQ(first.size(), len);
    // Resize the output vector to accommodate 'len' bytes
    first_out.resize(len);
    stream.read(storage::storage(first_out));
    EXPECT_TRUE(
        std::equal(first.begin(), first.end(), first_out.begin()));

    // Read the vector length
    stream.read(len);
    EXPECT_EQ(second.size(), len);
    // Resize the output vector to accommodate 'len' bytes
    second_out.resize(len);
    stream.read(storage::storage(second_out));
    EXPECT_TRUE(
        std::equal(second.begin(), second.end(), second_out.begin()));
}
