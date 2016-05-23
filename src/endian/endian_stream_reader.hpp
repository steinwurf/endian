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
    /// The idea behind the endian_stream_reader is to provide a stream-like
    /// interface for accessing a fixed-size buffer.
    /// All complexity regarding endianness is encapsulated.
    template<class EndianType>
    class endian_stream_reader : public endian_stream
    {
    public:

        /// Creates an endian stream on top of a pre-allocated buffer of the
        /// specified size.
        ///
        /// @param buffer a pointer to the buffer
        /// @param size the size of the buffer in bytes
        endian_stream_reader(const uint8_t* buffer, uint32_t size) :
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
        endian_stream_reader(const storage::const_storage& storage) :
            endian_stream(storage.m_size),
            m_buffer(storage.m_data)
        {
            assert(m_buffer != nullptr);
            assert(m_size > 0);
        }

        /// Reads from the stream and moves the read position.
        ///
        /// @param value reference to the value to be read
        template<class ValueType>
        void read(ValueType& value)
        {
            // Make sure there is enough data to read in the underlying buffer
            assert(m_size >= m_position + sizeof(ValueType));

            // Read the value at the current position
            value = EndianType::template get<ValueType>(m_buffer + m_position);

            // Advance the current position
            m_position += sizeof(ValueType);
        }

        /// Reads raw bytes from the stream to fill a buffer represented by
        /// a mutable storage object.
        ///
        /// Note, that this function is provided only for convenience and
        /// it does not perform any endian conversions.
        ///
        /// @param storage the storage to be filled
        void read(const storage::mutable_storage& storage)
        {
            // Make sure there is enough data to read in the underlying buffer
            assert(m_size >= m_position + storage.m_size);

            // Copy the data from the buffer to the storage
            std::copy_n(m_buffer + m_position, storage.m_size, storage.m_data);

            // Advance the current position
            m_position += storage.m_size;
        }

    private:

        /// Pointer to buffer
        const uint8_t* m_buffer;
    };
}
