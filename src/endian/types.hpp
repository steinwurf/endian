// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <type_traits>

namespace endian
{
struct u8
{
    using type = uint8_t;
    static constexpr uint32_t size = 1;
    static constexpr uint8_t max = 0xFF;
    static constexpr uint8_t min = 0x00;
};
struct i8
{
    using type = int8_t;
    static constexpr uint32_t size = 1;
    static constexpr int8_t max = 0X7F;
    static constexpr int8_t min = 0x80;
};
struct u16
{
    using type = uint16_t;
    static constexpr uint32_t size = 2;
    static constexpr uint16_t max = 0xFFFF;
    static constexpr uint16_t min = 0x0000;
};
struct i16
{
    using type = int16_t;
    static constexpr uint32_t size = 2;
    static constexpr int16_t max = 0x7FFF;
    static constexpr int16_t min = 0x8000;
};
struct u24
{
    using type = uint32_t;
    static constexpr uint32_t size = 3;
    static constexpr uint32_t max = 0x00FFFFFF;
    static constexpr uint32_t min = 0x00000000;
};
struct u32
{
    using type = uint32_t;
    static constexpr uint32_t size = 4;
    static constexpr uint32_t max = 0xFFFFFFFF;
    static constexpr uint32_t min = 0x00000000;
};
struct i32
{
    using type = int32_t;
    static constexpr uint32_t size = 4;
    static constexpr int32_t max = 0x7FFFFFFF;
    static constexpr int32_t min = 0x80000000;
};
struct u40
{
    using type = uint64_t;
    static constexpr uint32_t size = 5;
    static constexpr uint64_t max = 0x000000FFFFFFFFFF;
    static constexpr uint64_t min = 0x0000000000000000;
};
struct u48
{
    using type = uint64_t;
    static constexpr uint32_t size = 6;
    static constexpr uint64_t max = 0x0000FFFFFFFFFFFF;
    static constexpr uint64_t min = 0x0000000000000000;
};
struct u56
{
    using type = uint64_t;
    static constexpr uint32_t size = 7;
    static constexpr uint64_t max = 0x00FFFFFFFFFFFFFF;
    static constexpr uint64_t min = 0x0000000000000000;
};
struct u64
{
    using type = uint64_t;
    static constexpr uint32_t size = 8;
    static constexpr uint64_t max = 0xFFFFFFFFFFFFFFFF;
    static constexpr uint64_t min = 0x0000000000000000;
};
struct i64
{
    using type = int64_t;
    static constexpr uint32_t size = 8;
    static constexpr int64_t max = 0x7FFFFFFFFFFFFFFF;
    static constexpr int64_t min = 0x8000000000000000;
};
}