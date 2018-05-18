// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <endian/stream_reader.hpp>
#include <endian/stream_writer.hpp>
#include <endian/big_endian.hpp>
#include <endian/little_endian.hpp>

#include <random>

#include <gtest/gtest.h>

template<class EndianType, class ValueType, uint8_t Bytes>
void write_and_read_value_type(ValueType min, ValueType max)
{

    const uint32_t elements = 1024;
    const uint64_t size = elements * sizeof(ValueType);
    std::vector<uint8_t> buffer(size);

    endian::stream_reader<EndianType> stream_reader(buffer.data(), size);
    endian::stream_writer<EndianType> stream_writer(buffer.data(), size);


    for (uint32_t i = 0; i < elements; i++)
    {
        stream_writer.template write_bytes<Bytes>(max);
    }

    stream_reader.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        ValueType value = 0;
        stream_reader.template read_bytes<Bytes>(value);
        EXPECT_EQ(max, value);
    }

    stream_writer.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        stream_writer.template write_bytes<Bytes>(min);
    }

    stream_reader.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        ValueType value = 0;
        stream_reader.template read_bytes<Bytes>(value);
        EXPECT_EQ(min, value);
    }
}

template<class EndianType, class ValueType, uint8_t Bytes>
void write_and_read_random_value_type(ValueType min, ValueType max)
{
    const uint32_t elements = 1024;
    const uint64_t size = elements * sizeof(ValueType);
    std::vector<uint8_t> buffer(size);

    endian::stream_writer<EndianType> stream_writer(buffer.data(), size);
    endian::stream_reader<EndianType> stream_reader(buffer.data(), size);

    std::vector<ValueType> values(elements);

    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<uint64_t> distribution(min, max);

    for (uint32_t i = 0; i < elements; i++)
    {
        values[i] = (ValueType)distribution(engine);
        stream_writer.template write_bytes<Bytes>(values[i]);
    }
    for (uint32_t i = 0; i < elements; i++)
    {
        ValueType value;
        stream_reader.template read_bytes<Bytes>(value);
        EXPECT_EQ(values[i], value);
    }
}

template<class EndianType>
void run_write_peek_and_read_variadic_bytes()
{
    const uint32_t elements = 1024;
    const uint64_t size = elements * sizeof(uint64_t);
    std::vector<uint8_t> buffer(size);

    endian::stream_writer<EndianType> writer(buffer.data(), size);

    std::random_device device;
    std::mt19937 engine(device());

    std::vector<uint64_t> values(elements);
    for (uint32_t i = 0; i < elements; i++)
    {
        switch (i % 8)
        {
        case 0:
        {
            std::uniform_int_distribution<uint64_t> distribution(0, 0xFF);
            values[i] = (uint8_t)distribution(engine);
            writer.template write_bytes<1>((uint8_t)values[i]);
            break;
        }
        case 1:
        {
            std::uniform_int_distribution<uint64_t> distribution(0, 0xFFFF);
            values[i] = (uint16_t)distribution(engine);
            writer.template write_bytes<2>((uint16_t)values[i]);
            break;
        }
        case 2:
        {
            std::uniform_int_distribution<uint64_t> distribution(0, 0xFFFFFF);
            values[i] = (uint32_t)distribution(engine);
            writer.template write_bytes<3>((uint32_t)values[i]);
            break;
        }
        case 3:
        {
            std::uniform_int_distribution<uint64_t> distribution(0, 0xFFFFFFFF);
            values[i] = (uint32_t)distribution(engine);
            writer.template write_bytes<4>((uint32_t)values[i]);
            break;
        }
        case 4:
        {
            std::uniform_int_distribution<uint64_t> distribution(
                0, 0x000000FFFFFFFFFF);
            values[i] = (uint64_t)distribution(engine);
            writer.template write_bytes<5>((uint64_t)values[i]);
            break;
        }
        case 5:
        {
            std::uniform_int_distribution<uint64_t> distribution(
                0, 0x0000FFFFFFFFFFFF);
            values[i] = (uint64_t)distribution(engine);
            writer.template write_bytes<6>((uint64_t)values[i]);
            break;
        }
        case 6:
        {
            std::uniform_int_distribution<uint64_t> distribution(
                0, 0x00FFFFFFFFFFFFFF);
            values[i] = (uint64_t)distribution(engine);
            writer.template write_bytes<7>((uint64_t)values[i]);
            break;
        }
        case 7:
        {
            std::uniform_int_distribution<uint64_t> distribution(
                0, 0xFFFFFFFFFFFFFFFF);
            values[i] = (uint64_t)distribution(engine);
            writer.template write_bytes<8>((uint64_t)values[i]);
            break;
        }
        }
    }

    uint8_t last_uint8_t = 0;
    uint16_t last_uint16_t = 0;
    uint32_t last_uint24_t = 0;
    uint32_t last_uint32_t = 0;
    uint64_t last_uint40_t = 0;
    uint64_t last_uint48_t = 0;
    uint64_t last_uint56_t = 0;
    uint64_t last_uint64_t = 0;

    // create reader
    endian::stream_reader<EndianType> reader(buffer.data(), size);

    // Read values in FIFO order
    for (uint32_t i = 0; i < elements; i++)
    {
        switch (i % 8)
        {
        case 0:
            // test peek
            reader.template peek_bytes<1>(last_uint8_t);
            EXPECT_EQ(values[i], last_uint8_t);

            // test read
            last_uint8_t = 0;
            reader.template read_bytes<1>(last_uint8_t);
            EXPECT_EQ(values[i], last_uint8_t);
            break;
        case 1:
            // test peek
            reader.template peek_bytes<2>(last_uint16_t);
            EXPECT_EQ(values[i], last_uint16_t);

            // test read
            last_uint16_t = 0;
            reader.template read_bytes<2>(last_uint16_t);
            EXPECT_EQ(values[i], last_uint16_t);
            break;
        case 2:
            // test peek
            reader.template peek_bytes<3>(last_uint24_t);
            EXPECT_EQ(values[i], last_uint24_t);

            // test read
            last_uint24_t = 0;
            reader.template read_bytes<3>(last_uint24_t);
            EXPECT_EQ(values[i], last_uint24_t);
            break;
        case 3:
            // test peek
            reader.template peek_bytes<4>(last_uint32_t);
            EXPECT_EQ(values[i], last_uint32_t);

            // test read
            last_uint32_t = 0;
            reader.template read_bytes<4>(last_uint32_t);
            EXPECT_EQ(values[i], last_uint32_t);
            break;
        case 4:
            // test peek
            reader.template peek_bytes<5>(last_uint40_t);
            EXPECT_EQ(values[i], last_uint40_t);

            // test read
            last_uint40_t = 0;
            reader.template read_bytes<5>(last_uint40_t);
            EXPECT_EQ(values[i], last_uint40_t);
            break;
        case 5:
            // test peek
            reader.template peek_bytes<6>(last_uint48_t);
            EXPECT_EQ(values[i], last_uint48_t);

            // test read
            last_uint48_t = 0;
            reader.template read_bytes<6>(last_uint48_t);
            EXPECT_EQ(values[i], last_uint48_t);
            break;
        case 6:
            // test peek
            reader.template peek_bytes<7>(last_uint56_t);
            EXPECT_EQ(values[i], last_uint56_t);

            // test read
            last_uint56_t = 0;
            reader.template read_bytes<7>(last_uint56_t);
            EXPECT_EQ(values[i], last_uint56_t);
            break;
        case 7:
            // test peek
            reader.template peek_bytes<8>(last_uint64_t);
            EXPECT_EQ(values[i], last_uint64_t);

            // test read
            last_uint64_t = 0;
            reader.template read_bytes<8>(last_uint64_t);
            EXPECT_EQ(values[i], last_uint64_t);
            break;
        }
    }
}

