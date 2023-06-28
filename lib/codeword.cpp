#include "codeword.h"

#include "constants.h"
#include "ostream_wrapper.h"

#include <vector>

// vector<bool> -> bitset
// unique ptr

namespace huffman {

codeword::codeword() = default;

codeword::codeword(const vector<bool>& vec) : code(vec) {}

ostream_wrapper& operator<<(ostream_wrapper& out, codeword& cw) {
  for (bool bit : cw.code) {
    out.write_bit(bit);
  }
  return out;
}

codeword& operator<<(codeword& out, bool bit) {
  out.code.push_back(bit);
  return out;
}

codeword& operator<<(codeword& out, uchar letter) {
  for (size_t i = 1; i <= UCHAR_BIT; ++i) {
    out.code.push_back((letter >> (UCHAR_BIT - i)) & 1);
  }
  return out;
}

} // namespace huffman
