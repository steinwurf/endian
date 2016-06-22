// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>

namespace endian
{
    /// @brief Base-class for the endian stream reader and writer.
    class stream
    {
    public:

        /// Creates an endian stream on top of a pre-allocated buffer of the
        /// specified size.
        ///
        /// @param buffer a pointer to the buffer
        /// @param size the size of the buffer in bytes
        stream(uint32_t size) :
            m_size(size),
            m_position(0)
        {
            assert(m_size);
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

        /// The remaining bytes in the stream
        ///
        /// @return the remaining bytes.
        uint32_t remaining() const
        {
            return m_size - m_position;
        }

        /// Changes the current read/write position in the stream. The
        /// position is absolute i.e. it is always relative to the
        /// beginning of the buffer which is position 0.
        ///
        /// @param new_position the new position
        void seek(uint32_t new_position)
        {
            assert(new_position <= m_size);
            m_position = new_position;
        }

    protected:

        /// The size of the buffer in bytes
        uint32_t m_size;

        /// The current position
        uint32_t m_position;
    };
}
