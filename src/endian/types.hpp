// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <limits>

namespace endian
{
struct u8
{
    using type = uint8_t;
    static constexpr uint32_t size = 1;
    static constexpr type max = std::numeric_limits<type>::max();
    static constexpr type min = std::numeric_limits<type>::min();
};
struct i8
{
    using type = int8_t;
    static constexpr uint32_t size = 1;
    static constexpr type max = std::numeric_limits<type>::max();
    static constexpr type min = std::numeric_limits<type>::min();
};
struct u16
{
    using type = uint16_t;
    static constexpr uint32_t size = 2;
    static constexpr type max = std::numeric_limits<type>::max();
    static constexpr type min = std::numeric_limits<type>::min();
};
struct i16
{
    using type = int16_t;
    static constexpr uint32_t size = 2;
    static constexpr type max = std::numeric_limits<type>::max();
    static constexpr type min = std::numeric_limits<type>::min();
};
struct u24
{
    using type = uint32_t;
    static constexpr uint32_t size = 3;
    static constexpr type max = 0x00FFFFFF;
    static constexpr type min = 0;
};
struct u32
{
    using type = uint32_t;
    static constexpr uint32_t size = 4;
    static constexpr type max = std::numeric_limits<type>::max();
    static constexpr type min = std::numeric_limits<type>::min();
};
struct i32
{
    using type = int32_t;
    static constexpr uint32_t size = 4;
    static constexpr type max = std::numeric_limits<type>::max();
    static constexpr type min = std::numeric_limits<type>::min();
};
struct u40
{
    using type = uint64_t;
    static constexpr uint32_t size = 5;
    static constexpr uint64_t max = 0x000000FFFFFFFFFF;
    static constexpr uint64_t min = 0;
};
struct u48
{
    using type = uint64_t;
    static constexpr uint32_t size = 6;
    static constexpr uint64_t max = 0x0000FFFFFFFFFFFF;
    static constexpr uint64_t min = 0;
};
struct u56
{
    using type = uint64_t;
    static constexpr uint32_t size = 7;
    static constexpr uint64_t max = 0x00FFFFFFFFFFFFFF;
    static constexpr uint64_t min = 0;
};
struct u64
{
    using type = uint64_t;
    static constexpr uint32_t size = 8;
    static constexpr type max = std::numeric_limits<type>::max();
    static constexpr type min = std::numeric_limits<type>::min();
};
struct i64
{
    using type = int64_t;
    static constexpr uint32_t size = 8;
    static constexpr type max = std::numeric_limits<type>::max();
    static constexpr type min = std::numeric_limits<type>::min();
};
}