#pragma once
#include "stream_reader.hpp"
#include "stream_writer.hpp"

#ifndef NETWORK_ENDIAN_USE_LITTLE
#include "big_endian.hpp"
#define ENDIAN_NAMESPACE big_endian
#else
#include "little_endian.hpp"
#define ENDIAN_NAMESPACE little_endian
#endif

namespace endian
{
namespace network
{
/// Inserts a ValueType-sized value into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template <typename ValueType>
inline auto put(ValueType value, uint8_t* buffer)
{
    ENDIAN_NAMESPACE::put(value, buffer);
}

/// Gets a ValueType-sized integer value from a data buffer.
/// @param value variable where to get the value
/// @param buffer pointer to the data buffer
template <typename ValueType>
inline ValueType get(ValueType& value, uint8_t* buffer)
{
    return ENDIAN_NAMESPACE::get(value, buffer);
}

/// Gets a ValueType-sized integer value from a data buffer.
/// @return value variable where to get the value
/// @param buffer pointer to the data buffer
template <typename ValueType>
inline ValueType get(uint8_t* buffer)
{
    return ENDIAN_NAMESPACE::get<ValueType>(buffer);
}

/// Inserts a Bytes-sized integer value into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template <uint8_t Bytes, class ValueType>
static void put_bytes(ValueType value, uint8_t* buffer)
{
    return ENDIAN_NAMESPACE::put_bytes<Bytes>(value, buffer);
}

/// Gets a Bytes-sized integer value from a data buffer.
/// @param value variable where to get the value
/// @param buffer pointer to the data buffer
template <uint8_t Bytes, class ValueType>
static void get_bytes(ValueType& value, const uint8_t* buffer)
{
    return ENDIAN_NAMESPACE::get_bytes<Bytes>(value, buffer);
}

/// Gets a Bytes-sized integer value from a data buffer.
/// @return value variable where to get the value
/// @param buffer pointer to the data buffer
template <uint8_t Bytes, class ValueType>
static ValueType get_bytes(const uint8_t* buffer)
{
    return ENDIAN_NAMESPACE::get_bytes<Bytes, ValueType>(buffer);
}

/// The stream_reader provides a stream-like interface for reading from a
/// fixed-size buffer. All complexity regarding endianness is encapsulated.
using stream_reader = stream_reader<ENDIAN_NAMESPACE>;

/// The stream_writer provides a stream-like interface for writing to a fixed
/// size buffer. All complexity regarding endianness is encapsulated.
using stream_writer = stream_writer<ENDIAN_NAMESPACE>;
}
}