// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>

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
    /// @param data a data pointer to the buffer
    /// @param size the size of the buffer in bytes
    stream_reader(const uint8_t* data, uint64_t size) :
        stream(size),
        m_data(data)
    {
        assert(m_data != nullptr && "Invalid buffer pointer provided");
    }

    /// Creates an endian stream on top of a pre-allocated buffer
    ///
    /// @param buffer a vector containing the buffer
    stream_reader(const std::vector<uint8_t>& buffer) :
        stream_reader(buffer.data(), buffer.size())
    { }

    /// Reads from the stream and moves the read position.
    ///
    /// @param value reference to the value to be read
    template<class Type>
    void read(typename Type::type& value)
    {
        // Make sure there is enough data to read in the underlying buffer
        assert(Type::size <= remaining_size());

        // Get the value at the current position
        peek<Type>(value);

        // Advance the current position
        m_position += Type::size;
    }

    /// Reads raw bytes from the stream to fill a buffer represented by
    /// a mutable storage object.
    ///
    /// Note, that this function is provided only for convenience and
    /// it does not perform any endian conversions.
    ///
    /// @param data The data pointer to fill into
    /// @param size The number of bytes to fill.
    void read(uint8_t* data, uint64_t size)
    {
        // Make sure there is enough data to read in the underlying buffer
        assert(size <= remaining_size());

        // Copy the data from the buffer to the storage
        std::copy_n(remaining_data(), (std::size_t)size, data);

        // Advance the current position
        m_position += size;
    }

    /// Peeks in the stream by reading at the current position
    /// without moving it.
    ///
    /// @param value reference to the value to be read
    /// @param offset number of bytes to offset the peeking with
    template<class Type>
    void peek(typename Type::type& value, uint64_t offset=0) const
    {
        assert(remaining_size() >= offset);
        // Make sure there is enough data to read in the underlying buffer
        assert(Type::size <= remaining_size() - offset);

        const uint8_t* data_position = remaining_data() + offset;
        // Get the value at the current position
        value = EndianType::template get_bytes<Type>(data_position);
    }

    /// A pointer to the stream's data.
    ///
    /// @return pointer to the stream's data.
    const uint8_t* data() const
    {
        return m_data;
    }

    /// A pointer to the stream's data at the current position.
    ///
    /// @return pointer to the stream's data at the current position.
    const uint8_t* remaining_data() const
    {
        return m_data + m_position;
    }

private:

    /// Data pointer to buffer
    const uint8_t* m_data;
};
}
