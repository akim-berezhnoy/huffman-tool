#include "decoder.h"
#include "encoder.h"
#include "ss.h"
#include "test_utils.h"

#include <gtest/gtest.h>

class stream_compression_correctness : public testing::Test {};

using mode = ss::mode;

using std::vector;

using CHAR = std::numeric_limits<char>;

static const vector<size_t> stream_sizes = {1, 2, 3, 10, 30, 50, 100, 1000, 10000, 100000};

using mode = ss::mode;

using namespace huffman;

static constexpr size_t iterations = 100;

TEST(stream_compression_correctness, empty_file) {
  ss initial;
  test_encode_decode(initial);
}

TEST(stream_compression_correctness, file_with_only_one_type_of_letter) {
  for (char c = CHAR::min(); c < CHAR::max(); ++c) {
    ss initial;
    for (size_t i = 0; i < abs(c); ++i) {
      initial << c;
    }
    test_encode_decode(initial);
  }
}

TEST(stream_compression_correctness, english_charset) {
  for (size_t size : stream_sizes) {
    for (size_t i = iterations; i > 0; --i) {
      ss initial(size, mode::english);
      test_encode_decode(initial);
    }
  }
}

TEST(stream_compression_correctness, cpp_charset) {
  for (size_t size : stream_sizes) {
    for (size_t i = iterations; i > 0; --i) {
      ss initial(size, mode::code);
      test_encode_decode(initial);
    }
  }
}

TEST(stream_compression_correctness, random_charset) {
  for (size_t size : stream_sizes) {
    for (size_t i = iterations; i > 0; --i) {
      ss initial(size, mode::random);
      test_encode_decode(initial);
    }
  }
}
