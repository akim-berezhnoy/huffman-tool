#pragma once

#include "utility/codeword.h"
#include "utility/huffman_tree.h"
#include "wrappers/istream_wrapper.h"
#include "wrappers/ostream_wrapper.h"

#include <queue>

#include <array>
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
  std::array<size_t, UCHAR_STATES> frequencies{};
  std::array<codeword, UCHAR_STATES> codewords{};
  uchar letter = 0;

  encoder() = default;

  void prepare_encoding(istream& is);

  void encode(istream& is, ostream& os);

  void build_huffman_tree(node* current, vector<bool>& code);

public:
  friend void encode(istream& is, ostream& os);

  encoder& operator=(const encoder&) = delete;
};

void encode(istream& is, ostream& os);

} // namespace huffman
