// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <endian/stream.hpp>

#include <cstdint>
#include <vector>

#include <endian/stream_reader.hpp>
#include <endian/stream_writer.hpp>
#include <endian/big_endian.hpp>
#include <endian/little_endian.hpp>

#include <gtest/gtest.h>


TEST(test_stream, basic_api)
{
    {
        SCOPED_TRACE(testing::Message() << "size 1");
        uint32_t size = 1U;
        endian::stream stream(size);

        // check initial state
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(0U, stream.position());
        EXPECT_EQ(size, stream.remaining());

        // check state after seek
        stream.seek(1);
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(1U, stream.position());
        EXPECT_EQ(0U, stream.remaining());
    }

    {
        SCOPED_TRACE(testing::Message() << "size 10000");
        uint32_t size = 10000U;
        endian::stream stream(size);

        // check initial state
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(0U, stream.position());
        EXPECT_EQ(size, stream.remaining());

        // check state after seek
        stream.seek(size / 2);
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(size / 2, stream.position());
        EXPECT_EQ(size / 2, stream.remaining());

        // that consecutive seeks doesn't alter state.
        stream.seek(size / 2);
        stream.seek(size / 2);
        stream.seek(size / 2);
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(size / 2, stream.position());
        EXPECT_EQ(size / 2, stream.remaining());

        // seek to end
        stream.seek(size);
        EXPECT_EQ(size, stream.position());
        EXPECT_EQ(0U, stream.remaining());
    }
}

// ********************** //
//  Reader writer tests:  //
// ********************** //

template<class ValueType, class EndianType>
void write_and_read_value_type_test()
{
    const uint32_t elements = 1024;
    const uint32_t size = 1024 * sizeof(ValueType);
    std::vector<uint8_t> buffer;
    buffer.resize(size);
    endian::stream_reader<EndianType> stream_reader(buffer.data(), size);
    endian::stream_writer<EndianType> stream_writer(buffer.data(), size);

    ValueType lowest_value = std::numeric_limits<ValueType>::min();
    ValueType highest_value = std::numeric_limits<ValueType>::max();

    for (uint32_t i = 0; i < elements; i++)
    {
        stream_writer.write(highest_value);
    }

    stream_reader.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        ValueType value = 0;
        stream_reader.read(value);
        EXPECT_EQ(highest_value, value);
    }

    stream_writer.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        stream_writer.write(lowest_value);
    }

    stream_reader.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        ValueType value = 0;
        stream_reader.read(value);
        EXPECT_EQ(lowest_value, value);
    }
}

template<class ValueType, class EndianType>
void write_and_read_random_value_type_test()
{
    const uint32_t elements = 1024;
    const uint32_t size = 1024 * sizeof(ValueType);
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::stream_writer<EndianType> stream_writer(buffer.data(), size);
    endian::stream_reader<EndianType> stream_reader(buffer.data(), size);

    ValueType highest_value = std::numeric_limits<ValueType>::max();

    std::vector<ValueType> values;
    values.resize(elements);

    uint32_t seed = static_cast<uint32_t>(time(0));
    SCOPED_TRACE(testing::Message() << "seed: " << seed);
    srand(seed);

    for (uint32_t i = 0; i < elements; i++)
    {
        values[i] = rand() % (highest_value);
        stream_writer.write(values[i]);
    }
    for (uint32_t i = 0; i < elements; i++)
    {
        ValueType value;
        stream_reader.read(value);
        EXPECT_EQ(values[i], value);
    }
}

template<class EndianType>
void write_and_read_variadic_types_test()
{
    const uint32_t elements = 1024;
    const uint32_t size = 1024 * sizeof(uint64_t);
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::stream_writer<EndianType> writer(buffer.data(), size);

    std::vector<uint64_t> values;
    values.resize(elements);

    uint32_t seed = static_cast<uint32_t>(time(0));
    SCOPED_TRACE(testing::Message() << "seed: " << seed);
    srand(seed);

    for (uint32_t i = 0; i < elements; i++)
    {
        switch (i % 4)
        {
            case 0:
                values[i] = rand() % std::numeric_limits<uint8_t>::max();
                writer.write((uint8_t)values[i]);
                break;
            case 1:
                values[i] = rand() % std::numeric_limits<uint16_t>::max();
                writer.write((uint16_t)values[i]);
                break;
            case 2:
                values[i] = rand() % std::numeric_limits<uint32_t>::max();
                writer.write((uint32_t)values[i]);
                break;
            case 3:
                values[i] = rand() % std::numeric_limits<uint64_t>::max();
                writer.write((uint64_t)values[i]);
                break;
        }
    }

    uint8_t last_u8 = 0;
    uint16_t last_u16 = 0;
    uint32_t last_u32 = 0;
    uint64_t last_u64 = 0;

    // create reader
    endian::stream_reader<EndianType> reader(buffer.data(), size);

    // Read values in FIFO order
    for (uint32_t i = 0; i < elements; i++)
    {
        switch (i % 4)
        {
            case 0:
                reader.read(last_u8);
                EXPECT_EQ(values[i], last_u8);
                break;
            case 1:
                reader.read(last_u16);
                EXPECT_EQ(values[i], last_u16);
                break;
            case 2:
                reader.read(last_u32);
                EXPECT_EQ(values[i], last_u32);
                break;
            case 3:
                reader.read(last_u64);
                EXPECT_EQ(values[i], last_u64);
                break;
        }
    }
}

