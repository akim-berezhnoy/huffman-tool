#pragma once

#include "huffman_tree.h"

struct node_comparer {

  bool operator()(const node* a, const node* b) {
    return a->_weight > b->_weight;
  }
};
