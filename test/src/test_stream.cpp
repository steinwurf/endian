// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <endian/stream.hpp>

#include <cstdint>
#include <random>
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
        uint64_t size = 1U;
        endian::stream stream(size);

        // check initial state
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(0U, stream.position());
        EXPECT_EQ(size, stream.remaining_size());

        // check state after seek
        stream.seek(1);
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(1U, stream.position());
        EXPECT_EQ(0U, stream.remaining_size());
    }

    {
        SCOPED_TRACE(testing::Message() << "size 10000");
        uint64_t size = 10000U;
        endian::stream stream(size);

        // check initial state
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(0U, stream.position());
        EXPECT_EQ(size, stream.remaining_size());

        // check state after seek
        stream.seek(size / 2);
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(size / 2, stream.position());
        EXPECT_EQ(size / 2, stream.remaining_size());

        // that consecutive seeks doesn't alter state.
        stream.seek(size / 2);
        stream.seek(size / 2);
        stream.seek(size / 2);
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(size / 2, stream.position());
        EXPECT_EQ(size / 2, stream.remaining_size());

        // seek to end
        stream.seek(size);
        EXPECT_EQ(size, stream.position());
        EXPECT_EQ(0U, stream.remaining_size());
    }

    {
        SCOPED_TRACE(testing::Message() << "size 1");
        uint64_t size = 1U;
        endian::stream stream(size);

        // check initial state
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(0U, stream.position());
        EXPECT_EQ(size, stream.remaining_size());

        // check state after skip
        stream.skip(1);
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(1U, stream.position());
        EXPECT_EQ(0U, stream.remaining_size());
    }

    {
        SCOPED_TRACE(testing::Message() << "size 10000");
        uint64_t size = 10000U;
        endian::stream stream(size);

        // check initial state
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(0U, stream.position());
        EXPECT_EQ(size, stream.remaining_size());

        // check state after skip
        stream.skip(size / 2);
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(size / 2, stream.position());
        EXPECT_EQ(size / 2, stream.remaining_size());
        stream.skip(size / 2);
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(size, stream.position());
        EXPECT_EQ(0U, stream.remaining_size());

        // go back and skip each byte until the end
        stream.seek(0);
        for (uint64_t i = 0; i < size; ++i)
        {
            EXPECT_EQ(size - i, stream.remaining_size());
            ASSERT_NE(0U, stream.remaining_size());
            stream.skip(1);
        }

        EXPECT_EQ(size, stream.position());
        EXPECT_EQ(0U, stream.remaining_size());
    }
}

// ********************** //
//  Reader writer tests:  //
// ********************** //

template<class Type, class EndianType>
void write_and_read_value_type_test()
{
    using value_type = typename Type::type;
    const uint32_t elements = 1024;
    const uint64_t size = 1024 * Type::size;
    std::vector<uint8_t> buffer;
    buffer.resize(size);
    endian::stream_reader<EndianType> stream_reader(buffer.data(), size);
    endian::stream_writer<EndianType> stream_writer(buffer.data(), size);

    value_type lowest_value = Type::min;
    value_type highest_value = Type::max;

    for (uint32_t i = 0; i < elements; i++)
    {
        stream_writer.template write<Type>(highest_value);
    }

    stream_reader.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        value_type value = 0;
        stream_reader.template read<Type>(value);
        EXPECT_EQ(highest_value, value);
    }

    stream_writer.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        stream_writer.template write<Type>(lowest_value);
    }

    stream_reader.seek(0);
    for (uint32_t i = 0; i < elements; i++)
    {
        value_type value = 0;
        stream_reader.template read<Type>(value);
        EXPECT_EQ(lowest_value, value);
    }
}

template<class Type, class EndianType>
void write_and_read_random_value_type_test()
{
    using value_type = typename Type::type;
    const uint32_t elements = 1024;
    const uint64_t size = 1024 * Type::size;
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::stream_writer<EndianType> stream_writer(buffer.data(), size);
    endian::stream_reader<EndianType> stream_reader(buffer.data(), size);

    std::vector<value_type> values;
    values.resize(elements);

    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<uint64_t> distribution(
        Type::min, Type::max);

    for (uint32_t i = 0; i < elements; i++)
    {
        values[i] = (value_type)distribution(engine);
        stream_writer.template write<Type>(values[i]);
    }
    for (uint32_t i = 0; i < elements; i++)
    {
        value_type value;
        stream_reader.template read<Type>(value);
        EXPECT_EQ(values[i], value);
    }
}

