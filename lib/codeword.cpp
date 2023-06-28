#include "codeword.h"

#include "constants.h"
#include "ostream_wrapper.h"

#include <vector>

namespace huffman {

  codeword::codeword() = default;

  codeword::codeword(const vector<bool>& vec) : code(vec) {}

  void operator<<(ostream_wrapper& out, codeword& cw) {
    for (bool bit : cw.code) {
      out.write(bit, 1);
    }
  }

  void operator<<(codeword& out, bool bit) {
    out.code.push_back(bit);
  }

  void operator<<(codeword& out, uchar letter) {
    for (size_t i = 1; i <= UCHAR_BIT; ++i) {
      out.code.push_back((letter >> (UCHAR_BIT - i)) & 1);
    }
  }

}
