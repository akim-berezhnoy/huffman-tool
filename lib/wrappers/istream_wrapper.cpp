#include "istream_wrapper.h"

#include "utility/codeword.h"

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
    _is.read(STREAM_BUFFER.data(), STREAM_BUFFER_SIZE);
    STREAM_BUFFER_occupied = _is.gcount();
    STREAM_BUFFER_iterator = 0;
  }
  return ret;
}

bool istream_wrapper::read_bit() {
  if (!_occupied) {
    _buffer = next_char();
    _occupied = UCHAR_BIT;
  }
  uchar ret = _buffer >> (UCHAR_BIT - 1);
  _buffer <<= 1;
  --_occupied;
  return ret;
}

uchar istream_wrapper::read_letter() {
  uchar next = next_char();
  uchar ret = _buffer | (next >> _occupied);
  _buffer = next << (UCHAR_BIT - _occupied);
  return ret;
}

codeword istream_wrapper::read_codeword(size_t length) {
  codeword c;
  while (length > UCHAR_BIT) {
    c << read_letter();
    length -= UCHAR_BIT;
  }
  while (length-- > 0) {
    c << read_bit();
  }
  return c;
};

size_t istream_wrapper::read_number() {
  size_t ret = 0;
  for (size_t i = sizeof(size_t); i-- > 0;) {
    ret |= static_cast<size_t>(read_letter()) << (i * UCHAR_BIT);
  }
  return ret;
}

bool istream_wrapper::exhausted() const noexcept {
  return (!_occupied) && reached_end_of_buffer();
}

} // namespace huffman
