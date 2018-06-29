// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <endian/stream_writer.hpp>

#include <cstdint>
#include <vector>

#include <endian/big_endian.hpp>
#include <endian/little_endian.hpp>

#include <gtest/gtest.h>

template<class EndianType>
static void test_basic_api()
{
    {
        SCOPED_TRACE(testing::Message() << "size 1");
        uint32_t size = 1U;
        std::vector<uint8_t> buffer(size);
        endian::stream_writer<EndianType> stream(buffer.data(), buffer.size());

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
        std::vector<uint8_t> buffer(size);
        endian::stream_writer<EndianType> stream(buffer.data(), buffer.size());

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
        std::vector<uint8_t> buffer(size);
        endian::stream_writer<EndianType> stream1(buffer.data(), buffer.size());
        endian::stream_writer<EndianType> stream2(buffer.data(), buffer.size());
        EXPECT_EQ(stream1.size(), stream2.size());
        EXPECT_EQ(stream1.data(), stream2.data());
    }
}

TEST(test_stream_writer, basic_api_little_endian)
{
    test_basic_api<endian::little_endian>();
}

TEST(test_stream_writer, basic_api_big_endian)
{
    test_basic_api<endian::big_endian>();
}
