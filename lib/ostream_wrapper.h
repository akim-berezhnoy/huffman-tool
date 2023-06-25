#pragma once

#include <cctype>
#include <ostream>

using std::ostream;

struct ostream_wrapper {

  using uchar = unsigned char;
  using output_iterator = std::ostreambuf_iterator<char>;

  static constexpr unsigned int UCHAR_BIT = std::numeric_limits<uchar>::digits;

  output_iterator _it;

  uchar _buffer{};

  size_t _occupied{};

  ostream* _os;

  explicit ostream_wrapper(ostream& os) : _it(os), _os(&os) {}

  void write(uchar ch, size_t n) {
    assert(n <= UCHAR_BIT);
    assert(n > 0);
    if (n == UCHAR_BIT) {
      *_it++ = static_cast<char>(ch);
    } else if (n + _occupied > UCHAR_BIT) {
      size_t shift = abs(static_cast<int32_t>(n) - static_cast<int32_t>(_occupied));
      _buffer |= ch >> shift;
      *_it++ = static_cast<char>(_buffer);
      _buffer = ch << (UCHAR_BIT - shift);
      _occupied = shift;
    } else {
      _buffer |= ch << (UCHAR_BIT - n - _occupied);
      _occupied += n;
      if (_occupied == UCHAR_BIT) {
        *_it++ = static_cast<char>(_buffer);
        _buffer = 0;
        _occupied = 0;
      }
    }
  }

  void write(uchar ch) {
    write(ch, UCHAR_BIT);
  }

  void write_number(size_t number) {
    for (size_t i = 4; i-- > 0;) {
      write(number >> (i * UCHAR_BIT), UCHAR_BIT);
    }
  }

  void flush() {
    if (_occupied) {
      *_it++ = static_cast<char>(_buffer);
    }
    _os->flush();
  }

  ~ostream_wrapper() {
    flush();
  }
};
