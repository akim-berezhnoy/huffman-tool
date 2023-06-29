#include "ostream_wrapper.h"

#include "utility/constants.h"

#include <cassert>
#include <cctype>
#include <iostream>
#include <ostream>

using std::ostream;

namespace huffman {

void ostream_wrapper::write_next(uchar ch) {
  STREAM_BUFFER[STREAM_BUFFER_IT++] = static_cast<char>(ch);
  if (STREAM_BUFFER_IT == STREAM_BUFFER_SIZE) {
    _os.write(STREAM_BUFFER.data(), STREAM_BUFFER_IT);
    STREAM_BUFFER_IT = 0;
  }
}

void ostream_wrapper::write(uchar ch, size_t n) {
  assert(n <= UCHAR_BIT);
  assert(n > 0);
  if (n == UCHAR_BIT) {
    write(ch);
  } else if (n + _occupied > UCHAR_BIT) {
    size_t shift = (n + _occupied) - UCHAR_BIT;
    _buffer |= ch >> shift;
    write_next(_buffer);
    _buffer = ch << (UCHAR_BIT - shift);
    _occupied = shift;
  } else {
    _buffer |= ch << (UCHAR_BIT - n - _occupied);
    _occupied += n;
    if (_occupied == UCHAR_BIT) {
      write_next(_buffer);
      _buffer = 0;
      _occupied = 0;
    }
  }
}

void ostream_wrapper::write_bit(bool bit) {
  _buffer |= bit << (UCHAR_BIT - _occupied - 1);
  if (++_occupied == UCHAR_BIT) {
    write_next(_buffer);
    _buffer = 0;
    _occupied = 0;
  }
}

void ostream_wrapper::write(uchar ch) {
  write_next(ch);
}

void ostream_wrapper::write_number(size_t number) {
  for (size_t i = sizeof(size_t); i-- > 0;) {
    write(number >> (i * UCHAR_BIT));
  }
}

void ostream_wrapper::flush() {
  if (_occupied) {
    write_next(_buffer);
  }
  if (STREAM_BUFFER_IT != 0) {
    _os.write(STREAM_BUFFER.data(), STREAM_BUFFER_IT);
  }
  _os.flush();
}

ostream_wrapper::~ostream_wrapper() {
  flush();
}

} // namespace huffman
