// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <limits>
#include <vector>
#include <type_traits>

namespace endian
{
/// @brief Base-class for the endian stream reader and writer.
template<typename DataPointerType>
class stream
{
    static_assert(std::is_pointer<DataPointerType>::value,
        "The template type must be a pointer type");

    using DataType = typename std::remove_pointer<DataPointerType>::type;

public:

    /// Creates an endian stream used to track a buffer of the specified size.
    ///
    /// @param size the size of the buffer in bytes
    stream(DataPointerType data, uint64_t size) :
        m_data(data), m_size(size)
    {
        assert(data != nullptr && "Null pointer provided");
        assert(size > 0 && "Empty buffer provided");
        assert(size <= std::numeric_limits<std::size_t>::max() &&
               "Size is too large to be handled on this OS.");
    }

    stream(std::vector<DataType>& vector)
        : stream(vector.data(), vector.size())
    { }

    /// Gets the size of the underlying buffer in bytes.
    ///
    /// @return the size of the buffer
    uint64_t size() const noexcept
    {
        return m_size;
    }

    /// Gets the current read/write position in the stream
    ///
    /// @return the current position.
    uint64_t position() const  noexcept
    {
        return m_position;
    }

    /// The remaining number of bytes in the stream
    ///
    /// @return the remaining number of bytes.
    uint64_t remaining_size() const noexcept
    {
        assert(m_size >= m_position);

        return m_size - m_position;
    }

    /// Changes the current read/write position in the stream. The
    /// position is absolute i.e. it is always relative to the
    /// beginning of the buffer which is position 0.
    ///
    /// @param new_position the new position
    void seek(uint64_t new_position) noexcept
    {
        assert(new_position <= m_size);

        m_position = new_position;
    }

    /// Skips over a given number of bytes in the stream
    ///
    /// @param bytes_to_skip the bytes to skip
    void skip(uint64_t bytes_to_skip) noexcept
    {
        assert(bytes_to_skip + m_position <= m_size);

        seek(m_position + bytes_to_skip);
    }

    /// A pointer to the stream's data.
    ///
    /// @return pointer to the stream's data.
    DataPointerType data() const noexcept
    {
        return m_data;
    }

    /// A pointer to the stream's data at the current position.
    ///
    /// @return pointer to the stream's data at the current position.
    DataPointerType remaining_data() const noexcept
    {
        return m_data + m_position;
    }

private:

    /// Data pointer to buffer
    DataPointerType m_data;

    /// The size of the buffer in bytes
    uint64_t m_size;

    /// The current position
    uint64_t m_position = 0;
};
}
