// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>

namespace endian
{

// Inserts and extracts integers in big-endian format.
struct big_endian
{
    /// Gets an 8-bit value integer from a byte stream. Only exists for
    /// convenience in the template-based getters and putters.
    /// @param buffer pointer to the byte stream buffer
    /// @return retrieved value from the byte stream
    static uint8_t get8(const uint8_t* buffer)
    {
        assert(buffer != 0);
        return *buffer;
    }

    /// Inserts an 8-bit value integer into the byte stream. Only exists
    /// for convenience in the template-based getters and putters.
    /// @param value to put in the stream
    /// @param buffer pointer to the byte stream buffer
    static void put8(uint8_t value, uint8_t* buffer)
    {
        assert(buffer != 0);
        *buffer = value;
    }

    /// Gets a 16-bit value integer which is in big-endian format from
    /// a byte stream.
    /// @copydetails get8()
    static uint16_t get16(const uint8_t* buffer)
    {
        assert(buffer != 0);
        return (buffer[0] << 8) | buffer[1];
    }

    /// Inserts a 16-bit value into a byte stream in big-endian format.
    /// @copydetails put8()
    static void put16(uint16_t value, uint8_t* buffer)
    {
        assert(buffer != 0);

        buffer[1] = (value & 0xFF);
        buffer[0] = (value >> 8 & 0xFF);
    }

    /// Gets a 32-bit value integer which is in big-endian format from a
    /// byte stream.
    /// @copydetails get8()
    static uint32_t get32(const uint8_t* buffer)
    {
        assert(buffer != 0);
        return (buffer[0] << 24) | (buffer[1] << 16) |
               (buffer[2] << 8)  | buffer[3];
    }

    /// Inserts a 32-bit value into a byte stream in big-endian format.
    /// @copydetails put8()
    static void put32(uint32_t value, uint8_t* buffer)
    {
        assert(buffer != 0);

        buffer[3] = (value & 0xFF);
        buffer[2] = ((value >> 8) & 0xFF);
        buffer[1] = ((value >> 16) & 0xFF);
        buffer[0] = ((value >> 24) & 0xFF);
    }

    /// Gets a 64-bit value integer which is in big-endian format from a
    /// byte stream.
    /// @copydetails get8()
    static uint64_t get64(const uint8_t* buffer)
    {
        assert(buffer != 0);
        return (((uint64_t) buffer[0]) << 56) |
               (((uint64_t) buffer[1]) << 48) |
               (((uint64_t) buffer[2]) << 40) |
               (((uint64_t) buffer[3]) << 32) |
               (((uint64_t) buffer[4]) << 24) |
               (((uint64_t) buffer[5]) << 16) |
               (((uint64_t) buffer[6]) << 8)  |
               ((uint64_t) buffer[7]);
    }

    /// Inserts a 64-bit value into a byte stream in big-endian format.
    /// @copydetails put8()
    static void put64(uint64_t value, uint8_t* buffer)
    {
        assert(buffer != 0);

        buffer[7] = (value & 0xFF);
        buffer[6] = ((value >> 8) & 0xFF);
        buffer[5] = ((value >> 16) & 0xFF);
        buffer[4] = ((value >> 24) & 0xFF);
        buffer[3] = ((value >> 32) & 0xFF);
        buffer[2] = ((value >> 40) & 0xFF);
        buffer[1] = ((value >> 48) & 0xFF);
        buffer[0] = ((value >> 56) & 0xFF);
    }

    /// Template based put and get functions, the main reason for these is
    /// to allow generic code to be written where the "right" get/put
    /// function will be called based on the template parameter
    template<class ValueType>
    static void put(ValueType value, uint8_t* buffer);

    template<class ValueType>
    static ValueType get(const uint8_t* buffer);
};

template<>
inline void big_endian::put<uint8_t>(uint8_t value, uint8_t* buffer)
{
    big_endian::put8(value, buffer);
}

template<>
inline void big_endian::put<uint16_t>(uint16_t value, uint8_t* buffer)
{
    big_endian::put16(value, buffer);
}

template<>
inline void big_endian::put<uint32_t>(uint32_t value, uint8_t* buffer)
{
    big_endian::put32(value, buffer);
}

template<>
inline void big_endian::put<uint64_t>(uint64_t value, uint8_t* buffer)
{
    big_endian::put64(value, buffer);
}

template<>
inline uint8_t big_endian::get<uint8_t>(const uint8_t* buffer)
{
    return big_endian::get8(buffer);
}

template<>
inline uint16_t big_endian::get<uint16_t>(const uint8_t* buffer)
{
    return big_endian::get16(buffer);
}

template<>
inline uint32_t big_endian::get<uint32_t>(const uint8_t* buffer)
{
    return big_endian::get32(buffer);
}

template<>
inline uint64_t big_endian::get<uint64_t>(const uint8_t* buffer)
{
    return big_endian::get64(buffer);
}
}
