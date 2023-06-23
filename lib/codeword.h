#pragma once

#include "constants.h"
#include "ostream_wrapper.h"

using std::vector;

struct codeword {

  vector<bool> code;

  codeword() = default;

  explicit codeword(const vector<bool>& vec) : code(vec) {}

  friend void operator<<(ostream_wrapper& out, codeword& cw) {
    for (bool bit : cw.code) {
      out.write(bit, 1);
    }
  }

  friend void operator<<(codeword& out, bool bit) {
    out.code.push_back(bit);
  }

  friend void operator<<(codeword& out, uchar letter) {
    for (size_t i = 1; i <= UCHAR_BIT; ++i) {
      out.code.push_back((letter >> (UCHAR_BIT - i)) & 1);
    }
  }

};
