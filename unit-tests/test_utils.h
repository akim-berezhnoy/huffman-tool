#pragma once

#include "cctype"
#include "guard.h"
#include "ss.h"
#include "encoding-decoding.h"

constexpr size_t iterations = 333;

template <typename F>
static void test_multiple_times(F f) {
  for (size_t i = iterations; i > 0; --i)
    f();
}

static void test_encode_decode(ss& initial) {
  {
    guard g(initial);
    ss encoded;
    encode(initial, encoded);
    initial.clear();
    decode(encoded, initial);
  }
}
