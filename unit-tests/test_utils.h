#pragma once

#include "cctype"
#include "encoding-decoding.h"
#include "guard.h"
#include "ss.h"

static void test_encode_decode(ss& initial) {
  {
    guard g(initial);
    ss encoded;
    encode(initial, encoded);
    initial.clear();
    decode(encoded, initial);
  }
}