template<class EndianType>
void write_and_read_variadic_types_test()
{
    const uint32_t elements = 1024;
    const uint64_t size = 1024 * sizeof(uint64_t);
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::stream_writer<EndianType> writer(buffer.data(), size);

    std::vector<uint64_t> values;
    values.resize(elements);

    std::random_device device;
    std::mt19937 engine(device());

    for (uint32_t i = 0; i < elements; i++)
    {
        switch (i % 4)
        {
        case 0:
        {
            std::uniform_int_distribution<uint64_t> distribution(
                endian::u8::min, endian::u8::max);
            values[i] = (endian::u8::type)distribution(engine);
            writer.template write<endian::u8>(values[i]);
            break;
        }
        case 1:
        {
            std::uniform_int_distribution<uint64_t> distribution(
                endian::u16::min, endian::u16::max);
            values[i] = (endian::u16::type)distribution(engine);
            writer.template write<endian::u16>(values[i]);
            break;
        }
        case 2:
        {
            std::uniform_int_distribution<uint64_t> distribution(
                endian::u24::min, endian::u24::max);
            values[i] = (endian::u24::type)distribution(engine);
            writer.template write<endian::u24>(values[i]);
            break;
        }
        case 3:
        {
            std::uniform_int_distribution<uint64_t> distribution(
                endian::u32::min, endian::u32::max);
            values[i] = (endian::u32::type)distribution(engine);
            writer.template write<endian::u32>(values[i]);
            break;
        }
        case 4:
        {
            std::uniform_int_distribution<uint64_t> distribution(
                endian::u40::min, endian::u40::max);
            values[i] = (endian::u40::type)distribution(engine);
            writer.template write<endian::u40>(values[i]);
            break;
        }
        case 5:
        {
            std::uniform_int_distribution<uint64_t> distribution(
                endian::u48::min, endian::u48::max);
            values[i] = (endian::u48::type)distribution(engine);
            writer.template write<endian::u48>(values[i]);
            break;
        }
        case 6:
        {
            std::uniform_int_distribution<uint64_t> distribution(
                endian::u56::min, endian::u56::max);
            values[i] = (endian::u56::type)distribution(engine);
            writer.template write<endian::u56>(values[i]);
            break;
        }
        case 7:
        {
            std::uniform_int_distribution<uint64_t> distribution(
                endian::u64::min, endian::u64::max);
            values[i] = (endian::u64::type)distribution(engine);
            writer.template write<endian::u64>(values[i]);
            break;
        }
        }
    }

    uint8_t last_u8 = 0;
    uint16_t last_u16 = 0;
    uint32_t last_u24 = 0;
    uint32_t last_u32 = 0;
    uint64_t last_u40 = 0;
    uint64_t last_u48 = 0;
    uint64_t last_u56 = 0;
    uint64_t last_u64 = 0;

    // create reader
    endian::stream_reader<EndianType> reader(buffer.data(), size);

    // Read values in FIFO order
    for (uint32_t i = 0; i < elements; i++)
    {
        switch (i % 4)
        {
        case 0:
            reader.template read<endian::u8>(last_u8);
            EXPECT_EQ(values[i], last_u8);
            break;
        case 1:
            reader.template read<endian::u16>(last_u16);
            EXPECT_EQ(values[i], last_u16);
            break;
        case 2:
            reader.template read<endian::u24>(last_u24);
            EXPECT_EQ(values[i], last_u24);
            break;
        case 3:
            reader.template read<endian::u32>(last_u32);
            EXPECT_EQ(values[i], last_u32);
            break;
        case 4:
            reader.template read<endian::u40>(last_u40);
            EXPECT_EQ(values[i], last_u40);
            break;
        case 5:
            reader.template read<endian::u48>(last_u48);
            EXPECT_EQ(values[i], last_u48);
            break;
        case 6:
            reader.template read<endian::u56>(last_u56);
            EXPECT_EQ(values[i], last_u56);
            break;
        case 7:
            reader.template read<endian::u64>(last_u64);
            EXPECT_EQ(values[i], last_u64);
            break;
        }
    }
}

