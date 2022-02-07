// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <endian/big_endian.hpp>
#include <endian/stream_reader.hpp>
#include <endian/stream_writer.hpp>

int main()
{
    uint16_t a = 111, b = 222;
    uint16_t x, y;

    x = y = 0;
    std::vector<uint8_t> stream_buffer(sizeof(uint16_t) * 2);
    endian::stream_writer<endian::big_endian> writer(stream_buffer.data(),
                                                     stream_buffer.size());
    writer << a << b;

    endian::stream_reader<endian::big_endian> reader(stream_buffer.data(),
                                                     stream_buffer.size());
    reader >> x >> y;
    assert(x == a && y == b);

    return 0;
}
