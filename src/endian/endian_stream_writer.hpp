
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
    /// The idea behind the endian_stream_writer is to provide a stream-like
    /// interface for accessing a fixed-size buffer.
    /// All complexity regarding endianness is encapsulated.
    template<class EndianType>
    class endian_stream_writer
    {
    public:
        /// Creates an endian stream on top of a pre-allocated buffer of the
        /// specified size
        /// @param buffer a pointer to the buffer
        /// @param size the size of the buffer in bytes
        endian_stream_writer(uint8_t* buffer, uint32_t size) :
            m_buffer(buffer), m_size(size), m_position(0)
        {
            assert(m_buffer != nullptr);
            assert(m_size);
        }

        /// Creates an endian stream on top of a mutable storage that has
        /// a fixed size
        /// @param storage the mutable storage
        endian_stream_writer(const storage::mutable_storage& storage) :
            m_buffer(storage.m_data), m_size(storage.m_size), m_position(0)
        {
            assert(m_buffer != nullptr);
            assert(m_size);
        }

        /// Writes a value of the size of ValueType to the stream
        /// @param value the value to write
        template<class ValueType>
        void write(ValueType value)
        {
            // Make sure there is enough space in the underlying buffer
            assert(m_size >= m_position + sizeof(ValueType));
            // Write the value at the current position
            EndianType::template put<ValueType>(value, &m_buffer[m_position]);
            // Advance the current position
            m_position += sizeof(ValueType);
        }

        /// Writes the contents of a sak::storage container to the stream.
        /// Note that this function is provided only for convenience and
        /// it does not perform any endian conversions. Furthermore, the length
        /// of the container is not written to the stream.
        /// @param storage the storage to write
        void write(const storage::mutable_storage& storage)
        {
            write(storage::const_storage(storage));
        }

        /// Writes the contents of a sak::storage container to the stream.
        /// Note that this function is provided only for convenience and
        /// it does not perform any endian conversions. Furthermore, the length
        /// of the container is not written to the stream.
        /// @param storage the storage to write
        void write(const storage::const_storage& storage)
        {
            // Make sure there is enough space in the underlying buffer
            assert(m_size >= m_position + storage.m_size);
            // Copy the data to the buffer
            std::copy_n(storage.m_data, storage.m_size, &m_buffer[m_position]);
            // Advance the current position
            m_position += storage.m_size;
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

    private:

        /// Pointer to the buffer
        uint8_t* m_buffer;

        /// The size of the buffer
        uint32_t m_size;

        /// The current position
        uint32_t m_position;
    };
}
