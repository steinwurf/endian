// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <cassert>
#include <algorithm>

#include "endian_stream.hpp"

namespace endian
{
    /// The idea behind the endian_stream_writer is to provide a stream-like
    /// interface for accessing a fixed-size buffer.
    /// All complexity regarding endianness is encapsulated.
    template<class EndianType>
    class endian_stream_writer : public endian_stream
    {
    public:

        /// Creates an endian stream on top of a pre-allocated buffer of the
        /// specified size.
        ///
        /// @param buffer a pointer to the buffer
        /// @param size the size of the buffer in bytes
        endian_stream_writer(uint8_t* buffer, uint32_t size) :
            endian_stream(size),
            m_buffer(buffer)
        {
            assert(m_buffer != nullptr);
            assert(m_size > 0);
        }

        /// Creates an endian stream on top of a const storage that has
        /// a fixed size.
        ///
        /// @param storage the const storage
        endian_stream_writer(const storage::mutable_storage& storage) :
            endian_stream(storage.m_size),
            m_buffer(storage.m_data)
        {
            assert(m_buffer != nullptr);
            assert(m_size > 0);
        }

        /// Writes a value of ValueType type and size to the stream.
        ///
        /// @param value the value to write.
        template<class ValueType>
        void write(ValueType value)
        {
            // Make sure there is enough space in the underlying buffer
            assert(m_size >= m_position + sizeof(ValueType));

            // Write the value at the current position
            EndianType::template put<ValueType>(value, m_buffer + m_position);

            // Advance the current position
            m_position += sizeof(ValueType);
        }

        /// @copydoc write(const storage::const_storage&)
        void write(const storage::mutable_storage& storage)
        {
            write(storage::const_storage(storage));
        }

        /// Writes the raw bytes represented by the storage::const_storage
        /// object to the stream.
        ///
        /// Note, that this function is provided only for convenience and
        /// it does not perform any endian conversions.
        ///
        /// @param storage the storage object which should be written to
        ///        the stream.
        void write(const storage::const_storage& storage)
        {
            // Make sure there is enough space in the underlying buffer
            assert(m_size >= m_position + storage.m_size);

            // Copy the data to the buffer
            std::copy_n(storage.m_data, storage.m_size, m_buffer + m_position);

            // Advance the current position
            m_position += storage.m_size;
        }

    private:

        /// Pointer to buffer
        uint8_t* m_buffer;
    };
}