// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <cstring>
#include <limits>
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

template<>
struct little<float, 4>
{
    static_assert(std::numeric_limits<float>::is_iec559,
                  "Float type value is not iec559 compliant");
    static_assert(sizeof(float) == 4, "Float type must have a size of 4 bytes");

    static void put(float value, uint8_t* buffer)
    {
        assert(buffer != nullptr);

        uint32_t temp;
        memcpy(&temp, &value, sizeof(float));
        little<uint32_t, sizeof(float)>::put(temp, buffer);
    }

    static void get(float& value, const uint8_t* buffer)
    {
        assert(buffer != nullptr);

        uint32_t temp;
        little<uint32_t, sizeof(float)>::get(temp, buffer);
        memcpy(&value, &temp, sizeof(float));
    }
};

template<>
struct little<double, 8>
{
    static_assert(std::numeric_limits<double>::is_iec559,
                  "Double type value is not iec559 compliant");
    static_assert(sizeof(double) == 8, "Double type must have a size of 8 bytes");

    static void put(double value, uint8_t* buffer)
    {
        assert(buffer != nullptr);

        uint64_t temp;
        memcpy(&temp, &value, sizeof(double));
        little<uint64_t, sizeof(double)>::put(temp, buffer);
    }

    static void get(double& value, const uint8_t* buffer)
    {
        assert(buffer != nullptr);

        uint64_t temp;
        little<uint64_t, sizeof(double)>::get(temp, buffer);
        memcpy(&value, &temp, sizeof(double));
    }
};

}

// Inserts and extracts integers in little-endian format.
struct little_endian
{
    /// Gets a ValueType-sized integer value from a data buffer.
    /// @param value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<class ValueType>
    static void get(ValueType& value, const uint8_t* buffer)
    {
        detail::little<ValueType, sizeof(ValueType)>::get(value, buffer);
    }

    /// Gets a ValueType-sized integer value from a data buffer.
    /// @return value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<class ValueType>
    static ValueType get(const uint8_t* buffer)
    {
        ValueType value;
        get(value, buffer);
        return value;
    }

    /// Gets a Bytes-sized integer value from a data buffer.
    /// @param value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static void get_bytes(ValueType& value, const uint8_t* buffer)
    {
        detail::little<ValueType, Bytes>::get(value, buffer);
    }

    /// Gets a Bytes-sized integer value from a data buffer.
    /// @return value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static ValueType get_bytes(const uint8_t* buffer)
    {
        ValueType value;
        get_bytes<Bytes>(value, buffer);
        return value;
    }

    /// Inserts a ValueType-sized integer value into the data buffer.
    /// @param value to put in the data buffer
    /// @param buffer pointer to the data buffer
    template<class ValueType>
    static void put(ValueType value, uint8_t* buffer)
    {
        detail::little<ValueType, sizeof(ValueType)>::put(value, buffer);
    }

    /// Inserts a Bytes-sized integer value into the data buffer.
    /// @param value to put in the data buffer
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static void put_bytes(ValueType value, uint8_t* buffer)
    {
        detail::little<ValueType, Bytes>::put(value, buffer);
    }
};
}
