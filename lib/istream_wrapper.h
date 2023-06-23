#pragma once

#include "codeword.h"

using std::istream;
using std::pair;

struct istream_wrapper {

  input_iterator _it;

  buffer _buffer{};

  size_t _occupied{};

  explicit istream_wrapper(istream& is) : _it(is) {}

  struct wrong_input_file : std::exception {
    const char* what() const noexcept override {
      return "That file was not encoded by this utility. Please, check it.";
    }
  };

  uchar peek(size_t n) {
    assert(n <= CHAR_BIT);
    if (_occupied < n) {
      _buffer.second = next();
      _buffer.first |= _buffer.second >> _occupied;
      _buffer.second <<= UCHAR_BIT - _occupied;
      _occupied += UCHAR_BIT;
    }
    return _buffer.first >> (UCHAR_BIT - n);
  }

  uchar read(size_t n) {
    uchar ret = peek(n);
    _buffer.first <<= n;
    if (_buffer.second) {
      _buffer.first |= _buffer.second >> (UCHAR_BIT - n);
      _buffer.second <<= n;
    }
    _occupied -= n;
    return ret;
  }

  uchar read() {
    return read(UCHAR_BIT);
  }

  codeword read_codeword(size_t length) {
    codeword c;
    while (length > UCHAR_BIT) {
      c << read();
      length -= UCHAR_BIT;
    }
    while (length --> 0) {
      c << static_cast<bool>(read(1));
    }
    return c;
  };

  size_t read_number() {
    size_t ret = 0;
    for (size_t i = 4; i --> 0;) {
      ret |= static_cast<size_t>(read()) << (i * UCHAR_BIT);
    }
    return ret;
  }

  bool exhausted() const noexcept {
    return (_occupied == 0) && (_it == end);
  }

private:

  uchar next() {
    if (exhausted()) {
      throw wrong_input_file();
    }
    return static_cast<uchar>(*_it++);
  }

};
