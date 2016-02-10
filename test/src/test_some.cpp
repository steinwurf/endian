// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <endian/some.hpp>
#include <gtest/gtest.h>

TEST(test_some, return_value_of_some_method)
{
    endian::some s;
    EXPECT_TRUE(s.some_method());
}
