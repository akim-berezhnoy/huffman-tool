#pragma once

#include "cctype"
#include "coding/decoder.h"
#include "coding/encoder.h"
#include "guard.h"
#include "ss.h"

using namespace huffman;

static void test_encode_decode(ss& initial) {
  {
    guard g(initial);
    ss encoded;
    encode(initial, encoded);
    initial.clear();
    decode(encoded, initial);
  }
}
