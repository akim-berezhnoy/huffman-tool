#pragma once

#include "codeword.h"
#include "huffman_tree.h"
#include "istream_wrapper.h"
#include "ostream_wrapper.h"

#include <queue>

#include <iostream>
#include <limits>
#include <utility>
#include <vector>

using std::istream;
using std::ostream;
using std::pair;

namespace huffman {

class encoder {

  size_t file_length{};
  size_t frequencies[UCHAR_STATES]{};
  codeword codewords[UCHAR_STATES]{};

  uchar letter = 0;

  encoder() = default;

  void prepare_encoding(istream& is);

  void encode(istream& is, ostream& os);

  void build_huffman_tree(std::unique_ptr<node>& current, vector<bool>& code);

public:
  friend void encode(istream& is, ostream& os);

  encoder& operator=(const encoder&) = delete;
};

void encode(istream& is, ostream& os);

} // namespace huffman
