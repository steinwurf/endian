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
struct big
{
    static void put(ValueType value, uint8_t* buffer);
    static void get(ValueType& value, const uint8_t* buffer);
};

template<class ValueType>
struct big<ValueType, 1>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) <= 1, "ValueType too big.");
        assert(buffer != nullptr);
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
struct big<ValueType, 2>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) <= 2, "ValueType too big.");
        assert(buffer != nullptr);
        buffer[1] = ((value >> 0) & 0xFF);
        buffer[0] = ((value >> 8) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 2, "ValueType too small.");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[0]) << 8) |
                (((ValueType) buffer[1]) << 0);
    }
};

template<class ValueType>
struct big<ValueType, 3>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        assert(value <= 0x00FFFFFF);
        buffer[2] = ((value >> 0) & 0xFF);
        buffer[1] = ((value >> 8) & 0xFF);
        buffer[0] = ((value >> 16) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 3, "ValueType too small.");
        static_assert(std::is_unsigned<ValueType>::value, "must be unsigned");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[0]) << 16) |
                (((ValueType) buffer[1]) << 8)  |
                (((ValueType) buffer[2]) << 0);
    }
};

template<class ValueType>
struct big<ValueType, 4>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) <= 4, "ValueType too big.");
        assert(buffer != nullptr);
        buffer[3] = ((value >> 0) & 0xFF);
        buffer[2] = ((value >> 8) & 0xFF);
        buffer[1] = ((value >> 16) & 0xFF);
        buffer[0] = ((value >> 24) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 4, "ValueType too small.");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[0]) << 24) |
                (((ValueType) buffer[1]) << 16) |
                (((ValueType) buffer[2]) << 8)  |
                (((ValueType) buffer[3]) << 0);
    }
};

template<class ValueType>
struct big<ValueType, 5>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        assert(value <= 0x000000FFFFFFFFFF);
        buffer[4] = ((value >> 0) & 0xFF);
        buffer[3] = ((value >> 8) & 0xFF);
        buffer[2] = ((value >> 16) & 0xFF);
        buffer[1] = ((value >> 24) & 0xFF);
        buffer[0] = ((value >> 32) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 5, "ValueType too small.");
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[0]) << 32) |
                (((ValueType) buffer[1]) << 24) |
                (((ValueType) buffer[2]) << 16) |
                (((ValueType) buffer[3]) << 8)  |
                (((ValueType) buffer[4]) << 0);
    }
};

template<class ValueType>
struct big<ValueType, 6>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        assert(value <= 0x0000FFFFFFFFFFFF);
        buffer[5] = ((value >> 0) & 0xFF);
        buffer[4] = ((value >> 8) & 0xFF);
        buffer[3] = ((value >> 16) & 0xFF);
        buffer[2] = ((value >> 24) & 0xFF);
        buffer[1] = ((value >> 32) & 0xFF);
        buffer[0] = ((value >> 40) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 6, "ValueType too small.");
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[0]) << 40) |
                (((ValueType) buffer[1]) << 32) |
                (((ValueType) buffer[2]) << 24) |
                (((ValueType) buffer[3]) << 16) |
                (((ValueType) buffer[4]) << 8)  |
                (((ValueType) buffer[5]) << 0);
    }
};

template<class ValueType>
struct big<ValueType, 7>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        assert(value <= 0x00FFFFFFFFFFFFFF);
        buffer[6] = ((value >> 0) & 0xFF);
        buffer[5] = ((value >> 8) & 0xFF);
        buffer[4] = ((value >> 16) & 0xFF);
        buffer[3] = ((value >> 24) & 0xFF);
        buffer[2] = ((value >> 32) & 0xFF);
        buffer[1] = ((value >> 40) & 0xFF);
        buffer[0] = ((value >> 48) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 7, "ValueType too small.");
        static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[0]) << 48) |
                (((ValueType) buffer[1]) << 40) |
                (((ValueType) buffer[2]) << 32) |
                (((ValueType) buffer[3]) << 24) |
                (((ValueType) buffer[4]) << 16) |
                (((ValueType) buffer[5]) << 8)  |
                (((ValueType) buffer[6]) << 0);
    }
};

template<class ValueType>
struct big<ValueType, 8>
{
    static void put(ValueType value, uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) <= 8, "ValueType too big.");
        assert(buffer != nullptr);
        buffer[7] = ((value >> 0) & 0xFF);
        buffer[6] = ((value >> 8) & 0xFF);
        buffer[5] = ((value >> 16) & 0xFF);
        buffer[4] = ((value >> 24) & 0xFF);
        buffer[3] = ((value >> 32) & 0xFF);
        buffer[2] = ((value >> 40) & 0xFF);
        buffer[1] = ((value >> 48) & 0xFF);
        buffer[0] = ((value >> 56) & 0xFF);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= 8, "ValueType too small.");
        assert(buffer != nullptr);
        value = (((ValueType) buffer[0]) << 56) |
                (((ValueType) buffer[1]) << 48) |
                (((ValueType) buffer[2]) << 40) |
                (((ValueType) buffer[3]) << 32) |
                (((ValueType) buffer[4]) << 24) |
                (((ValueType) buffer[5]) << 16) |
                (((ValueType) buffer[6]) << 8)  |
                (((ValueType) buffer[7]) << 0);
    }
};
}

// Inserts and extracts integers in big-endian format.
struct big_endian
{
    /// Inserts a Bytes-sized integer value into the data buffer.
    /// @param value to put in the data buffer
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static void put(ValueType value, uint8_t* buffer)
    {
        detail::big<ValueType, Bytes>::put(value, buffer);
    }

    /// Gets a Bytes-sized integer value from a data buffer.
    /// @param value to insert into the data buffer
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static void get(ValueType& value, const uint8_t* buffer)
    {
        detail::big<ValueType, Bytes>::get(value, buffer);
    }

    template<class ValueType>
    static void put(ValueType value, uint8_t* buffer)
    {
        detail::big<ValueType, sizeof(ValueType)>::put(value, buffer);
    }

    template<class ValueType>
    static void get(ValueType& value, const uint8_t* buffer)
    {
        detail::big<ValueType, sizeof(ValueType)>::get(value, buffer);
    }
};
}
