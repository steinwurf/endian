// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <limits>

namespace endian
{
namespace detail
{

template<class ValueType, uint8_t Bytes>
struct big
{
    static void put(ValueType& value, uint8_t* buffer)
    {
        big<ValueType, Bytes - 1>::put(value, buffer + 1);
        *buffer = value & 0xFF;
        value = (value >> 8);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        value = (value << 8);
        value |= ((ValueType) *buffer);
        big<ValueType, Bytes - 1>::get(value, buffer + 1);
    }
};

template<class ValueType>
struct big<ValueType, 0>
{
    static void put(ValueType& value, uint8_t* buffer)
    {
        (void) buffer;
        (void) value;
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        (void) buffer;
        (void) value;
    }
};

template<>
struct big<float, 4>
{
    static_assert(std::numeric_limits<float>::is_iec559,
                  "Float type value is not iec559 compliant");
    static_assert(sizeof(float) == 4, "Float type must have a size of 4 bytes");

    static void put(float value, uint8_t* buffer)
    {
        assert(buffer != nullptr);

        uint32_t temp = 0;
        memcpy(&temp, &value, sizeof(float));
        big<uint32_t, sizeof(float)>::put(temp, buffer);
    }

    static void get(float& value, const uint8_t* buffer)
    {
        assert(buffer != nullptr);

        uint32_t temp = 0;
        big<uint32_t, sizeof(float)>::get(temp, buffer);
        memcpy(&value, &temp, sizeof(float));
    }
};

template<>
struct big<double, 8>
{
    static_assert(std::numeric_limits<double>::is_iec559,
                  "Double type value is not iec559 compliant");
    static_assert(sizeof(double) == 8, "Double type must have a size of 8 bytes");

    static void put(double value, uint8_t* buffer)
    {
        assert(buffer != nullptr);

        uint64_t temp = 0;
        memcpy(&temp, &value, sizeof(double));
        big<uint64_t, sizeof(double)>::put(temp, buffer);
    }

    static void get(double& value, const uint8_t* buffer)
    {
        assert(buffer != nullptr);

        uint64_t temp = 0;
        big<uint64_t, sizeof(double)>::get(temp, buffer);
        memcpy(&value, &temp, sizeof(double));
    }
};

}

// Inserts and extracts integers in big-endian format.
struct big_endian
{
    /// Gets a ValueType-sized integer value from a data buffer.
    /// @param value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<class ValueType>
    static void get(ValueType& value, const uint8_t* buffer)
    {
        assert(buffer != nullptr);

        value = 0;
        detail::big<ValueType, sizeof(ValueType)>::get(value, buffer);
    }

    /// Gets a ValueType-sized integer value from a data buffer.
    /// @return value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<class ValueType>
    static ValueType get(const uint8_t* buffer)
    {
        assert(buffer != nullptr);

        ValueType value = 0;
        get(value, buffer);
        return value;
    }

    /// Gets a Bytes-sized integer value from a data buffer.
    /// @param value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static void get_bytes(ValueType& value, const uint8_t* buffer)
    {
        // static_assert(std::is_unsigned<ValueType>::value, "Must be unsigned");
        assert(buffer != nullptr);

        value = 0;
        detail::big<ValueType, Bytes>::get(value, buffer);
    }

    /// Gets a Bytes-sized integer value from a data buffer.
    /// @return value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static ValueType get_bytes(const uint8_t* buffer)
    {
        assert(buffer != nullptr);

        ValueType value = 0;
        get_bytes<Bytes>(value, buffer);
        return value;
    }

    /// Inserts a ValueType-sized integer value into the data buffer.
    /// @param value to put in the data buffer
    /// @param buffer pointer to the data buffer
    template<class ValueType>
    static void put(ValueType value, uint8_t* buffer)
    {
        assert(buffer != nullptr);

        detail::big<ValueType, sizeof(ValueType)>::put(value, buffer);
    }

    /// Inserts a Bytes-sized integer value into the data buffer.
    /// @param value to put in the data buffer
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static void put_bytes(ValueType value, uint8_t* buffer)
    {
        assert(buffer != nullptr);
        // @TODO add a check that the value is not too big - in case of partial valuetypes

        detail::big<ValueType, Bytes>::put(value, buffer);
    }
};
}