template<class EndianType>
static void run_write_and_read_string_test()
{
    const uint64_t size = 1024;
    std::vector<uint8_t> buffer(size);

    endian::stream_writer<EndianType> writer(buffer.data(), size);

    std::string first("first first first");
    std::string second("second second");
    std::string third("third");

    // Write the strings together with their lengths
    // The uint16_t-bit integers
    writer.template write_bytes<2>((uint16_t)first.size());
    writer.write((uint8_t*)first.c_str(), first.size());
    writer.template write_bytes<2>((uint16_t)second.size());
    writer.write((uint8_t*)second.c_str(), second.size());
    writer.template write_bytes<2>((uint16_t)third.size());
    writer.write((uint8_t*)third.c_str(), third.size());

    // Create reader
    endian::stream_reader<EndianType> reader(buffer.data(), size);

    // Read the strings together with their lengths
    {
        std::string string;
        uint16_t length;
        reader.template read_bytes<2>(length);
        EXPECT_EQ(first.size(), length);
        string.resize(length);
        reader.read((uint8_t*)string.c_str(), length);
        EXPECT_EQ(first, string);
    }
    {
        std::string string;
        uint16_t length;
        reader.template read_bytes<2>(length);
        EXPECT_EQ(second.size(), length);
        string.resize(length);
        reader.read((uint8_t*)string.c_str(), length);
        EXPECT_EQ(second, string);
    }
    {
        std::string string;
        uint16_t length;
        reader.template read_bytes<2>(length);
        EXPECT_EQ(third.size(), length);
        string.resize(length);
        reader.read((uint8_t*)string.c_str(), length);
        EXPECT_EQ(third, string);
    }
}

