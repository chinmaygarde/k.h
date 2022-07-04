#include "khash.h"

KHash KHashCombine(KHash a, KHash b) {
  a ^= b + 0x9e3779b9 + (a << 6) + (a >> 2);
  return a;
}

// http://isthe.com/chongo/tech/comp/fnv/#FNV-param
static KHash kOffsetBasis = 14695981039346656037u;
static KHash kFNVPrime = 1099511628211u;

KHash KHashCreate(const uint8_t* data, size_t length) {
  KHash hash = kOffsetBasis;
  for (size_t i = 0; i < length; i++) {
    hash ^= data[i];
    hash *= kFNVPrime;
  }
  return hash;
}
