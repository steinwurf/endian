// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <limits>
#include <type_traits>

namespace endian
{
namespace detail
{

// Helper to convet floating point type into identically sized unsigned integer
template<class Type>
struct FloatType
{ };

template<>
struct FloatType<float>
{
    static_assert(sizeof(float) == 4, "Float type must have a size of 4 bytes");
    using UnsignedType = uint32_t;
};

template<>
struct FloatType<double>
{
    static_assert(sizeof(double) == 8, "Float type must have a size of 8 bytes");
    using UnsignedType = uint64_t;
};


template<class ValueType, uint8_t Bytes>
struct big_impl
{
    static void put(ValueType& value, uint8_t* buffer)
    {
        big_impl<ValueType, Bytes - 1>::put(value, buffer + 1);

        value = (value >> 8);
        *buffer = value & 0xFF;
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        value |= ((ValueType) *buffer);
        value = (value << 8);

        big_impl<ValueType, Bytes - 1>::get(value, buffer + 1);
    }
};

template<class ValueType>
struct big_impl<ValueType, 1>
{
    static void put(ValueType& value, uint8_t* buffer)
    {
        *buffer = value & 0xFF;
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        value |= ((ValueType) *buffer);
    }
};


// Wrapper to allow for different checks depending on the Value type,
// @TODO remove this wrapper when we have CXX17 support and "if constexpr"
template<class ValueType, uint8_t Bytes, bool isUnsigened, bool isIEC559>
struct big
{ };

// Unsigned specialization
template<class ValueType, uint8_t Bytes>
struct big<ValueType, Bytes, true, false>
{
    static void put(ValueType& value, uint8_t* buffer)
    {
        static_assert(Bytes > sizeof(ValueType) / 2,
        "Inappropiate value type used for the provided bytes, user small type");
        big_impl<ValueType, Bytes>::put(value, buffer);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(Bytes > sizeof(ValueType) / 2,
        "Inappropiate value type used for the provided bytes, user small type");

        big_impl<ValueType, Bytes>::get(value, buffer);
    }
};

// Unsigned specialization with 3 bytes
template<class ValueType>
struct big<ValueType, 3, true, false>
{
    static void put(ValueType& value, uint8_t* buffer)
    {
        static_assert(3 > sizeof(ValueType) / 2,
        "Inappropiate value type used for the provided bytes, user small type");
        assert(value <= 0xFFFFFF && "Provided value too big");

        big_impl<ValueType, 3>::put(value, buffer);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(3 > sizeof(ValueType) / 2,
        "Inappropiate value type used for the provided bytes, user small type");

        big_impl<ValueType, 3>::get(value, buffer);
    }
};

// Unsigned specialization with 5 bytes
template<class ValueType>
struct big<ValueType, 5, true, false>
{
    static void put(ValueType& value, uint8_t* buffer)
    {
        static_assert(5 > sizeof(ValueType) / 2,
        "Inappropiate value type used for the provided bytes, user small type");
        assert(value <= 0xFFFFFFFFFF && "Provided value too big");

        big_impl<ValueType, 5>::put(value, buffer);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(5 > sizeof(ValueType) / 2,
        "Inappropiate value type used for the provided bytes, user small type");

        big_impl<ValueType, 5>::get(value, buffer);
    }
};


// Signed specialization
template<class ValueType, uint8_t Bytes>
struct big<ValueType, Bytes, false, false>
{
    static void put(ValueType& value, uint8_t* buffer)
    {
        static_assert(Bytes == sizeof(ValueType),
            "The number of bytes must match the size of the signed type");

        big_impl<ValueType, Bytes>::put(value, buffer);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        static_assert(Bytes == sizeof(ValueType),
            "The number of bytes must match the size of the signed type");

        big_impl<ValueType, Bytes>::get(value, buffer);
    }
};

// IEC569 specialization
template<class ValueType, uint8_t Bytes>
struct big<ValueType, Bytes, false, true>
{
    static_assert(std::numeric_limits<ValueType>::is_iec559,
        "Platform must be iec559 compliant when floating point types are used");

    static void put(ValueType& value, uint8_t* buffer)
    {
        assert(buffer != nullptr);

        typename FloatType<ValueType>::UnsignedType temp = 0;
        memcpy(&temp, &value, sizeof(ValueType));
        big<decltype(temp), sizeof(ValueType), true, false>::put(temp, buffer);
    }

    static void get(ValueType& value, const uint8_t* buffer)
    {
        assert(buffer != nullptr);

        typename FloatType<ValueType>::UnsignedType temp = 0;
        big<decltype(temp), sizeof(ValueType), true, false>::get(temp, buffer);
        memcpy(&value, &temp, sizeof(ValueType));
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
        detail::big<ValueType, sizeof(ValueType),
            std::is_unsigned<ValueType>::value, std::is_floating_point<ValueType>::value>::get(
                value, buffer);
    }

    /// Gets a ValueType-sized integer value from a data buffer.
    /// @return value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<class ValueType>
    static ValueType get(const uint8_t* buffer)
    {
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
        static_assert(sizeof(ValueType) >= Bytes, "ValueType too small");
        assert(buffer != nullptr);

        value = 0;
        detail::big<ValueType, Bytes, std::is_unsigned<ValueType>::value, std::is_floating_point<ValueType>::value>::get(
            value, buffer);
    }

    /// Gets a Bytes-sized integer value from a data buffer.
    /// @return value variable where to get the value
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static ValueType get_bytes(const uint8_t* buffer)
    {
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

        detail::big<ValueType, sizeof(ValueType),
            std::is_unsigned<ValueType>::value, std::is_floating_point<ValueType>::value>::put(
                value, buffer);
    }

    /// Inserts a Bytes-sized integer value into the data buffer.
    /// @param value to put in the data buffer
    /// @param buffer pointer to the data buffer
    template<uint8_t Bytes, class ValueType>
    static void put_bytes(ValueType value, uint8_t* buffer)
    {
        static_assert(sizeof(ValueType) >= Bytes, "ValueType too small");
        assert(buffer != nullptr);

        detail::big<ValueType, Bytes, std::is_unsigned<ValueType>::value,
        std::is_floating_point<ValueType>::value>::put(
            value, buffer);
    }
};
}
