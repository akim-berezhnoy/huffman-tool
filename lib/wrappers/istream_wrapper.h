#pragma once

#include "utility/codeword.h"

#include <array>
#include <cassert>

using std::istream;
using std::pair;

namespace huffman {

class istream_wrapper {

  istream& _is;
  uchar _buffer{};
  size_t _occupied{};

  // Added buffered reading
  std::array<char, STREAM_BUFFER_SIZE> STREAM_BUFFER{};
  size_t STREAM_BUFFER_occupied = 0;
  size_t STREAM_BUFFER_iterator = 0;

  bool reached_end_of_buffer() const;

  uchar next_char();

public:
  explicit istream_wrapper(istream& is) : _is(is) {
    _is.read(STREAM_BUFFER.data(), STREAM_BUFFER_SIZE);
    STREAM_BUFFER_occupied = _is.gcount();
  }

  struct wrong_input_file : std::exception {
    const char* what() const noexcept override {
      return "That file was not encoded by this utility. Please, check it.";
    }
  };

  bool read_bit();

  uchar read_letter();

  codeword read_codeword(size_t length);

  size_t read_number();

  bool exhausted() const noexcept;
};

} // namespace huffman
