#include "encoding-decoding.h"
#include "gtest/gtest.h"
#include "ss.h"

static constexpr size_t HUGE_SIZE = static_cast<size_t>(1) << 26;

static ss h_initial(HUGE_SIZE);
static ss h_encoded;
static ss h_decoded;

TEST(performance_check, stream_encode_100MB) {
  encode(h_initial, h_encoded);
}

TEST(performance_check, stream_decode_100MB) {
  decode(h_encoded, h_decoded);
}

TEST(performance_check, stream_correctness_100MB) {
  ASSERT_EQ(h_initial, h_decoded);
}
