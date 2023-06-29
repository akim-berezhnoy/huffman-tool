#pragma once

#include "utility/constants.h"

#include <array>
#include <cassert>
#include <cctype>
#include <iostream>
#include <ostream>

using std::ostream;

namespace huffman {

class ostream_wrapper {

  uchar _buffer{};

  size_t _occupied{};

  ostream& _os;

  // Added buffered reading
  std::array<char, STREAM_BUFFER_SIZE> STREAM_BUFFER{};
  size_t STREAM_BUFFER_IT = 0;

  void write_next(uchar ch);

public:
  explicit ostream_wrapper(ostream& os) : _os(os) {}

  void write(uchar ch, size_t n);

  void write_bit(bool bit);

  void write(uchar ch);

  void write_number(size_t number);

  void flush();

  ~ostream_wrapper();
};

} // namespace huffman
