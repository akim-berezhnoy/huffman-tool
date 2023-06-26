#pragma once

#include "codeword.h"
#include "constants.h"

#include <cctype>

class coder {
protected:
  size_t file_length{};
  size_t letter_info[UCHAR_STATES]{};
  codeword codewords[UCHAR_STATES]{};

  template <typename F>
  void forall_letters(F f) {
    uchar letter = 0;
    do {
      if (letter_info[letter]) {
        f(letter);
      }
    } while (++letter != 0);
  }
};
