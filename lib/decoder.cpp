#include "decoder.h"

#include "constants.h"
#include "huffman_tree.h"
#include "istream_wrapper.h"
#include "ostream_wrapper.h"

#include <istream>
#include <vector>

using std::vector;

namespace huffman {

decoder::decoder() = default;

void decoder::decode(istream& is, ostream& os) {
  auto iw = istream_wrapper(is);
  file_length = iw.read_number();
  if (file_length == 0) {
    return;
  }
  uchar ch = iw.read_letter();
  do {
    used_letters.insert(ch);
    ch = iw.read_letter();
  } while (ch != '\0');
  for (uchar c : used_letters) {
    codewords_lengths[c] = iw.read_letter();
  }
  size_t unique_letters_cnt = 0;
  uchar unique_letter = 0;
  for (uchar c : used_letters) {
    codewords[c] = iw.read_codeword(codewords_lengths[c]);
    ++unique_letters_cnt;
    unique_letter = c;
  }
  auto ow = ostream_wrapper(os);
  if (unique_letters_cnt == 1) {
    for (size_t i = 0; i < file_length; ++i) {
      ow.write(unique_letter);
    }
  } else {
    build_huffman_tree();
    for (size_t i = 0; i < file_length; ++i) {
      ow.write(parse_letter(huffman_root.get(), iw));
    }
  }
}

uchar decoder::parse_letter(node* current, istream_wrapper& iw) {
  while (true) {
    if (!current->is_leaf()) {
      bool bit = iw.read_bit();
      if (bit) {
        current = current->_right_child.get();
      } else {
        current = current->_left_child.get();
      }
    } else {
      return current->_value;
    }
  }
}

void decoder::build_huffman_tree() {
  for (uchar c : used_letters) {
    build_branch(codewords[c].code, c);
  }
}

void decoder::build_branch(vector<bool>& code, uchar letter) {
  node* current = huffman_root.get();
  bool last_bit = code.back();
  code.pop_back();
  for (bool bit : code) {
    unode& bound = bit ? current->_right_child : current->_left_child;
    if (!bound.get()) {
      bound = std::make_unique<node>();
    }
    current = bound.get();
  }
  if (last_bit) {
    current->_right_child = std::make_unique<node>(letter);
  } else {
    current->_left_child = std::make_unique<node>(letter);
  }
}

void decode(istream& is, ostream& os) {
  decoder d;
  d.decode(is, os);
}

void decode(istream& is, ostream& os);

} // namespace huffman
