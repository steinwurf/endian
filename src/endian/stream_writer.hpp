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
/// The stream_writer provides a stream-like interface for writing to a fixed
/// size buffer. All complexity regarding endianness is encapsulated.
template<typename EndianType>
class stream_writer : public detail::stream<uint8_t*>
{
    static_assert(
        std::numeric_limits<size_type>::max() >=
        std::numeric_limits<std::vector<uint8_t>::size_type>::max(),
        "std::vector::size() can return a value we cannot hold in size_type");

public:

    /// Creates an endian stream on top of a pre-allocated buffer of the
    /// specified size.
    ///
    /// @param data a data pointer to the buffer
    /// @param size the size of the buffer in bytes
    stream_writer(uint8_t* data, size_type size) :
        stream(data, size)
    {
        assert(data != nullptr && "Null pointer provided");
        assert(size > 0 && "Empty buffer provided");
    }

    /// Creates an endian stream on top of a pre-allocated buffer
    ///
    /// @param buffer a vector containing the buffer
    stream_writer(std::vector<uint8_t>& buffer) :
        stream_writer(buffer.data(), buffer.size())
    { }

    /// Writes a Bytes-sized integer to the stream.
    ///
    /// @param value the value to write.
    template<uint8_t Bytes, class ValueType>
    void write_bytes(const ValueType value) noexcept
    {
        assert(Bytes <= remaining_size());

        EndianType::template put_bytes<Bytes>(value, this->remaining_data());
        skip(Bytes);
    }

    /// Writes a Bytes-sized integer to the stream.
    ///
    /// @param value the value to write.
    template<class ValueType>
    void write(const ValueType value) noexcept
    {
        assert(sizeof(ValueType) <= remaining_size());

        write_bytes<sizeof(ValueType), const ValueType>(value);
    }

    /// Writes the raw bytes represented by the storage::const_storage
    /// object to the stream.
    ///
    /// Note, that this function is provided only for convenience and
    /// it does not perform any endian conversions.
    ///
    /// @param data Pointer to the data, to be written to the stream.
    /// @param size Number of bytes from the data pointer.
    void write(const uint8_t* data, size_type size) noexcept
    {
        assert(size <= remaining_size());

        std::copy_n(data, (std::size_t)size, this->remaining_data());
        skip(size);
    }
};
}