template<class EndianType>
static void run_write_read_vector_test()
{
    const uint64_t size = 1024;
    std::vector<uint8_t> buffer(size);
    endian::stream_writer<EndianType> writer(buffer.data(), size);

    using first_type = uint8_t;
    std::vector<first_type> first(100, 'a');
    using second_type = uint32_t;
    std::vector<second_type> second(200, 1234);

    // Write the vectors together with their lengths
    // The uint16_t-bit integers
    writer.template write_bytes<2>((uint16_t)first.size());
    writer.write((uint8_t*)first.data(), first.size() * sizeof(first_type));

    // The size here refers to the number of integers
    // stored
    writer.template write_bytes<2>((uint16_t)second.size());
    writer.write((uint8_t*)second.data(), second.size() * sizeof(second_type));

    // Temp variables
    std::vector<first_type> first_out;
    std::vector<second_type> second_out;

    // Create reader
    endian::stream_reader<EndianType> reader(buffer.data(), size);

    {
        uint16_t length;
        reader.template read_bytes<2>(length);
        EXPECT_EQ(first.size(), length);

        std::vector<first_type> vector;
        vector.resize(length);
        reader.read((uint8_t*)vector.data(), length * sizeof(first_type));
        EXPECT_EQ(first, vector);
    }
    {
        uint16_t length;
        reader.template read_bytes<2>(length);
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
        SCOPED_TRACE("u8");
        using value_type = uint8_t;
        value_type min = std::numeric_limits<value_type>::max();
        value_type max = std::numeric_limits<value_type>::min();
        write_and_read_value_type<EndianType, value_type, 1>(min, max);
        write_and_read_random_value_type<EndianType, value_type, 1>(min, max);
    }
    {
        SCOPED_TRACE("i8");
        using value_type = int8_t;
        value_type min = std::numeric_limits<value_type>::max();
        value_type max = std::numeric_limits<value_type>::min();
        write_and_read_value_type<EndianType, value_type, 1>(min, max);
        write_and_read_random_value_type<EndianType, value_type, 1>(min, max);
    }
    {
        SCOPED_TRACE("u16");
        using value_type = uint16_t;
        value_type min = std::numeric_limits<value_type>::max();
        value_type max = std::numeric_limits<value_type>::min();
        write_and_read_value_type<EndianType, value_type, 2>(min, max);
        write_and_read_random_value_type<EndianType, value_type, 2>(min, max);
    }
    {
        SCOPED_TRACE("i16");
        using value_type = int16_t;
        value_type min = std::numeric_limits<value_type>::max();
        value_type max = std::numeric_limits<value_type>::min();
        write_and_read_value_type<EndianType, value_type, 2>(min, max);
        write_and_read_random_value_type<EndianType, value_type, 2>(min, max);
    }
    {
        SCOPED_TRACE("u24");
        using value_type = uint32_t;
        value_type min = 0x00000000;
        value_type max = 0x00FFFFFF;
        write_and_read_value_type<EndianType, value_type, 3>(min, max);
        write_and_read_random_value_type<EndianType, value_type, 3>(min, max);
    }
    {
        SCOPED_TRACE("u32");
        using value_type = uint32_t;
        value_type min = std::numeric_limits<value_type>::max();
        value_type max = std::numeric_limits<value_type>::min();
        write_and_read_value_type<EndianType, value_type, 4>(min, max);
        write_and_read_random_value_type<EndianType, value_type, 4>(min, max);
    }
    {
        SCOPED_TRACE("i32");
        using value_type = int32_t;
        value_type min = std::numeric_limits<value_type>::max();
        value_type max = std::numeric_limits<value_type>::min();
        write_and_read_value_type<EndianType, value_type, 4>(min, max);
        write_and_read_random_value_type<EndianType, value_type, 4>(min, max);
    }
    {
        SCOPED_TRACE("u40");
        using value_type = uint64_t;
        value_type min = 0x0000000000000000;
        value_type max = 0x000000FFFFFFFFFF;
        write_and_read_value_type<EndianType, value_type, 5>(min, max);
        write_and_read_random_value_type<EndianType, value_type, 5>(min, max);
    }
    {
        SCOPED_TRACE("u48");
        using value_type = uint64_t;
        value_type min = 0x0000000000000000;
        value_type max = 0x0000FFFFFFFFFFFF;
        write_and_read_value_type<EndianType, value_type, 6>(min, max);
        write_and_read_random_value_type<EndianType, value_type, 6>(min, max);
    }
    {
        SCOPED_TRACE("u56");
        using value_type = uint64_t;
        value_type min = 0x0000000000000000;
        value_type max = 0x00FFFFFFFFFFFFFF;
        write_and_read_value_type<EndianType, value_type, 7>(min, max);
        write_and_read_random_value_type<EndianType, value_type, 7>(min, max);
    }
    {
        SCOPED_TRACE("u64");
        using value_type = uint64_t;
        value_type min = std::numeric_limits<value_type>::max();
        value_type max = std::numeric_limits<value_type>::min();
        write_and_read_value_type<EndianType, value_type, 8>(min, max);
        write_and_read_random_value_type<EndianType, value_type, 8>(min, max);
    }
    {
        SCOPED_TRACE("i64");
        using value_type = int64_t;
        value_type min = std::numeric_limits<value_type>::max();
        value_type max = std::numeric_limits<value_type>::min();
        write_and_read_value_type<EndianType, value_type, 8>(min, max);
        write_and_read_random_value_type<EndianType, value_type, 8>(min, max);
    }

    run_write_peek_and_read_variadic_bytes<EndianType>();
    run_write_and_read_string_test<EndianType>();
    run_write_read_vector_test<EndianType>();
}

TEST(test_stream_writer_reader, test_reader_and_writer)
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
