#pragma once

#include "codeword.h"
#include "huffman_tree.h"
#include "istream_wrapper.h"
#include "node_comparer.h"
#include "ostream_wrapper.h"
#include "coder.h"
#include "signature_manager.h"

#include <queue>

#include <iostream>
#include <limits>
#include <utility>
#include <vector>

using std::istream;
using std::ostream;
using std::pair;

class encoder : coder {

  size_t* frequencies = letter_info;

  encoder() = default;

  void prepare_encoding(istream& is) {
    auto iw = istream_wrapper(is);
    while (!iw.exhausted()) {
      ++file_length;
      ++frequencies[iw.read()];
    }
    if (file_length == 0) {
      return;
    }
    std::priority_queue<node*, std::vector<node*>, node_comparer> nodes;
    for_all_letters([this, &nodes](uchar l){
      node* letter = new leaf(l, frequencies[l]);
      nodes.push(letter);
    });
    if (nodes.size() == 1) {
      codewords[static_cast<leaf*>(nodes.top())->_value].code.push_back(false);
    } else {
      while (nodes.size() > 1) {
        node* first = nodes.top();
        nodes.pop();
        node* second = nodes.top();
        nodes.pop();
        nodes.push(new node(first, second));
      }
      node* root = nodes.top();
      vector<bool> init;
      build_huffman_tree(root, init);
      leaf::destroy_tree(nodes.top());
    }
    nodes.pop();
  }

  void encode(istream& is, ostream& os) {
    auto ow = ostream_wrapper(os);
    signature_manager::add_signature(ow);
    if (file_length == 0) {
      return;
    }
    ow.write_number(file_length);
    for_all_letters([this, &ow](uchar l){
      ow.write(l);
    });
    ow.write('\0');
    for_all_letters([this, &ow](uchar l){
      ow.write(static_cast<uchar>(codewords[l].code.size()));
    });
    for_all_letters([this, &ow](uchar l){
      ow << codewords[l];
    });
    auto iw = istream_wrapper(is);
    while (!iw.exhausted()) {
      ow << codewords[iw.read()];
    }
  }

  void build_huffman_tree(node* current, vector<bool>& code) {
    if (current->is_leaf()) {
      codewords[static_cast<leaf*>(current)->_value] = codeword(code);
    } else {
      code.push_back(false);
      build_huffman_tree(current->_left_child, code);
      code.pop_back();
      code.push_back(true);
      build_huffman_tree(current->_right_child, code);
      code.pop_back();
    }
  }

public:

  static void apply_encode(istream& is, ostream& os) {
    encoder e;
    e.prepare_encoding(is);
    is.seekg(0);
    e.encode(is, os);
  }

  encoder& operator=(const encoder&) = delete;

};