#include "decoder.h"
#include "encoder.h"

void encode(istream& is, ostream& os) {
  encoder::apply_encode(is, os);
}

void decode(istream& is, ostream& os) {
  decoder::apply_decode(is, os);
}
