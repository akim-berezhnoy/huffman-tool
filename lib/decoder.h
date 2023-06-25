#pragma once

#include "coder.h"
#include "codeword.h"
#include "constants.h"
#include "huffman_tree.h"
#include "istream_wrapper.h"
#include "ostream_wrapper.h"
#include "signature_manager.h"

#include <istream>
#include <vector>

using std::vector;

class decoder : coder {

  size_t* codewords_lengths = letter_info;

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
    for_all_letters([this, &iw](uchar l) { codewords_lengths[l] = iw.read(); });
    for_all_letters([this, &iw](uchar l) { codewords[l] = iw.read_codeword(codewords_lengths[l]); });
    build_huffman_tree();
    auto ow = ostream_wrapper(os);
    for (size_t i = 0; i < file_length; ++i) {
      uchar letter = parse_letter(&huffman_root, iw);
      ow.write(letter);
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
    for_all_letters([this](uchar l) { build_branch(codewords[l].code, l); });
  }

  void build_branch(vector<bool>& code, uchar letter) {
    node* current = &huffman_root;
    bool last_bit = code.back();
    code.pop_back();
    for (bool bit : code) {
      node*& bound = bit ? current->_right_child : current->_left_child;
      if (!bound) {
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
