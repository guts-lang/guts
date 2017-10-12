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

/*!@file nt/math.h
 * @author uael
 */
#ifndef __NT_MATH_H
# define __NT_MATH_H

#include "tys.h"

#if !defined MIN
# define MIN(a, b) (((a)<(b))?(a):(b))
#endif

#if !defined MAX
# define MAX(a, b) (((a)>(b))?(a):(b))
#endif

#if __has_builtin__(popcount)
#define ISPOW2(n) (__builtin_popcount(n) == 1)
#else
#define ISPOW2(n) (((n) != 0) && (((n) & (~(n) + 1)) == (n)))
#endif

#define i8eq(a, b) ((bool_t)((i8_t)(a) == (i8_t)(b)))
#define u8eq(a, b) ((bool_t)((u8_t)(a) == (u8_t)(b)))
#define i16eq(a, b) ((bool_t)((i16_t)(a) == (i16_t)(b)))
#define u16eq(a, b) ((bool_t)((u16_t)(a) == (u16_t)(b)))
#define i32eq(a, b) ((bool_t)((i32_t)(a) == (i32_t)(b)))
#define u32eq(a, b) ((bool_t)((u32_t)(a) == (u32_t)(b)))
#define i64eq(a, b) ((bool_t)((i64_t)(a) == (i64_t)(b)))
#define u64eq(a, b) ((bool_t)((u64_t)(a) == (u64_t)(b)))
#define isizeeq(a, b) ((bool_t)((isize_t)(a) == (isize_t)(b)))
#define usizeeq(a, b) ((bool_t)((usize_t)(a) == (usize_t)(b)))
#define f32eq(a, b) ((bool_t)((f32_t)(a) == (f32_t)(b)))
#define f64eq(a, b) ((bool_t)((f64_t)(a) == (f64_t)(b)))
#define streq(a, b) ((bool_t)(strcmp(a, b) == 0))
#define addreq(a, b) ((bool_t)(&(a) == &(b)))

#define i8cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define u8cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define i16cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define u16cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define i32cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define u32cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define i64cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define u64cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define isizecmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define usizecmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define f32cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define f64cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define addrcmp(a, b) ((i8_t)((&(a) > &(b)) - (&(a) < &(b))))

static PURE CONST FORCEINLINE u8_t
pow2_next8(u8_t n) {
  u32_t j;
  u32_t i;

  if (n == U8_MAX || ISPOW2(n)) {
    return n;
  }
  i = (u32_t) n;
  i = (u32_t) (
    (void) ((j = i & 0xFFFF0000) || (j = i)),
      (void) ((i = j & 0xFF00FF00) || (i = j)),
      (void) ((j = i & 0xF0F0F0F0) || (j = i)),
      (void) ((i = j & 0xCCCCCCCC) || (i = j)),
      (void) ((j = i & 0xAAAAAAAA) || (j = i)),
      j << 1
  );
  return (i > U8_MAX || i < (u32_t) n) ? (u8_t) U8_MAX : (u8_t) i;
}

static PURE CONST FORCEINLINE u16_t
pow2_next16(u16_t n) {
  u32_t j;
  u32_t i;

  if (n == U16_MAX || ISPOW2(n)) {
    return n;
  }
  i = (u32_t) n;
  i = (u32_t) (
    (void) ((j = i & 0xFFFF0000) || (j = i)),
      (void) ((i = j & 0xFF00FF00) || (i = j)),
      (void) ((j = i & 0xF0F0F0F0) || (j = i)),
      (void) ((i = j & 0xCCCCCCCC) || (i = j)),
      (void) ((j = i & 0xAAAAAAAA) || (j = i)),
      j << 1
  );
  return (i > U16_MAX || i < (u32_t) n) ? (u16_t) U16_MAX : (u16_t) i;
}

static PURE CONST FORCEINLINE u32_t
pow2_next32(u32_t n) {
  u32_t j;

  if (n == U32_MAX || ISPOW2(n)) {
    return n;
  }
  j = (u32_t) (
    (void) ((j = n & 0xFFFF0000) || (j = n)),
      (void) ((n = j & 0xFF00FF00) || (n = j)),
      (void) ((j = n & 0xF0F0F0F0) || (j = n)),
      (void) ((n = j & 0xCCCCCCCC) || (n = j)),
      (void) ((j = n & 0xAAAAAAAA) || (j = n)),
      j << 1
  );
  return (j < n) ? (u32_t) U32_MAX : j;
}

static PURE CONST FORCEINLINE u64_t
pow2_next64(u64_t n) {
  u64_t j;

  if (n == U64_MAX || ISPOW2(n)) {
    return n;
  }
  j = (u64_t) (
    (void) ((j = n & 0xFFFF0000LL) || (j = n)),
      (void) ((n = j & 0xFF00FF00LL) || (n = j)),
      (void) ((j = n & 0xF0F0F0F0LL) || (j = n)),
      (void) ((n = j & 0xCCCCCCCCLL) || (n = j)),
      (void) ((j = n & 0xAAAAAAAALL) || (j = n)),
      j << 1
  );
  return (j < n) ? (u64_t) U64_MAX : j;
}

static PURE CONST FORCEINLINE usize_t
pow2_nextsize(usize_t n) {
  usize_t j;

  if (n == USIZE_MAX || ISPOW2(n)) {
    return n;
  }
  j = (usize_t) (
    (void) ((j = n & 0xFFFF0000) || (j = n)),
      (void) ((n = j & 0xFF00FF00) || (n = j)),
      (void) ((j = n & 0xF0F0F0F0) || (j = n)),
      (void) ((n = j & 0xCCCCCCCC) || (n = j)),
      (void) ((j = n & 0xAAAAAAAA) || (j = n)),
      j << 1
  );
  return (j < n) ? (usize_t) USIZE_MAX : j;
}

#endif /* !__NT_MATH_H */
