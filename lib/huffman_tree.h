#pragma once

#include "constants.h"

struct node {

  size_t _weight{};
  node* _left_child{};
  node* _right_child{};

  node()
    : _weight(0)
    ,_left_child(nullptr)
    , _right_child(nullptr)
  {}

  explicit node(size_t weight)
    : _weight(weight)
  {}

  node(node* left_child, node* right_child)
    : _weight(left_child->_weight + right_child->_weight)
    , _left_child(left_child)
    , _right_child(right_child)
  {}

  bool is_leaf() {
    return _left_child == nullptr && _right_child == nullptr;
  }

};


struct leaf : node {
  uchar _value;

  explicit leaf(const uchar& value) : leaf(value, 0) {}

  leaf(uchar value, size_t weight)
      : node(weight)
        , _value(value)
  {}

  static void destroy_tree(node* current) noexcept {
    if (current == nullptr) return;
    if (current->is_leaf()) {
      delete static_cast<leaf*>(current);
    } else {
      destroy_tree(current->_left_child);
      destroy_tree(current->_right_child);
      delete current;
    }
  }

};
