// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <endian/stream_reader.hpp>

#include <vector>
#include <cstdint>

#include <endian/big_endian.hpp>
#include <endian/little_endian.hpp>

#include <gtest/gtest.h>

template<class EndianType>
static void test_basic_api()
{
    {
        SCOPED_TRACE(testing::Message() << "size 1");
        uint32_t size = 1U;
        std::vector<uint8_t> buffer;
        buffer.resize(size);
        endian::stream_reader<EndianType> stream(buffer.data(), buffer.size());

        // check initial state
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(0U, stream.position());
        EXPECT_EQ(size, stream.remaining_size());
        EXPECT_EQ(buffer.data(), stream.remaining_data());
        EXPECT_EQ(buffer.data(), stream.data());

        // check state after seek
        stream.seek(1);
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(1U, stream.position());
        EXPECT_EQ(0U, stream.remaining_size());
        EXPECT_EQ(buffer.data() + 1, stream.remaining_data());
        EXPECT_EQ(buffer.data(), stream.data());
    }

    {
        SCOPED_TRACE(testing::Message() << "size 1000");
        uint32_t size = 1000U;
        std::vector<uint8_t> buffer;
        buffer.resize(size);
        endian::stream_reader<EndianType> stream(buffer.data(), buffer.size());

        // check initial state
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(0U, stream.position());
        EXPECT_EQ(size, stream.remaining_size());
        EXPECT_EQ(buffer.data(), stream.remaining_data());
        EXPECT_EQ(buffer.data(), stream.data());

        // check state after seek
        stream.seek(size / 2);
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(size / 2, stream.position());
        EXPECT_EQ(size / 2, stream.remaining_size());
        EXPECT_EQ(buffer.data() + size / 2, stream.remaining_data());
        EXPECT_EQ(buffer.data(), stream.data());

        // that consecutive seeks doesn't alter state.
        stream.seek(size / 2);
        stream.seek(size / 2);
        stream.seek(size / 2);
        EXPECT_EQ(size, stream.size());
        EXPECT_EQ(size / 2, stream.position());
        EXPECT_EQ(size / 2, stream.remaining_size());
        EXPECT_EQ(buffer.data() + size / 2, stream.remaining_data());
        EXPECT_EQ(buffer.data(), stream.data());

        // seek to end
        stream.seek(size);
        EXPECT_EQ(size, stream.position());
        EXPECT_EQ(0U, stream.remaining_size());
        EXPECT_EQ(buffer.data() + size, stream.remaining_data());
        EXPECT_EQ(buffer.data(), stream.data());
    }

    {
        SCOPED_TRACE(testing::Message() << "vector vs pointer");

        uint32_t size = 10U;
        std::vector<uint8_t> buffer;
        buffer.resize(size);
        endian::stream_reader<EndianType> stream1(buffer.data(), buffer.size());
        endian::stream_reader<EndianType> stream2(buffer.data(), buffer.size());
        EXPECT_EQ(stream1.size(), stream2.size());
        EXPECT_EQ(stream1.data(), stream2.data());
    }

    {
        SCOPED_TRACE(testing::Message() << "peek_bytes");
        std::vector<uint8_t> buffer = { 1, 2 };
        endian::stream_reader<EndianType> stream(buffer.data(), buffer.size());
        uint8_t first_peek = 0;
        uint8_t second_peek = 0;
        uint8_t first_read = 0;
        stream.template peek_bytes<1>(first_peek);
        stream.template peek_bytes<1>(second_peek);
        stream.template read_bytes<1>(first_read);
        EXPECT_EQ(first_peek, second_peek);
        EXPECT_EQ(first_peek, first_read);

        uint8_t second_read = 0;
        uint8_t third_peek = 0;
        stream.template peek_bytes<1>(third_peek);
        stream.template read_bytes<1>(second_read);
        EXPECT_NE(first_peek, third_peek);
        EXPECT_EQ(third_peek, second_read);
    }

    {
        SCOPED_TRACE(testing::Message() << "peek_read");
        std::vector<uint8_t> buffer = { 1, 2 };
        endian::stream_reader<EndianType> stream(buffer.data(), buffer.size());
        uint8_t first_peek = 0;
        uint8_t second_peek = 0;
        uint8_t first_read = 0;
        stream.peek(first_peek);
        stream.peek(second_peek);
        stream.read(first_read);
        EXPECT_EQ(first_peek, second_peek);
        EXPECT_EQ(first_peek, first_read);

        uint8_t second_read = 0;
        uint8_t third_peek = 0;
        stream.peek(third_peek);
        stream.read(second_read);
        EXPECT_NE(first_peek, third_peek);
        EXPECT_EQ(third_peek, second_read);
    }

    {
        SCOPED_TRACE(testing::Message() << "peek_read_return_value");
        std::vector<uint8_t> buffer = { 1, 2 };
        endian::stream_reader<EndianType> stream(buffer.data(), buffer.size());

        uint8_t first_peek = stream.template peek<uint8_t>();
        uint8_t second_peek = stream.template peek<uint8_t>();
        uint8_t first_read = stream.template read<uint8_t>();
        EXPECT_EQ(first_peek, second_peek);
        EXPECT_EQ(first_peek, first_read);

        auto third_peek = stream.template peek<uint8_t>();
        auto second_read = stream.template read<uint8_t>();
        EXPECT_NE(first_peek, third_peek);
        EXPECT_EQ(third_peek, second_read);
    }

    {
        SCOPED_TRACE(testing::Message() << "offset peek");
        std::vector<uint8_t> buffer = { 1, 2, 3, 4 };
        endian::stream_reader<EndianType> stream(buffer.data(), buffer.size());
        uint8_t first_peek_at_3 = 0;
        uint8_t second_peek_at_1 = 0;
        uint8_t first_read = 0;
        stream.template peek_bytes<1>(first_peek_at_3, 2);
        stream.template peek_bytes<1>(second_peek_at_1, 0);
        stream.template read_bytes<1>(first_read);
        EXPECT_NE(first_peek_at_3, second_peek_at_1);
        EXPECT_NE(first_peek_at_3, first_read);
        EXPECT_EQ(second_peek_at_1, first_read);
    }
}

TEST(test_stream_reader, basic_api_little_endian)
{
    test_basic_api<endian::little_endian>();
}

TEST(test_stream_reader, basic_api_big_endian)
{
    test_basic_api<endian::big_endian>();
}
