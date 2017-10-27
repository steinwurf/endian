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

    /// Reads a Bytes-sized integer from the stream and moves the read position.
    ///
    /// @param value reference to the value to be read
    template<uint8_t Bytes, class ValueType>
    void read_bytes(ValueType& value)
    {
        // Make sure there is enough data to read in the underlying buffer
        assert(Bytes <= remaining_size());

        // Get the value at the current position
        peek_bytes<Bytes, ValueType>(value);

        // Advance the current position
        m_position += Bytes;
    }

    /// Reads a ValueType-sized integer from the stream and moves the read
    /// position.
    ///
    /// @param value reference to the value to be read
    template<class ValueType>
    void read(ValueType& value)
    {
        // Make sure there is enough data to read in the underlying buffer
        assert(sizeof(ValueType) <= remaining_size());
        read_bytes<sizeof(ValueType), ValueType>(value);
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

    /// Peek a Bytes-sized integer in the stream without moving the read
    /// position
    ///
    /// @param value reference to the value to be read
    /// @param offset number of bytes to offset the peeking with
    template<uint8_t Bytes, class ValueType>
    void peek_bytes(ValueType& value, uint64_t offset=0) const
    {
        assert(remaining_size() >= offset);
        // Make sure there is enough data to read in the underlying buffer
        assert(Bytes <= remaining_size() - offset);

        const uint8_t* data_position = remaining_data() + offset;
        // Get the value at the current position
        EndianType::template get_bytes<Bytes>(value, data_position);
    }

    /// Peek a ValueType-sized integer in the stream without moving the read
    /// position
    ///
    /// @param value reference to the value to be read
    /// @param offset number of bytes to offset the peeking with
    template<class ValueType>
    void peek(ValueType& value, uint64_t offset=0) const
    {
        assert(remaining_size() >= offset);
        // Make sure there is enough data to read in the underlying buffer
        assert(sizeof(ValueType) <= remaining_size() - offset);
        peek_bytes<sizeof(ValueType), ValueType>(value, offset);
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
