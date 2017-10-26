// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>

#include "bytes.hpp"

namespace endian
{
// Inserts and extracts integers in little-endian format.
struct little_endian
{
    template<class ValueType>
    static ValueType get(const uint8_t* buffer);

    template<class ValueType>
    static void put(ValueType value, uint8_t* buffer);

    template<class Bytes>
    static typename Bytes::type get_bytes(const uint8_t* buffer);

    template<class Bytes>
    static void put_bytes(typename Bytes::type value, uint8_t* buffer);
};

template<>
inline uint8_t little_endian::get<uint8_t>(const uint8_t* buffer)
{
    assert(buffer != nullptr);
    return *buffer;
}

template<>
inline void little_endian::put<uint8_t>(uint8_t value, uint8_t* buffer)
{
    assert(buffer != nullptr);
    *buffer = value;
}

template<>
inline uint16_t little_endian::get<uint16_t>(const uint8_t* buffer)
{
    assert(buffer != nullptr);
    return (buffer[1] << 8) | buffer[0];
}

template<>
inline void little_endian::put<uint16_t>(uint16_t value, uint8_t* buffer)
{
    assert(buffer != nullptr);
    buffer[0] = (value & 0xFF);
    buffer[1] = (value >> 8 & 0xFF);
}

template<>
inline uint32_t little_endian::get<uint32_t>(const uint8_t* buffer)
{
    assert(buffer != nullptr);
    return (buffer[3] << 24) | (buffer[2] << 16) |
           (buffer[1] << 8)  | buffer[0];
}

template<>
inline void little_endian::put<uint32_t>(uint32_t value, uint8_t* buffer)
{
    assert(buffer != nullptr);
    buffer[0] = (value & 0xFF);
    buffer[1] = ((value >> 8) & 0xFF);
    buffer[2] = ((value >> 16) & 0xFF);
    buffer[3] = ((value >> 24) & 0xFF);
}

template<>
inline uint64_t little_endian::get<uint64_t>(const uint8_t* buffer)
{
    assert(buffer != nullptr);
    return (((uint64_t) buffer[7]) << 56) |
           (((uint64_t) buffer[6]) << 48) |
           (((uint64_t) buffer[5]) << 40) |
           (((uint64_t) buffer[4]) << 32) |
           (((uint64_t) buffer[3]) << 24) |
           (((uint64_t) buffer[2]) << 16) |
           (((uint64_t) buffer[1]) << 8)  |
           (((uint64_t) buffer[0]));
}

template<>
inline void little_endian::put<uint64_t>(uint64_t value, uint8_t* buffer)
{
    assert(buffer != nullptr);
    buffer[0] = (value & 0xFF);
    buffer[1] = ((value >> 8) & 0xFF);
    buffer[2] = ((value >> 16) & 0xFF);
    buffer[3] = ((value >> 24) & 0xFF);
    buffer[4] = ((value >> 32) & 0xFF);
    buffer[5] = ((value >> 40) & 0xFF);
    buffer[6] = ((value >> 48) & 0xFF);
    buffer[7] = ((value >> 56) & 0xFF);
}

/// Gets an 8-bit value integer from a data buffer.
/// @param buffer pointer to the data buffer
/// @return retrieved value from the data buffer
template<>
inline u8::type little_endian::get_bytes<u8>(const uint8_t* buffer)
{
    assert(buffer != nullptr);
    return get<u8::type>(buffer);
}

/// Inserts an 8-bit value integer into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template<>
inline void little_endian::put_bytes<u8>(u8::type value, uint8_t* buffer)
{
    assert(buffer != nullptr);
    put<u8::type>(value, buffer);
}

/// Gets an 8-bit signed value integer from a data buffer.
/// @param buffer pointer to the data buffer
/// @return retrieved value from the data buffer
template<>
inline i8::type little_endian::get_bytes<i8>(const uint8_t* buffer)
{
    return little_endian::get_bytes<u8>(buffer);
}

/// Inserts an 8-bit signed value integer into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template<>
inline void little_endian::put_bytes<i8>(i8::type value, uint8_t* buffer)
{
    little_endian::put_bytes<u8>(value, buffer);
}

/// Gets an 16-bit value integer from a data buffer.
/// @param buffer pointer to the data buffer
/// @return retrieved value from the data buffer
template<>
inline u16::type little_endian::get_bytes<u16>(const uint8_t* buffer)
{
    assert(buffer != nullptr);
    return get<u16::type>(buffer);
}

/// Inserts an 16-bit value integer into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template<>
inline void little_endian::put_bytes<u16>(u16::type value, uint8_t* buffer)
{
    assert(buffer != nullptr);
    put<u16::type>(value, buffer);
}

/// Gets an 16-bit signed value integer from a data buffer.
/// @param buffer pointer to the data buffer
/// @return retrieved value from the data buffer
template<>
inline i16::type little_endian::get_bytes<i16>(const uint8_t* buffer)
{
    return little_endian::get_bytes<u16>(buffer);
}

/// Inserts an 16-bit signed value integer into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template<>
inline void little_endian::put_bytes<i16>(i16::type value, uint8_t* buffer)
{
    little_endian::put_bytes<u16>(value, buffer);
}

/// Gets an 24-bit value integer from a data buffer.
/// @param buffer pointer to the data buffer
/// @return retrieved value from the data buffer
template<>
inline u24::type little_endian::get_bytes<u24>(const uint8_t* buffer)
{
    assert(buffer != nullptr);
    return (buffer[2] << 16) | (buffer[1] << 8)  | buffer[0];
}

/// Inserts an 24-bit value integer into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template<>
inline void little_endian::put_bytes<u24>(u24::type value, uint8_t* buffer)
{
    assert(buffer != nullptr);
    buffer[0] = ((value) & 0xFF);
    buffer[1] = ((value >> 8) & 0xFF);
    buffer[2] = ((value >> 16) & 0xFF);
}

/// Gets an 32-bit value integer from a data buffer.
/// @param buffer pointer to the data buffer
/// @return retrieved value from the data buffer
template<>
inline u32::type little_endian::get_bytes<u32>(const uint8_t* buffer)
{
    assert(buffer != nullptr);
    return get<u32::type>(buffer);
}

/// Inserts an 32-bit value integer into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template<>
inline void little_endian::put_bytes<u32>(u32::type value, uint8_t* buffer)
{
    assert(buffer != nullptr);
    put<u32::type>(value, buffer);
}

/// Gets an 32-bit signed value integer from a data buffer.
/// @param buffer pointer to the data buffer
/// @return retrieved value from the data buffer
template<>
inline i32::type little_endian::get_bytes<i32>(const uint8_t* buffer)
{
    return little_endian::get_bytes<u32>(buffer);
}

/// Inserts an 32-bit signed value integer into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template<>
inline void little_endian::put_bytes<i32>(i32::type value, uint8_t* buffer)
{
    little_endian::put_bytes<u32>(value, buffer);
}

/// Gets an 40-bit value integer from a data buffer.
/// @param buffer pointer to the data buffer
/// @return retrieved value from the data buffer
template<>
inline u40::type little_endian::get_bytes<u40>(const uint8_t* buffer)
{
    assert(buffer != nullptr);
    return (((uint64_t) buffer[4]) << 32) |
           (((uint64_t) buffer[3]) << 24) |
           (((uint64_t) buffer[2]) << 16) |
           (((uint64_t) buffer[1]) << 8)  |
           ((uint64_t) buffer[0]);
}

/// Inserts an 40-bit value integer into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template<>
inline void little_endian::put_bytes<u40>(u40::type value, uint8_t* buffer)
{
    assert(buffer != nullptr);
    buffer[0] = (value & 0xFF);
    buffer[1] = ((value >> 8) & 0xFF);
    buffer[2] = ((value >> 16) & 0xFF);
    buffer[3] = ((value >> 24) & 0xFF);
    buffer[4] = ((value >> 32) & 0xFF);
}

/// Gets an 48-bit value integer from a data buffer.
/// @param buffer pointer to the data buffer
/// @return retrieved value from the data buffer
template<>
inline u48::type little_endian::get_bytes<u48>(const uint8_t* buffer)
{
    assert(buffer != nullptr);
    return (((uint64_t) buffer[5]) << 40) |
           (((uint64_t) buffer[4]) << 32) |
           (((uint64_t) buffer[3]) << 24) |
           (((uint64_t) buffer[2]) << 16) |
           (((uint64_t) buffer[1]) << 8)  |
           (((uint64_t) buffer[0]));
}

/// Inserts an 48-bit value integer into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template<>
inline void little_endian::put_bytes<u48>(u48::type value, uint8_t* buffer)
{
    assert(buffer != nullptr);
    buffer[0] = (value & 0xFF);
    buffer[1] = ((value >> 8) & 0xFF);
    buffer[2] = ((value >> 16) & 0xFF);
    buffer[3] = ((value >> 24) & 0xFF);
    buffer[4] = ((value >> 32) & 0xFF);
    buffer[5] = ((value >> 40) & 0xFF);
}

/// Gets an 56-bit value integer from a data buffer.
/// @param buffer pointer to the data buffer
/// @return retrieved value from the data buffer
template<>
inline u56::type little_endian::get_bytes<u56>(const uint8_t* buffer)
{
    assert(buffer != nullptr);
    return (((uint64_t) buffer[6]) << 48) |
           (((uint64_t) buffer[5]) << 40) |
           (((uint64_t) buffer[4]) << 32) |
           (((uint64_t) buffer[3]) << 24) |
           (((uint64_t) buffer[2]) << 16) |
           (((uint64_t) buffer[1]) << 8)  |
           (((uint64_t) buffer[0]));
}

/// Inserts an 56-bit value integer into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template<>
inline void little_endian::put_bytes<u56>(u56::type value, uint8_t* buffer)
{
    assert(buffer != nullptr);
    buffer[0] = (value & 0xFF);
    buffer[1] = ((value >> 8) & 0xFF);
    buffer[2] = ((value >> 16) & 0xFF);
    buffer[3] = ((value >> 24) & 0xFF);
    buffer[4] = ((value >> 32) & 0xFF);
    buffer[5] = ((value >> 40) & 0xFF);
    buffer[6] = ((value >> 48) & 0xFF);
}

/// Gets an 64-bit value integer from a data buffer.
/// @param buffer pointer to the data buffer
/// @return retrieved value from the data buffer
template<>
inline u64::type little_endian::get_bytes<u64>(const uint8_t* buffer)
{
    assert(buffer != nullptr);
    return get<u64::type>(buffer);
}

/// Inserts an 64-bit value integer into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template<>
inline void little_endian::put_bytes<u64>(u64::type value, uint8_t* buffer)
{
    assert(buffer != nullptr);
    put<u64::type>(value, buffer);
}

/// Gets an 64-bit signed value integer from a data buffer.
/// @param buffer pointer to the data buffer
/// @return retrieved value from the data buffer
template<>
inline i64::type little_endian::get_bytes<i64>(const uint8_t* buffer)
{
    return little_endian::get_bytes<u64>(buffer);
}

/// Inserts an 64-bit signed value integer into the data buffer.
/// @param value to put in the data buffer
/// @param buffer pointer to the data buffer
template<>
inline void little_endian::put_bytes<i64>(i64::type value, uint8_t* buffer)
{
    little_endian::put_bytes<u64>(value, buffer);
}
}
