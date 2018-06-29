// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <endian/detail/stream.hpp>

#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

TEST(test_stream, basicbuffer)
{
    std::vector<uint8_t> buffer(1);
    endian::detail::stream<uint8_t*> stream(buffer.data(), buffer.size());

    // check initial state
    EXPECT_EQ(buffer.size(), stream.size());
    EXPECT_EQ(0U, stream.position());
    EXPECT_EQ(buffer.size(), stream.remaining_size());

    // check state after skip
    stream.skip(1);
    EXPECT_EQ(buffer.size(), stream.size());
    EXPECT_EQ(1U, stream.position());
    EXPECT_EQ(0U, stream.remaining_size());
}

TEST(test_stream, seek)
{
    SCOPED_TRACE(testing::Message() << "size 10000");
    std::vector<uint8_t> buffer(10000U);
    endian::detail::stream<uint8_t*> stream(buffer.data(), buffer.size());

    // check initial state
    EXPECT_EQ(buffer.size(), stream.size());
    EXPECT_EQ(0U, stream.position());
    EXPECT_EQ(buffer.size(), stream.remaining_size());

    // check state after seek
    stream.seek(buffer.size() / 2);
    EXPECT_EQ(buffer.size(), stream.size());
    EXPECT_EQ(buffer.size() / 2, stream.position());
    EXPECT_EQ(buffer.size() / 2, stream.remaining_size());

    // that consecutive seeks doesn't alter state.
    stream.seek(buffer.size() / 2);
    stream.seek(buffer.size() / 2);
    stream.seek(buffer.size() / 2);
    EXPECT_EQ(buffer.size(), stream.size());
    EXPECT_EQ(buffer.size() / 2, stream.position());
    EXPECT_EQ(buffer.size() / 2, stream.remaining_size());

    // seek to end
    stream.seek(buffer.size());
    EXPECT_EQ(buffer.size(), stream.position());
    EXPECT_EQ(0U, stream.remaining_size());
}

TEST(test_stream, skip)
{
    SCOPED_TRACE(testing::Message() << "size 10000");
    std::vector<uint8_t> buffer(10000U);
    endian::detail::stream<uint8_t*> stream(buffer.data(), buffer.size());

    // check initial state
    EXPECT_EQ(buffer.size(), stream.size());
    EXPECT_EQ(0U, stream.position());
    EXPECT_EQ(buffer.size(), stream.remaining_size());

    // check state after skip
    stream.skip(buffer.size() / 2);
    EXPECT_EQ(buffer.size(), stream.size());
    EXPECT_EQ(buffer.size() / 2, stream.position());
    EXPECT_EQ(buffer.size() / 2, stream.remaining_size());
    stream.skip(buffer.size() / 2);
    EXPECT_EQ(buffer.size(), stream.size());
    EXPECT_EQ(buffer.size(), stream.position());
    EXPECT_EQ(0U, stream.remaining_size());

    // go back and skip each byte until the end
    stream.seek(0);
    for (uint64_t i = 0; i < buffer.size(); ++i)
    {
        EXPECT_EQ(buffer.size() - i, stream.remaining_size());
        ASSERT_NE(0U, stream.remaining_size());
        stream.skip(1);
    }

    EXPECT_EQ(buffer.size(), stream.position());
    EXPECT_EQ(0U, stream.remaining_size());
}
