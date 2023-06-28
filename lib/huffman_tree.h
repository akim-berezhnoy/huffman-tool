#pragma once

#include "constants.h"

#include <memory>

namespace huffman {

struct node {

  using unode = std::unique_ptr<node>;

  size_t _weight{};
  uchar _value{};
  unode _left_child{};
  unode _right_child{};

  node();

  node(size_t weight, uchar value);

  explicit node(uchar value) : node(0, value) {}

  node(unode&& left_child, unode&& right_child);

  bool is_leaf() const;
};

struct node_comparer {

  bool operator()(const std::unique_ptr<node>& a, const std::unique_ptr<node>& b);

  bool operator()(const node* a, const node* b);
};

} // namespace huffman
