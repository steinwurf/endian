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
/// The stream_reader provides a stream-like interface for reading from a
/// fixed-size buffer. All complexity regarding endianness is encapsulated.
template<typename EndianType, typename SizeType = uint64_t>
class stream_reader : public stream<const uint8_t*, SizeType>
{
public:

    /// Creates an endian stream on top of a pre-allocated buffer of the
    /// specified size.
    ///
    /// @param data a data pointer to the buffer
    /// @param size the size of the buffer in bytes
    stream_reader(const uint8_t* data, SizeType size) :
        stream<const uint8_t*, SizeType>(data, size)
    {
        assert(data != nullptr && "Null pointer provided");
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
    void read_bytes(ValueType& value) noexcept
    {
        assert(Bytes <= this->remaining_size() &&
               "Reading over the end of the underlying buffer");

        peek_bytes<Bytes, ValueType>(value);
        this->skip(Bytes);
    }

    /// Reads a ValueType-sized integer from the stream and moves the read
    /// position.
    ///
    /// @param value reference to the value to be read
    template<class ValueType>
    void read(ValueType& value) noexcept
    {
        assert(sizeof(ValueType) <= this->remaining_size() &&
               "Reading over the end of the underlying buffer");

        read_bytes<sizeof(ValueType), ValueType>(value);
    }

    /// Reads a ValueType-sized integer from the stream and moves the read
    /// position.
    ///
    /// @return the read value
    template<class ValueType>
    ValueType read() noexcept
    {
        assert(sizeof(ValueType) <= this->remaining_size() &&
               "Reading over the end of the underlying buffer");

        ValueType value;
        read(value);
        return value;
    }

    /// Reads raw bytes from the stream to fill a buffer represented by
    /// a mutable storage object.
    ///
    /// Note, that this function is provided only for convenience and
    /// it does not perform any endian conversions.
    ///
    /// @param data The data pointer to fill into
    /// @param size The number of bytes to fill.
    void read(uint8_t* data, SizeType size) noexcept
    {
        assert(size <= this->remaining_size() &&
               "Reading over the end of the underlying buffer");

        std::copy_n(this->remaining_data(), (std::size_t)size, data);
        this->skip(size);
    }

    /// Reads the raw bytes from the stream.
    ///
    /// Note, that this function is provided only for convenience and
    /// it does not perform any endian conversions.
    ///
    /// @param stream the stream to read from
    /// @param size Number of bytes to read and write
    void read(stream<const uint8_t*, SizeType>& s, SizeType size) noexcept
    {
        assert(size <= this->remaining_size());
        assert(size <= s.remaining_size());

        read(s.remaining_data(), size);
        s.skip(size);
    }

    /// Peek a Bytes-sized integer in the stream without moving the read
    /// position
    ///
    /// @param value reference to the value to be read
    /// @param offset number of bytes to offset the peeking with
    template<uint8_t Bytes, class ValueType>
    void peek_bytes(ValueType& value, SizeType offset=0) const noexcept
    {
        assert(this->remaining_size() >= offset && "Offset too large");
        assert(Bytes <= this->remaining_size() - offset &&
               "Reading over the end of the underlying buffer");

        const uint8_t* data_position = this->remaining_data() + offset;
        EndianType::template get_bytes<Bytes>(value, data_position);
    }

    /// Peek a ValueType-sized integer in the stream without moving the read
    /// position
    ///
    /// @param value reference to the value to be read
    /// @param offset number of bytes to offset the peeking with
    template<class ValueType>
    void peek(ValueType& value, SizeType offset=0) const noexcept
    {
        assert(this->remaining_size() >= offset && "Offset too large");
        assert(sizeof(ValueType) <= this->remaining_size() - offset &&
               "Reading over the end of the underlying buffer");

        peek_bytes<sizeof(ValueType), ValueType>(value, offset);
    }

    /// Peek a ValueType-sized integer in the stream without moving the read
    /// position
    ///
    /// @param offset number of bytes to offset the peeking with
    /// @return the peeked value
    template<class ValueType>
    ValueType peek(SizeType offset=0) const noexcept
    {
        assert(this->remaining_size() >= offset && "Offset too large");
        assert(sizeof(ValueType) <= this->remaining_size() - offset &&
               "Reading over the end of the underlying buffer");

        ValueType value;
        peek(value, offset);
        return value;
    }
};
}
