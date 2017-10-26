// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <type_traits>

namespace endian
{
namespace detail
{
template<class ValueType, uint8_t Bytes>
struct little
{
    static void put(ValueType value, uint8_t* buffer);
    static void get(ValueType& value, const uint8_t* buffer);
};

template<class ValueType>
struct little<ValueType, 1>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) <= 1, "ValueType too big.");
        assert(buffer != nullptr);
        assert(value <= 0xFF);
        *buffer = value;
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 1, "ValueType too small.");
        assert(buffer != nullptr);
        value = *buffer;
    }
};

template<class ValueType>
struct little<ValueType, 2>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) <= 2, "ValueType too big.");
        assert(buffer != nullptr);
        assert(value <= 0xFFFF);
        buffer[0] = (value >> 0 & 0xFF);
        buffer[1] = (value >> 8 & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 2, "ValueType too small.");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[1]) << 8) |
                (((ValueType) buffer[0]) << 0);
    }
};

template<class ValueType>
struct little<ValueType, 3>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        assert(value <= 0x00FFFFFF);
        buffer[0] = ((value) & 0xFF);
        buffer[1] = ((value >> 8) & 0xFF);
        buffer[2] = ((value >> 16) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 3, "ValueType too small.");
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[2]) << 16) |
                (((ValueType) buffer[1]) << 8)  |
                (((ValueType) buffer[0]) << 0);
    }
};

template<class ValueType>
struct little<ValueType, 4>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) <= 4, "ValueType too big.");
        assert(buffer != nullptr);
        buffer[0] = ((value >> 0) & 0xFF);
        buffer[1] = ((value >> 8) & 0xFF);
        buffer[2] = ((value >> 16) & 0xFF);
        buffer[3] = ((value >> 24) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 4, "ValueType too small.");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[3]) << 24) |
                (((ValueType) buffer[2]) << 16) |
                (((ValueType) buffer[1]) << 8)  |
                (((ValueType) buffer[0]) << 0);
    }
};

template<class ValueType>
struct little<ValueType, 5>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        assert(value <= 0x000000FFFFFFFFFF);
        buffer[0] = ((value >> 0) & 0xFF);
        buffer[1] = ((value >> 8) & 0xFF);
        buffer[2] = ((value >> 16) & 0xFF);
        buffer[3] = ((value >> 24) & 0xFF);
        buffer[4] = ((value >> 32) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 5, "ValueType too small.");
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[4]) << 32) |
                (((ValueType) buffer[3]) << 24) |
                (((ValueType) buffer[2]) << 16) |
                (((ValueType) buffer[1]) << 8)  |
                (((ValueType) buffer[0]) << 0);
    }
};

template<class ValueType>
struct little<ValueType, 6>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        assert(value <= 0x0000FFFFFFFFFFFF);
        buffer[0] = ((value >> 0) & 0xFF);
        buffer[1] = ((value >> 8) & 0xFF);
        buffer[2] = ((value >> 16) & 0xFF);
        buffer[3] = ((value >> 24) & 0xFF);
        buffer[4] = ((value >> 32) & 0xFF);
        buffer[5] = ((value >> 40) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 6, "ValueType too small.");
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[5]) << 40) |
                (((ValueType) buffer[4]) << 32) |
                (((ValueType) buffer[3]) << 24) |
                (((ValueType) buffer[2]) << 16) |
                (((ValueType) buffer[1]) << 8)  |
                (((ValueType) buffer[0]) << 0);
    }
};

template<class ValueType>
struct little<ValueType, 7>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        assert(value <= 0x00FFFFFFFFFFFFFF);
        buffer[0] = ((value >> 0) & 0xFF);
        buffer[1] = ((value >> 8) & 0xFF);
        buffer[2] = ((value >> 16) & 0xFF);
        buffer[3] = ((value >> 24) & 0xFF);
        buffer[4] = ((value >> 32) & 0xFF);
        buffer[5] = ((value >> 40) & 0xFF);
        buffer[6] = ((value >> 48) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 7, "ValueType too small.");
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[6]) << 48) |
                (((ValueType) buffer[5]) << 40) |
                (((ValueType) buffer[4]) << 32) |
                (((ValueType) buffer[3]) << 24) |
                (((ValueType) buffer[2]) << 16) |
                (((ValueType) buffer[1]) << 8)  |
                (((ValueType) buffer[0]) << 0);
    }
};

template<class ValueType>
struct little<ValueType, 8>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) <= 8, "ValueType too big.");
        assert(buffer != nullptr);
        buffer[0] = ((value >> 0) & 0xFF);
        buffer[1] = ((value >> 8) & 0xFF);
        buffer[2] = ((value >> 16) & 0xFF);
        buffer[3] = ((value >> 24) & 0xFF);
        buffer[4] = ((value >> 32) & 0xFF);
        buffer[5] = ((value >> 40) & 0xFF);
        buffer[6] = ((value >> 48) & 0xFF);
        buffer[7] = ((value >> 56) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 8, "ValueType too small.");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[7]) << 56) |
                (((ValueType) buffer[6]) << 48) |
                (((ValueType) buffer[5]) << 40) |
                (((ValueType) buffer[4]) << 32) |
                (((ValueType) buffer[3]) << 24) |
                (((ValueType) buffer[2]) << 16) |
                (((ValueType) buffer[1]) << 8)  |
                (((ValueType) buffer[0]) << 0);
    }
};
}

// Inserts and extracts integers in little-endian format.
struct little_endian
{
    /// Inserts a Bytes-sized integer value into the data buffer.
    /// @param value to put in the data buffer
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static void put(ValueType value, uint8_t* buffer)
    {
        detail::little<ValueType, Bytes>::put(value, buffer);
    }

    /// Gets a Bytes-sized integer value from a data buffer.
    /// @param value to insert into the data buffer
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static void get(ValueType& value, const uint8_t* buffer)
    {
        detail::little<ValueType, Bytes>::get(value, buffer);
    }
};
}
