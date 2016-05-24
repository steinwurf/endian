// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.


#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>

#include "../helper_functions.hpp"
#include <endian/big_endian.hpp>
#include <endian/little_endian.hpp>

#include <endian/stream_writer.hpp>
#include <endian/stream_reader.hpp>
#include <gtest/gtest.h>

template<class EndianType>
static void run_test_create_writer()
{
    const uint32_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::stream_writer<EndianType> stream_writer(buffer.data(), size);

    EXPECT_EQ(size, stream_writer.size());
    EXPECT_EQ(0u, stream_writer.position());
}

template<class EndianType>
static void run_test_create_reader()
{
    const uint32_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::stream_reader<EndianType> stream_reader(buffer.data(), size);

    EXPECT_EQ(size, stream_reader.size());
    EXPECT_EQ(0u, stream_reader.position());
}

template<class EndianType>
static void run_test_simple_read_write()
{
    const uint32_t elements = 10; /// no. of elements
    const uint32_t size = elements * sizeof(uint32_t);

    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::stream_writer<EndianType> writer(buffer.data(), buffer.size());

    endian::stream_reader<EndianType> reader(buffer.data(), buffer.size());

    EXPECT_EQ(size, writer.size());
    EXPECT_EQ(0U, writer.position());

    EXPECT_EQ(size, reader.size());
    EXPECT_EQ(0U, reader.position());

    for (uint32_t i = 0; i < elements; i++)
    {
        writer.write(i);
    }

    EXPECT_EQ(size, writer.size());
    EXPECT_EQ(0U, writer.position());

    uint32_t last_value = 0;
    for(uint32_t i = 0; i < elements; i++)
    {
        reader.read(last_value);
        EXPECT_EQ(i, last_value);
    }
}

/// Write-read test
template<class ValueType, class EndianType>
static void run_write_read_test()
{
    write_read_test<ValueType, EndianType>();
}

/// Pseudorandom write-read tests
template<class ValueType, class EndianType>
static void run_pseudorandom_read_write_test()
{
    random_write_read_test<ValueType, EndianType>(true);
}

/// Random write read tests
template<class ValueType, class EndianType>
static void run_random_read_write_test()
{
    random_write_read_test<ValueType, EndianType>(false);
}

/// Various read writes
template<class EndianType>
static void run_pseudorandom_various_read_writes_test()
{
    various_write_read_test<EndianType>(true);
}

template<class EndianType>
static void run_random_various_read_writes_test()
{
    various_write_read_test<EndianType>(false);
}

template<class EndianType>
static void run_read_write_string_test()
{
    const uint32_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::stream_writer<EndianType> writer(buffer.data(), size);

    std::string first("first first first");
    std::string second("second second");
    std::string third("third");

    // Write the strings together with their lengths
    // The length is written as 16-bit integers
    writer.write((uint16_t)first.size());
    writer.write((uint8_t*)first.c_str(), first.size());
    writer.write((uint16_t)second.size());
    writer.write((uint8_t*)second.c_str(), second.size());
    writer.write((uint16_t)third.size());
    writer.write((uint8_t*)third.c_str(), third.size());

    // Temp variables
    std::string current;
    uint16_t len = 0;

    // Create reader
    endian::stream_reader<EndianType> reader(buffer.data(), size);

    // Read the strings together with their lengths
    reader.read(len);
    EXPECT_EQ(first.size(), len);
    // Resize the current string to accommodate 'len' bytes
    current.resize(len);
    reader.read((uint8_t*)current.c_str(), len);
    EXPECT_EQ(first, current);

    reader.read(len);
    EXPECT_EQ(second.size(), len);
    current.resize(len);
    reader.read((uint8_t*)current.c_str(), len);
    EXPECT_EQ(second, current);

    reader.read(len);
    EXPECT_EQ(third.size(), len);
    current.resize(len);
    reader.read((uint8_t*)current.c_str(), len);
    EXPECT_EQ(third, current);
}


template<class EndianType>
static void run_read_write_vector_test()
{
    const uint32_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);
    endian::stream_writer<EndianType> writer(buffer.data(), size);

    std::vector<uint8_t> first(100, 'a');
    using second_type = uint32_t;
    std::vector<second_type> second(200, 1234);

    // Write the vectors together with their lengths
    // The length is written as 16-bit integers
    writer.write((uint16_t)first.size());
    writer.write(first.data(), first.size());
    // The size here refers to the number of integers
    // stored in the second vector
    writer.write((uint16_t)second.size());
    writer.write((uint8_t*)second.data(), second.size() * sizeof(second_type));

    // Temp variables
    std::vector<uint8_t> first_out;
    std::vector<second_type> second_out;
    uint16_t len = 0;

    // Create reader
    endian::stream_reader<EndianType> reader(buffer.data(), size);

    // Read the vector length
    reader.read(len);
    EXPECT_EQ(first.size(), len);
    // Resize the output vector to accommodate 'len' bytes
    first_out.resize(len);
    reader.read(first_out.data(), len);
    EXPECT_EQ(first, first_out);

    // Read the vector length
    reader.read(len);
    EXPECT_EQ(second.size(), len);
    // Resize the output vector to accommodate 'len' bytes
    second_out.resize(len);
    reader.read((uint8_t*)second_out.data(), len * sizeof(second_type));
    EXPECT_EQ(second, second_out);
}

template<class EndianType>
static void test_basic_api()
{
    run_test_create_writer<EndianType>();

    run_test_create_reader<EndianType>();

    SCOPED_TRACE(testing::Message() << "uint = uint8_t");
    {
        run_write_read_test<uint8_t, EndianType>();
        run_pseudorandom_read_write_test<uint8_t, EndianType>();
        run_random_read_write_test<uint8_t, EndianType>();
    }

    SCOPED_TRACE(testing::Message() << "uint = uint16_t");
    {
        run_write_read_test<uint16_t, EndianType>();
        run_pseudorandom_read_write_test<uint16_t, EndianType>();
        run_random_read_write_test<uint16_t, EndianType>();
    }

    SCOPED_TRACE(testing::Message() << "uint = uint32_t");
    {
        run_write_read_test<uint32_t, EndianType>();
        run_pseudorandom_read_write_test<uint32_t, EndianType>();
        run_random_read_write_test<uint32_t, EndianType>();
    }

    SCOPED_TRACE(testing::Message() << "uint = uint64_t");
    {
        run_write_read_test<uint64_t, EndianType>();
        run_pseudorandom_read_write_test<uint64_t, EndianType>();
        run_random_read_write_test<uint64_t, EndianType>();
    }

    run_pseudorandom_various_read_writes_test<EndianType>();
    run_random_various_read_writes_test<EndianType>();

    run_read_write_string_test<EndianType>();
    run_read_write_vector_test<EndianType>();
}


TEST(test_stream, big_endian)
{
    test_basic_api<endian::big_endian>();
}

TEST(test_stream, little_endian)
{
    test_basic_api<endian::little_endian>();
}