template<class EndianType>
static void write_and_read_string_test()
{
    const uint64_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);

    endian::stream_writer<EndianType> writer(buffer.data(), size);

    std::string first("first first first");
    std::string second("second second");
    std::string third("third");

    // Write the strings together with their lengths
    // The length is written as 16-bit integers
    writer.template write<endian::u16>(first.size());
    writer.write((uint8_t*)first.c_str(), first.size());
    writer.template write<endian::u16>(second.size());
    writer.write((uint8_t*)second.c_str(), second.size());
    writer.template write<endian::u16>(third.size());
    writer.write((uint8_t*)third.c_str(), third.size());

    // Create reader
    endian::stream_reader<EndianType> reader(buffer.data(), size);

    // Read the strings together with their lengths
    {
        std::string string;
        uint16_t length;
        reader.template read<endian::u16>(length);
        EXPECT_EQ(first.size(), length);
        string.resize(length);
        reader.read((uint8_t*)string.c_str(), length);
        EXPECT_EQ(first, string);
    }
    {
        std::string string;
        uint16_t length;
        reader.template read<endian::u16>(length);
        EXPECT_EQ(second.size(), length);
        string.resize(length);
        reader.read((uint8_t*)string.c_str(), length);
        EXPECT_EQ(second, string);
    }
    {
        std::string string;
        uint16_t length;
        reader.template read<endian::u16>(length);
        EXPECT_EQ(third.size(), length);
        string.resize(length);
        reader.read((uint8_t*)string.c_str(), length);
        EXPECT_EQ(third, string);
    }
}

template<class EndianType>
static void run_read_write_vector_test()
{
    const uint64_t size = 1024;
    std::vector<uint8_t> buffer;
    buffer.resize(size);
    endian::stream_writer<EndianType> writer(buffer.data(), size);

    using first_type = uint8_t;
    std::vector<first_type> first(100, 'a');
    using second_type = uint32_t;
    std::vector<second_type> second(200, 1234);

    // Write the vectors together with their lengths
    // The length is written as 16-bit integers
    writer.template write<endian::u16>(first.size());
    writer.write((uint8_t*)first.data(), first.size() * sizeof(first_type));

    // The size here refers to the number of integers
    // stored in the second vector
    writer.template write<endian::u16>(second.size());
    writer.write((uint8_t*)second.data(), second.size() * sizeof(second_type));

    // Temp variables
    std::vector<first_type> first_out;
    std::vector<second_type> second_out;

    // Create reader
    endian::stream_reader<EndianType> reader(buffer.data(), size);

    {
        uint16_t length;
        reader.template read<endian::u16>(length);
        EXPECT_EQ(first.size(), length);

        std::vector<first_type> vector;
        vector.resize(length);
        reader.read((uint8_t*)vector.data(), length * sizeof(first_type));
        EXPECT_EQ(first, vector);
    }
    {
        uint16_t length;
        reader.template read<endian::u16>(length);
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
        SCOPED_TRACE("i8");
        write_and_read_value_type_test<endian::i8, EndianType>();
        write_and_read_random_value_type_test<endian::i8, EndianType>();
    }
    {
        SCOPED_TRACE("u8");
        write_and_read_value_type_test<endian::u8, EndianType>();
        write_and_read_random_value_type_test<endian::u8, EndianType>();
    }
    {
        SCOPED_TRACE("u16");
        write_and_read_value_type_test<endian::u16, EndianType>();
        write_and_read_random_value_type_test<endian::u16, EndianType>();
    }
    {
        SCOPED_TRACE("i16");
        write_and_read_value_type_test<endian::i16, EndianType>();
        write_and_read_random_value_type_test<endian::i16, EndianType>();
    }
    {
        SCOPED_TRACE("u24");
        write_and_read_value_type_test<endian::u24, EndianType>();
        write_and_read_random_value_type_test<endian::u24, EndianType>();
    }
    {
        SCOPED_TRACE("u32");
        write_and_read_value_type_test<endian::u32, EndianType>();
        write_and_read_random_value_type_test<endian::u32, EndianType>();
    }
    {
        SCOPED_TRACE("i32");
        write_and_read_value_type_test<endian::i32, EndianType>();
        write_and_read_random_value_type_test<endian::i32, EndianType>();
    }
    {
        SCOPED_TRACE("u40");
        write_and_read_value_type_test<endian::u40, EndianType>();
        write_and_read_random_value_type_test<endian::u40, EndianType>();
    }
    {
        SCOPED_TRACE("u48");
        write_and_read_value_type_test<endian::u48, EndianType>();
        write_and_read_random_value_type_test<endian::u48, EndianType>();
    }
    {
        SCOPED_TRACE("u56");
        write_and_read_value_type_test<endian::u56, EndianType>();
        write_and_read_random_value_type_test<endian::u56, EndianType>();
    }
    {
        SCOPED_TRACE("u64");
        write_and_read_value_type_test<endian::u64, EndianType>();
        write_and_read_random_value_type_test<endian::u64, EndianType>();
    }
    {
        SCOPED_TRACE("i64");
        write_and_read_value_type_test<endian::i64, EndianType>();
        write_and_read_random_value_type_test<endian::i64, EndianType>();
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
