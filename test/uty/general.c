#include <uty.h>
#include <assert.h>
#include <stdlib.h>

int main(void) {
  assert(NULL == nil);
  assert(sizeof(char) == sizeof(i8_t));
  assert(sizeof(unsigned char) == sizeof(u8_t));
  assert(sizeof(short) == sizeof(i16_t));
  assert(sizeof(unsigned short) == sizeof(u16_t));
  assert(sizeof(int) == sizeof(i32_t));
  assert(sizeof(unsigned int) == sizeof(u32_t));
  assert(sizeof(long long) == sizeof(i64_t));
  assert(sizeof(unsigned long long) == sizeof(u64_t));
  assert(sizeof(float) == sizeof(f32_t));
  assert(sizeof(double) == sizeof(f64_t));
  assert((-128) == I8_MIN);
  assert((-32768) == I16_MIN);
  assert((-2147483647 - 1) == I32_MIN);
  assert((-9223372036854775807LL - 1) == I64_MIN);
  assert(127 == I8_MAX);
  assert(32767 == I16_MAX);
  assert(2147483647 == I32_MAX);
  assert(9223372036854775807LL == I64_MAX);
  assert(0xffU == U8_MAX);
  assert(0xffffU == U16_MAX);
  assert(0xffffffffUL == U32_MAX);
  assert(0xffffffffffffffffULL == U64_MAX);
  assert(true == i8eq(I8_MIN, I8_MIN));
  assert(true == u8eq(U8_MAX, U8_MAX));
  assert(true == i16eq(I16_MIN, I16_MIN));
  assert(true == u16eq(U16_MAX, U16_MAX));
  assert(true == i32eq(I32_MIN, I32_MIN));
  assert(true == u32eq(U32_MAX, U32_MAX));
  assert(true == i64eq(I64_MIN, I64_MIN));
  assert(true == u64eq(U64_MAX, U64_MAX));
  assert(true == streq("oops", "oops"));
  return 0;
}
