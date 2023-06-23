#include "gtest/gtest.h"

#include "ss.h"
#include "encoding-decoding.h"

static constexpr size_t HUGE_SIZE = static_cast<size_t>(1) << 31;

ss h_initial(HUGE_SIZE);
ss h_encoded;
ss h_decoded;

TEST(performance_check, stream_encode_2GB) {
  encode(h_initial, h_encoded);
}

TEST(performance_check, stream_decode_2GB) {
  decode(h_encoded, h_decoded);
}

TEST(performance_check, stream_correctness_check_2GB) {
  ASSERT_EQ(h_initial, h_decoded);
}
