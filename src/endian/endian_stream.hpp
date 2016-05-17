// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <algorithm>

#include <storage/storage.hpp>
#include "big_endian.hpp"
#include "little_endian.hpp"

namespace endian
{
    class endian_stream
    {
    public:
        /// Creates an endian stream on top of a pre-allocated buffer of the
        /// specified size
        /// @param buffer a pointer to the buffer
        /// @param size the size of the buffer in bytes
        endian_stream(uint8_t* buffer, uint32_t size) :
            m_buffer(buffer),
            m_size(size),
            m_position(0)
        {
            assert(m_buffer != nullptr);
            assert(m_size);
        }
        /// Creates an endian stream on top of a const storage that has
        /// a fixed size
        /// @param storage the const storage
        endian_stream(const storage::const_storage& storage) :
            m_buffer(storage.m_data),
            m_size(storage.m_size),
            m_position(0)
        {
            assert(m_buffer != nullptr);
            assert(m_size);
        }


        /// Gets the size of the underlying buffer
        /// @return the size of the buffer
        uint32_t size() const
        {
            return m_size;
        }

        /// Gets the current read/write position in the stream
        /// @return the current position
        uint32_t position() const
        {
            return m_position;
        }

        /// Changes the current read/write position in the stream
        /// @param new_position the new position
        void seek(uint32_t new_position)
        {
            assert(new_position <= m_size);
            m_position = new_position;
        }
    protected:
        /// Pointer to buffer
        const uint8_t* m_buffer;

        /// The size of the buffer
        uint32_t m_size;

        /// The current position
        uint32_t m_position;
    };
}
