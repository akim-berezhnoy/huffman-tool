#pragma once

#include <istream>
#include <utility>

namespace {
using uchar = unsigned char;
using buffer = std::pair<uchar, uchar>;
using input_iterator = std::istreambuf_iterator<char>;

constexpr input_iterator end = input_iterator();
constexpr size_t UCHAR_BIT = std::numeric_limits<uchar>::digits;
constexpr size_t UCHAR_STATES = 1 << UCHAR_BIT;
} // namespace
