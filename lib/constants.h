#pragma once

#include <istream>
#include <limits>
#include <ostream>
#include <utility>

namespace huffman {

using uchar = unsigned char;
using buffer = std::pair<uchar, uchar>;

constexpr size_t UCHAR_BIT = std::numeric_limits<uchar>::digits;
constexpr size_t UCHAR_STATES = 1 << UCHAR_BIT;
constexpr size_t STREAM_BUFFER_SIZE = 512;

} // namespace huffman
