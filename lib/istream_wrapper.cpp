#include "istream_wrapper.h"

#include "codeword.h"

#include <cassert>

using std::istream;
using std::pair;

namespace huffman {

bool istream_wrapper::reached_end_of_buffer() const {
  return STREAM_BUFFER_iterator == STREAM_BUFFER_occupied;
}

uchar istream_wrapper::next_char() {
  if (_is.eof() && reached_end_of_buffer()) {
    throw wrong_input_file();
  }
  auto ret = static_cast<uchar>(STREAM_BUFFER[STREAM_BUFFER_iterator++]);
  if (reached_end_of_buffer()) {
    _is.read(STREAM_BUFFER, STREAM_BUFFER_SIZE);
    STREAM_BUFFER_occupied = _is.gcount();
    STREAM_BUFFER_iterator = 0;
  }
  return ret;
}

uchar istream_wrapper::peek(size_t n) {
  assert(n <= UCHAR_BIT);
  if (_occupied < n) {
    _buffer.second = next_char();
    _buffer.first |= _buffer.second >> _occupied;
    _buffer.second <<= UCHAR_BIT - _occupied;
    _occupied += UCHAR_BIT;
  }
  return _buffer.first >> (UCHAR_BIT - n);
}

uchar istream_wrapper::read(size_t n) {
  uchar ret = peek(n);
  _buffer.first <<= n;
  if (_buffer.second) {
    _buffer.first |= _buffer.second >> (UCHAR_BIT - n);
    _buffer.second <<= n;
  }
  _occupied -= n;
  return ret;
}

uchar istream_wrapper::read() {
  return read(UCHAR_BIT);
}

codeword istream_wrapper::read_codeword(size_t length) {
  codeword c;
  while (length > UCHAR_BIT) {
    c << read();
    length -= UCHAR_BIT;
  }
  while (length-- > 0) {
    c << static_cast<bool>(read(1));
  }
  return c;
};

size_t istream_wrapper::read_number() {
  size_t ret = 0;
  for (size_t i = 4; i-- > 0;) {
    ret |= static_cast<size_t>(read()) << (i * UCHAR_BIT);
  }
  return ret;
}

bool istream_wrapper::exhausted() const noexcept {
  return (_occupied == 0) && reached_end_of_buffer();
}

} // namespace huffman
