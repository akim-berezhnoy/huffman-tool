#include "encoder.h"

#include "utility/codeword.h"
#include "utility/huffman_tree.h"
#include "wrappers/istream_wrapper.h"
#include "wrappers/ostream_wrapper.h"

#include <utility>
#include <vector>

using std::istream;
using std::make_unique;
using std::ostream;
using std::unique_ptr;

namespace huffman {

void encoder::prepare_encoding(istream& is) {
  auto iw = istream_wrapper(is);
  while (!iw.exhausted()) {
    ++file_length;
    ++frequencies[iw.read_letter()];
  }
  if (file_length == 0) {
    return;
  }
  node_comparer comp{};
  std::vector<std::unique_ptr<node>> nodes;
  std::vector<std::unique_ptr<node>> holder;
  do {
    if (frequencies[letter]) {
      nodes.push_back(std::make_unique<node>(frequencies[letter], letter));
    }
  } while (++letter != 0);
  std::make_heap(nodes.begin(), nodes.end(), comp);
  if (nodes.size() == 1) {
    codewords[nodes.back()->_value].code.push_back(false);
  } else {
    while (nodes.size() > 1) {
      std::pop_heap(nodes.begin(), nodes.end(), comp);
      holder.push_back(std::move(nodes.back()));
      nodes.pop_back();
      node* first = holder.back().get();

      std::pop_heap(nodes.begin(), nodes.end(), comp);
      holder.push_back(std::move(nodes.back()));
      nodes.pop_back();
      node* second = holder.back().get();

      nodes.push_back(std::make_unique<node>(first, second));
      std::push_heap(nodes.begin(), nodes.end(), comp);
    }
    node* root = nodes.back().get();
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

void encoder::build_huffman_tree(node* current, vector<bool>& code) {
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

} // namespace huffman
