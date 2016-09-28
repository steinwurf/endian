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
/// The idea behind the stream_reader is to provide a stream-like
/// interface for accessing a fixed-size buffer.
/// All complexity regarding endianness is encapsulated.
template<class EndianType>
class stream_reader : public stream
{
public:

    /// Creates an endian stream on top of a pre-allocated buffer of the
    /// specified size.
    ///
    /// @param buffer a pointer to the buffer
    /// @param size the size of the buffer in bytes
    stream_reader(const uint8_t* buffer, uint32_t size) :
        stream(size),
        m_buffer(buffer)
    {
        assert(buffer != nullptr && "Invalid buffer");
    }

    /// Reads from the stream and moves the read position.
    ///
    /// @param value reference to the value to be read
    template<class ValueType>
    void read(ValueType& value)
    {
        // Make sure there is enough data to read in the underlying buffer
        assert(sizeof(ValueType) <= remaining_size());

        // Read the value at the current position
        value = EndianType::template get<ValueType>(remaining_data());

        // Advance the current position
        m_position += sizeof(ValueType);
    }

    /// Reads raw bytes from the stream to fill a buffer represented by
    /// a mutable storage object.
    ///
    /// Note, that this function is provided only for convenience and
    /// it does not perform any endian conversions.
    ///
    /// @param data The data pointer to fill into
    /// @param size The number of bytes to fill.
    void read(uint8_t* data, uint32_t size)
    {
        // Make sure there is enough data to read in the underlying buffer
        assert(size <= remaining_size());

        // Copy the data from the buffer to the storage
        std::copy_n(remaining_data(), size, data);

        // Advance the current position
        m_position += size;
    }

    /// A pointer to the stream's data.
    ///
    /// @return pointer to the stream's data.
    const uint8_t* data() const
    {
        return m_buffer;
    }

    /// A pointer to the stream's data at the current position.
    ///
    /// @return pointer to the stream's data at the current position.
    const uint8_t* remaining_data() const
    {
        return m_buffer + m_position;
    }

private:

    /// Pointer to buffer
    const uint8_t* m_buffer;
};
}
