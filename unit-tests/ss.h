#pragma once

#include "random_generator.h"

#include <gtest/gtest.h>

#include <iostream>

static random_generator rnd;

class ss {
public:
  using stream = std::stringstream;

  stream ios;

public:
  enum class mode {
    random,
    english,
    code,
  };

  ss() = default;

  operator stream&() {
    return ios;
  }

  ss(size_t n) : ss(n, mode::random) {}

  ss(size_t n, mode m) {
    while (n-- > 0) {
      switch (m) {
      case mode::random:
        ios << rnd.random_char();
        break;
      case mode::english:
        ios << rnd.english_char();
        break;
      case mode::code:
        ios << rnd.code_char();
        break;
      }
    }
  }

  ss(const ss& other) {
    ios << other.ios.str();
  }

  ss& operator=(const ss& other) {
    ios << other.ios.str();
    return *this;
  }

  void clear() {
    ios.str("");
    ios.seekg(0);
    ios.clear();
  }

  friend bool operator==(const ss& a, const ss& b) {
    return a.ios.str() == b.ios.str();
  }

  friend std::ostream& operator<<(std::ostream& os, const ss& ss) {
    os << ss.ios.str();
    return os;
  }

  friend std::ostream& operator<<(std::ostream& os, char s) {
    os << s;
    return os;
  }
};
