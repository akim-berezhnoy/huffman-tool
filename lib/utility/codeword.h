#pragma once

#include "constants.h"
#include "wrappers/ostream_wrapper.h"

#include <vector>

using std::vector;

namespace huffman {

struct codeword {

  vector<bool> code;

  codeword();

  explicit codeword(const vector<bool>& vec);

  friend ostream_wrapper& operator<<(ostream_wrapper& out, codeword& cw);

  friend codeword& operator<<(codeword& out, bool bit);

  friend codeword& operator<<(codeword& out, uchar letter);
};

ostream_wrapper& operator<<(ostream_wrapper& out, codeword& cw);
codeword& operator<<(codeword& out, bool bit);
codeword& operator<<(codeword& out, uchar letter);

} // namespace huffman