template<class EndianType>
static void write_and_read_string_test()
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

    // Create reader
    endian::stream_reader<EndianType> reader(buffer.data(), size);

    // Read the strings together with their lengths
    {
        std::string string;
        uint16_t length;
        reader.read(length);
        EXPECT_EQ(first.size(), length);
        string.resize(length);
        reader.read((uint8_t*)string.c_str(), length);
        EXPECT_EQ(first, string);
    }
    {
        std::string string;
        uint16_t length;
        reader.read(length);
        EXPECT_EQ(second.size(), length);
        string.resize(length);
        reader.read((uint8_t*)string.c_str(), length);
        EXPECT_EQ(second, string);
    }
    {
        std::string string;
        uint16_t length;
        reader.read(length);
        EXPECT_EQ(third.size(), length);
        string.resize(length);
        reader.read((uint8_t*)string.c_str(), length);
        EXPECT_EQ(third, string);
    }
}

template<class EndianType>
static void run_read_write_vector_test()
{
    const uint32_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);
    endian::stream_writer<EndianType> writer(buffer.data(), size);

    using first_type = uint8_t;
    std::vector<first_type> first(100, 'a');
    using second_type = uint32_t;
    std::vector<second_type> second(200, 1234);

    // Write the vectors together with their lengths
    // The length is written as 16-bit integers
    writer.write((uint16_t)first.size());
    writer.write((uint8_t*)first.data(), first.size() * sizeof(first_type));

    // The size here refers to the number of integers
    // stored in the second vector
    writer.write((uint16_t)second.size());
    writer.write((uint8_t*)second.data(), second.size() * sizeof(second_type));

    // Temp variables
    std::vector<first_type> first_out;
    std::vector<second_type> second_out;

    // Create reader
    endian::stream_reader<EndianType> reader(buffer.data(), size);

    {
        uint16_t length;
        reader.read(length);
        EXPECT_EQ(first.size(), length);

        std::vector<first_type> vector;
        vector.resize(length);
        reader.read((uint8_t*)vector.data(), length * sizeof(first_type));
        EXPECT_EQ(first, vector);
    }
    {
        uint16_t length;
        reader.read(length);
        EXPECT_EQ(second.size(), length);

        std::vector<second_type> vector;
        vector.resize(length);
        reader.read((uint8_t*)vector.data(), length * sizeof(second_type));
        EXPECT_EQ(second, vector);
    }
}

template<class EndianType>
static void test_reader_and_writer_api()
{
    {
        SCOPED_TRACE("uint8_t");
        write_and_read_value_type_test<uint8_t, EndianType>();
        write_and_read_random_value_type_test<uint8_t, EndianType>();
    }
    {
        SCOPED_TRACE("uint16_t");
        write_and_read_value_type_test<uint16_t, EndianType>();
        write_and_read_random_value_type_test<uint16_t, EndianType>();
    }
    {
        SCOPED_TRACE("uint32_t");
        write_and_read_value_type_test<uint32_t, EndianType>();
        write_and_read_random_value_type_test<uint32_t, EndianType>();
    }
    {
        SCOPED_TRACE("uint64_t");
        write_and_read_value_type_test<uint64_t, EndianType>();
        write_and_read_random_value_type_test<uint64_t, EndianType>();
    }

    write_and_read_variadic_types_test<EndianType>();
    write_and_read_string_test<EndianType>();
    run_read_write_vector_test<EndianType>();
}

TEST(test_stream, test_reader_and_writer)
{
    {
        SCOPED_TRACE("big endian");
        test_reader_and_writer_api<endian::big_endian>();
    }

    {
        SCOPED_TRACE("little endian");
        test_reader_and_writer_api<endian::little_endian>();
    }
}
