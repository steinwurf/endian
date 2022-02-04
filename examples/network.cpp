// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <endian/network.hpp>
#include <endian/stream_reader.hpp>
#include <endian/stream_writer.hpp>

int main()
{
    uint16_t a = 111, b = 222;
    uint16_t x, y;

    auto buffer = new uint8_t[sizeof(uint16_t) * 2];

    // Usage: `put` and `get`
    endian::network::put(a, buffer);
    endian::network::put(b, buffer + sizeof(a));

    endian::network::get(x, buffer);
    y = endian::network::get<uint16_t>(buffer + sizeof(x));
    assert(x == a && y == b);

    // Usage: `stream_writer` and `stream_reader`
    x = y = 0;
    std::vector<uint8_t> stream_buffer(sizeof(uint16_t) * 2);
    endian::stream_writer<endian::network> writer(stream_buffer.data(),
                                                  stream_buffer.size());
    writer << a << b;

    endian::stream_reader<endian::network> reader(stream_buffer.data(),
                                                  stream_buffer.size());
    reader >> x >> y;
    assert(x == a && y == b);

    return 0;
}
