#pragma once

#include "utility/codeword.h"
#include "utility/constants.h"
#include "utility/huffman_tree.h"
#include "wrappers/istream_wrapper.h"
#include "wrappers/ostream_wrapper.h"

#include <istream>
#include <set>
#include <vector>

using std::array;
using std::set;
using std::vector;

namespace huffman {

class decoder {
  node* huffman_root = nullptr;
  std::vector<std::unique_ptr<node>> holder;

  size_t file_length{};
  array<size_t, UCHAR_STATES> codewords_lengths{};
  array<codeword, UCHAR_STATES> codewords{};
  set<uchar> used_letters{};

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
