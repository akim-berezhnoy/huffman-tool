#pragma once

#include "istream_wrapper.h"
#include "ostream_wrapper.h"

struct signature_manager {

  signature_manager() = delete;
  signature_manager(const signature_manager&) = delete;
  signature_manager& operator=(const signature_manager&) = delete;

  struct signature_error : std::exception {
    const char* what() const noexcept override {
      return "Input file was not encoded bu this program. Please, check your file.";
    }
  };

  inline static const std::string SIGNATURE = "\nHUFFMAN_AKIM_BEREZHNOY_ARCHIVE_228_1337\n\n";

  static void check_signature(istream_wrapper& iw) {
    for (uchar c : SIGNATURE) {
      if (iw.read() != c) {
        throw signature_error();
      }
    }
  }

  static void add_signature(ostream_wrapper& ow) {
    for (uchar c : SIGNATURE) {
      ow.write(c);
    }
  }

};
