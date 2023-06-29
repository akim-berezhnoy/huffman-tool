#pragma once

#include "constants.h"

#include <memory>

namespace huffman {

struct node {

  size_t _weight{};
  uchar _value{};
  node* _left_child = nullptr;
  node* _right_child = nullptr;

  node();

  node(size_t weight, uchar value);

  explicit node(uchar value) : node(0, value) {}

  node(node* left_child, node* right_child);

  inline bool is_leaf() const {
    return !(_left_child || _right_child);
  }
};

struct node_comparer {

  bool operator()(const std::unique_ptr<node>& a, const std::unique_ptr<node>& b);

  bool operator()(const node* a, const node* b);
};

} // namespace huffman
