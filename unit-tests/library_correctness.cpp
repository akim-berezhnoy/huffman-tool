#include "codeword.h"
#include "gtest/gtest.h"
#include "istream_wrapper.h"
#include "ostream_wrapper.h"
#include "random_generator.h"
#include "ss.h"
#include "test_utils.h"

using namespace huffman;

class library_correctness : public testing::Test {};

static constexpr size_t iterations = 100;

static void pass_bits_to_ostream(ostream_wrapper& ow, uchar ch, size_t bits) {
  size_t done = 0;
  while (done != UCHAR_BIT) {
    ow.write(ch >> (UCHAR_BIT - done - bits), bits);
    done += bits;
  }
}

static uchar pass_bits_from_istream(istream_wrapper& iw, size_t bits) {
  size_t done = 0;
  uchar ch = 0;
  while (done != UCHAR_BIT) {
    ch |= iw.read(bits) << (UCHAR_BIT - done - bits);
    done += bits;
  }
  return ch;
}

TEST(library_correctness, istream_wrapper) {
  size_t bits = 1;
  for (size_t i = iterations; i > 0; --i) {
    for (size_t letters = 1; letters < 300; ++letters) {
      ss in(letters, ss::mode::english);
      istream_wrapper iw(in);
      ss out;
      for (size_t i = 0; i < in.ios.str().size(); ++i) {
        out << pass_bits_from_istream(iw, bits);
      }
      ASSERT_EQ(in, out);
      in.clear();
    }
  }
}

TEST(library_correctness, ostream_wrapper) {
  size_t bits = 1;
  for (size_t i = iterations; i > 0; --i) {
    for (size_t letters = 1; letters < 2; ++letters) {
      ss in(letters);
      ss out;
      ostream_wrapper ow(out);
      for (uchar c : in.ios.str()) {
        pass_bits_to_ostream(ow, c, bits);
        bits = bits * 2 ? bits < 8 : 1;
      }
      ow.flush();
      ASSERT_EQ(in, out);
    }
  }

  ss in;
  ss out;
  ostream_wrapper ow(out);
  ow.write(0b00001010, 4);
  ow.write(0b00001001, 6);
  ow.write(0b00011001, 7);
  ow.write(0b00000011, 2);
  ow.write(0b01001111, 7);
  ow.write(0b00100110, 7);
  ow.write(0b01001101, 7);
  ow.flush();
  in << static_cast<uchar>(0b10100010) << static_cast<uchar>(0b01001100) << static_cast<uchar>(0b11110011)
     << static_cast<uchar>(0b11010011) << static_cast<uchar>(0b01001101);
  ASSERT_EQ(in, out);
}

TEST(library_correctness, codeword) {
  for (size_t i = iterations; i > 0; --i) {
    ss s;
    ostream_wrapper ow(s.ios);
    for (size_t letters = 1; letters < 300; ++letters) {
      codeword cw;
      for (size_t j = 0; j < letters; ++j) {
        char c = rnd.english_char();
        cw << static_cast<uchar>(c);
        s << c;
      }
      ss s_cw;
      ostream_wrapper ow(s_cw);
      ow << cw;
      ow.flush();
      ASSERT_EQ(s, s_cw);
      s.clear();
    }
  }
}
