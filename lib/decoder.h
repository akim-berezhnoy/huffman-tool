#pragma once

#include "codeword.h"
#include "constants.h"
#include "huffman_tree.h"
#include "istream_wrapper.h"
#include "ostream_wrapper.h"

#include <istream>
#include <vector>

using std::vector;

namespace huffman {

class decoder {

  using unode = std::unique_ptr<node>;

  size_t file_length{};
  size_t codewords_lengths[UCHAR_STATES]{};
  codeword codewords[UCHAR_STATES]{};

  uchar letter = 0;

  unode huffman_root = std::make_unique<node>();

  decoder();

  void decode(istream&, ostream&);

  uchar parse_letter(node*, istream_wrapper&);

  void build_huffman_tree();

  void build_branch(vector<bool>&, uchar);

public:
  friend void decode(istream&, ostream&);

  decoder& operator=(const decoder&) = delete;
};

void decode(istream&, ostream&);

} // namespace huffman
