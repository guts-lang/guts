/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <nt/tys.h>
#include <nt/math.h>

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
