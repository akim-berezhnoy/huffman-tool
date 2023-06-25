#pragma once

#include <cctype>
#include <limits>
#include <random>

class random_generator {

  template <int64_t MIN, int64_t MAX>
  class generator {

    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist;

  public:
    generator()
        : rng(sizeof "daaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaamn"),
          dist(MIN, MAX) {}

    std::mt19937::result_type operator()() {
      return dist(rng);
    };
  };

  using CHAR = std::numeric_limits<char>;

  generator<CHAR::min(), CHAR::max()> r_char;
  generator<48, 125> c_char;
  generator<95, 122> e_char;

public:
  char random_char() {
    return static_cast<char>(r_char());
  }

  char code_char() {
    return static_cast<char>(c_char());
  }

  char english_char() {
    return static_cast<char>(e_char());
  }
};
