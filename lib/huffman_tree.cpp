#include "huffman_tree.h"

#include "constants.h"

#include <memory>

namespace huffman {

using unode = std::unique_ptr<node>;

node::node() : _weight(0), _left_child(nullptr), _right_child(nullptr) {}

node::node(size_t weight, uchar value) : _weight(weight), _value(value) {}

node::node(unode&& left_child, unode&& right_child)
    : _weight(left_child->_weight + right_child->_weight),
      _left_child(left_child.release()),
      _right_child(right_child.release()) {}

bool node::is_leaf() const {
  return _left_child == nullptr && _right_child == nullptr;
}

bool node_comparer::operator()(const std::unique_ptr<node>& a, const std::unique_ptr<node>& b) {
  return a->_weight > b->_weight;
}

bool node_comparer::operator()(const node* a, const node* b) {
  return a->_weight > b->_weight;
}

} // namespace huffman
