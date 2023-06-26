#pragma once

#include "codeword.h"
#include "constants.h"
#include "huffman_tree.h"
#include "istream_wrapper.h"
#include "ostream_wrapper.h"
#include "signature_manager.h"

#include <istream>
#include <vector>

using std::vector;

class decoder {

  size_t file_length{};
  size_t codewords_lengths[UCHAR_STATES]{};
  codeword codewords[UCHAR_STATES]{};

  uchar letter = 0;

  node huffman_root{};

  decoder() = default;

  void decode(istream& is, ostream& os) {
    auto iw = istream_wrapper(is);
    signature_manager::check_signature(iw);
    if (iw.exhausted()) {
      return;
    }
    file_length = iw.read_number();
    uchar ch = iw.read();
    do {
      codewords_lengths[ch] = true;
      ch = iw.read();
    } while (ch != '\0');
    do {
      if (codewords_lengths[letter]) {
        codewords_lengths[letter] = iw.read();
      }
    } while (++letter != 0);
    size_t unique_letters_cnt = 0;
    uchar unique_letter = 0;
    do {
      if (codewords_lengths[letter]) {
        codewords[letter] = iw.read_codeword(codewords_lengths[letter]);
        ++unique_letters_cnt;
        unique_letter = letter;
      }
    } while (++letter != 0);
    auto ow = ostream_wrapper(os);
    if (unique_letters_cnt == 1) {
      for (size_t i = 0; i < file_length; ++i) {
        ow.write(unique_letter);
      }
    } else {
      build_huffman_tree();
      for (size_t i = 0; i < file_length; ++i) {
        uchar letter = parse_letter(&huffman_root, iw);
        ow.write(letter);
      }
      leaf::destroy_tree(huffman_root._left_child);
      leaf::destroy_tree(huffman_root._right_child);
    }
  }

  uchar parse_letter(node* current, istream_wrapper& iw) {
    if (current->is_leaf()) {
      return static_cast<leaf*>(current)->_value;
    } else {
      if (current->is_leaf()) {
        throw istream_wrapper::wrong_input_file();
      }
      bool bit = iw.read(1);
      if (bit) {
        return parse_letter(current->_right_child, iw);
      } else {
        return parse_letter(current->_left_child, iw);
      }
    }
  }

  void build_huffman_tree() {
    do {
      if (codewords_lengths[letter]) {
        build_branch(codewords[letter].code, letter);
      }
    } while (++letter != 0);
  }

  void build_branch(vector<bool>& code, uchar letter) {
    node* current = &huffman_root;
    bool last_bit = code.back();
    code.pop_back();
    for (bool bit : code) {
      node*& bound = bit ? current->_right_child : current->_left_child;
      if (bound == nullptr) {
        bound = new node();
      }
      current = bound;
    }
    if (last_bit) {
      current->_right_child = new leaf(letter);
    } else {
      current->_left_child = new leaf(letter);
    }
  }

public:
  static void apply_decode(istream& is, ostream& os) {
    decoder d;
    d.decode(is, os);
  }

  decoder& operator=(const decoder&) = delete;
};
