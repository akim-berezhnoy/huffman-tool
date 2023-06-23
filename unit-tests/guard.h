#pragma once

#include "ss.h"

template<typename T>
class guard {
public:
  explicit guard(T& stream)
      : stream_after_cycle(stream)
      , initial_stream(stream)
  {}

  guard(const guard&) = delete;

  ~guard() {
    destruct();
  }

private:
  void destruct() const {
    ASSERT_EQ(initial_stream, stream_after_cycle);
  }

private:
  const T& stream_after_cycle;
  T initial_stream;
};

