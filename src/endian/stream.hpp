// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <system_error>

namespace endian
{
/// @brief Base-class for the endian stream reader and writer.
class stream
{
public:

    /// Creates an endian stream used to track a buffer of the specified size.
    ///
    /// @param size the size of the buffer in bytes
    stream(uint32_t size) :
        m_size(size)
    {
        assert(m_size > 0 && "Tracking buffer of size zero is not valid");
    }

    /// Gets the size of the underlying buffer in bytes.
    ///
    /// @return the size of the buffer
    uint32_t size() const
    {
        return m_size;
    }

    /// Gets the current read/write position in the stream
    ///
    /// @return the current position.
    uint32_t position() const
    {
        return m_position;
    }

    /// The remaining number of bytes in the stream
    ///
    /// @return the remaining number of bytes.
    uint32_t remaining_size() const
    {
        return m_size - m_position;
    }

    /// Changes the current read/write position in the stream. The
    /// position is absolute i.e. it is always relative to the
    /// beginning of the buffer which is position 0.
    ///
    /// @param new_position the new position
    /// @param error reference to the error code which will be set if the seek
    ///              failed
    void seek(uint32_t new_position, std::error_code& error)
    {
        assert(!error);
        if (new_position > m_size)
        {
            error = std::make_error_code(std::errc::value_too_large);
            return;
        }
        m_position = new_position;
    }

    /// Changes the current read/write position in the stream. The
    /// position is absolute i.e. it is always relative to the
    /// beginning of the buffer which is position 0.
    ///
    /// @param new_position the new position
    void seek(uint32_t new_position)
    {
        std::error_code error;
        seek(new_position, error);
        assert(!error);
    }

    /// Skips over a given number of bytes in the stream
    ///
    /// @param bytes_to_skip the bytes to skip
    /// @param error reference to the error code which will be set if the skip
    ///              failed
    void skip(uint32_t bytes_to_skip, std::error_code& error)
    {
        assert(!error);
        auto new_position = m_position + bytes_to_skip;
        seek(new_position, error);
    }

    /// Skips over a given number of bytes in the stream
    ///
    /// @param bytes_to_skip the bytes to skip
    void skip(uint32_t bytes_to_skip)
    {
        std::error_code error;
        skip(bytes_to_skip, error);
        assert(!error);
    }

protected:

    /// The size of the buffer in bytes
    uint32_t m_size;

    /// The current position
    uint32_t m_position = 0;
};
}
