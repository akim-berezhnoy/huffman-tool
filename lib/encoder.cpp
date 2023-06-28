#include "encoder.h"

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
using std::make_unique;
using std::ostream;
using std::pair;
using std::unique_ptr;

namespace huffman {

using unode = std::unique_ptr<node>;

void encoder::prepare_encoding(istream& is) {
  auto iw = istream_wrapper(is);
  while (!iw.exhausted()) {
    ++file_length;
    ++frequencies[iw.read_letter()];
  }
  if (file_length == 0) {
    return;
  }
  std::priority_queue<unode, std::vector<unode>, node_comparer> nodes;
  do {
    if (frequencies[letter]) {
      nodes.push(make_unique<node>(frequencies[letter], letter));
    }
  } while (++letter != 0);
  if (nodes.size() == 1) {
    unode unique_leaf = const_cast<unode&&>(nodes.top());
    nodes.pop();
    codewords[unique_leaf->_value].code.push_back(false);
  } else {
    while (nodes.size() > 1) {
      unode first = const_cast<unode&&>(nodes.top());
      nodes.pop();
      unode second = const_cast<unode&&>(nodes.top());
      nodes.pop();
      nodes.push(make_unique<node>(std::move(first), std::move(second)));
    }
    unode root = const_cast<unode&&>(nodes.top());
    nodes.pop();
    vector<bool> init;
    build_huffman_tree(root, init);
  }
}

void encoder::encode(istream& is, ostream& os) {
  auto ow = ostream_wrapper(os);
  ow.write_number(file_length);
  if (file_length == 0) {
    return;
  }
  do {
    if (frequencies[letter]) {
      ow.write(letter);
    }
  } while (++letter != 0);
  ow.write('\0');
  do {
    if (frequencies[letter]) {
      ow.write(static_cast<uchar>(codewords[letter].code.size()));
    }
  } while (++letter != 0);
  do {
    if (frequencies[letter]) {
      ow << codewords[letter];
    }
  } while (++letter != 0);
  auto iw = istream_wrapper(is);
  while (!iw.exhausted()) {
    ow << codewords[iw.read_letter()];
  }
}

void encoder::build_huffman_tree(unode& current, vector<bool>& code) {
  if (current->is_leaf()) {
    codewords[current->_value] = codeword(code);
  } else {
    code.push_back(false);
    build_huffman_tree(current->_left_child, code);
    code.pop_back();
    code.push_back(true);
    build_huffman_tree(current->_right_child, code);
    code.pop_back();
  }
}

void encode(istream& is, ostream& os) {
  encoder e;
  e.prepare_encoding(is);
  is.clear();
  is.seekg(0);
  e.encode(is, os);
}

void encode(istream& is, ostream& os);

} // namespace huffman
