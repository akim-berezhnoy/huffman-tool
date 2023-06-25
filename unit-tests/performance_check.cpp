#include "encoding-decoding.h"
#include "gtest/gtest.h"
#include "ss.h"

static constexpr size_t HUGE_SIZE = static_cast<size_t>(1) << 30;

ss h_initial(HUGE_SIZE);
ss h_encoded;
ss h_decoded;

TEST(performance_check, stream_encode_1GB) {
  encode(h_initial, h_encoded);
}

TEST(performance_check, stream_decode_1GB) {
  decode(h_encoded, h_decoded);
}

TEST(performance_check, stream_correctness_check_1GB) {
  ASSERT_EQ(h_initial, h_decoded);
}
