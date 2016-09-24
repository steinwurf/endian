// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <algorithm>

#include "stream.hpp"

namespace endian
{
/// The idea behind the stream_writer is to provide a stream-like
/// interface for accessing a fixed-size buffer.
/// All complexity regarding endianness is encapsulated.
template<class EndianType>
class stream_writer : public stream
{
public:

    /// Creates an endian stream on top of a pre-allocated buffer of the
    /// specified size.
    ///
    /// @param buffer a pointer to the buffer
    /// @param size the size of the buffer in bytes
    stream_writer(uint8_t* buffer, uint32_t size) :
        stream(size),
        m_buffer(buffer)
    {
        assert(m_buffer != nullptr);
    }

    /// Writes a value of ValueType type and size to the stream.
    ///
    /// @param value the value to write.
    template<class ValueType>
    void write(ValueType value)
    {
        // Make sure there is enough space in the underlying buffer
        assert(sizeof(ValueType) <= remaining_size());

        // Write the value at the current position
        EndianType::template put<ValueType>(value, remaining_data());

        // Advance the current position
        m_position += sizeof(ValueType);
    }

    /// Writes the raw bytes represented by the storage::const_storage
    /// object to the stream.
    ///
    /// Note, that this function is provided only for convenience and
    /// it does not perform any endian conversions.
    ///
    /// @param data Pointer to the data, to be written to the stream.
    /// @param size Number of bytes from the data pointer.
    void write(const uint8_t* data, uint32_t size)
    {
        // Make sure there is enough space in the underlying buffer
        assert(size <= remaining_size());

        // Copy the data to the buffer
        std::copy_n(data, size, remaining_data());

        // Advance the current position
        m_position += size;
    }

    /// A pointer to the stream's data.
    ///
    /// @return pointer to the stream's data.
    uint8_t* data() const
    {
        return m_buffer;
    }

    /// A pointer to the stream's data at the current position.
    ///
    /// @return pointer to the stream's data at the current position.
    uint8_t* remaining_data() const
    {
        return m_buffer + m_position;
    }

private:

    /// Pointer to buffer
    uint8_t* m_buffer;
};
}
