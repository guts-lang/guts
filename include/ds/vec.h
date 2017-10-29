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

/*!@file ds/vec.h
 * @author uael
 */
#ifndef __DS_VEC_H
# define __DS_VEC_H

#include <mem.h>

#include "seq.h"

#define invec(V) (V).buf, (V).len
#define vecof(T, BITS) \
  struct { \
    T *buf; \
    UTY(BITS) cap, len; \
  }

#define VEC_DECL_DFT(SCOPE, ID, T, BITS) \
  typedef vecof(T, BITS) seq_tyof(ID); \
  SEQ_DECL(SCOPE, ID, T, BITS)

#define VEC8_DECL(SCOPE, ID, T) VEC_DECL_DFT(SCOPE, ID, T, 8)
#define VEC16_DECL(SCOPE, ID, T) VEC_DECL_DFT(SCOPE, ID, T, 16)
#define VEC32_DECL(SCOPE, ID, T) VEC_DECL_DFT(SCOPE, ID, T, 32)
#define VEC64_DECL(SCOPE, ID, T) VEC_DECL_DFT(SCOPE, ID, T, 64)
#define VEC_DECL(SCOPE, ID, T) VEC_DECL_DFT(SCOPE, ID, T, size)

#define VEC_IMPL_DFT(SCOPE, ID, T, BITS, REALLOC, FREE, CMP) \
  SEQ_IMPL(SCOPE, ID, T, BITS, cap, len, buf, REALLOC, FREE, CMP, \
    SEQ_IMPL_size, \
    SEQ_IMPL_begin, \
    SEQ_IMPL_end, \
    SEQ_IMPL_pushn, \
    SEQ_IMPL_unshiftn, \
    SEQ_IMPL_popn, \
    SEQ_IMPL_shiftn \
  )
#define VEC8_IMPL_DFT(SCOPE, ID, T, REALLOC, FREE, CMP) \
  VEC_IMPL_DFT(SCOPE, ID, T, 8, REALLOC, FREE, CMP)
#define VEC16_IMPL_DFT(SCOPE, ID, T, REALLOC, FREE, CMP) \
  VEC_IMPL_DFT(SCOPE, ID, T, 16, REALLOC, FREE, CMP)
#define VEC32_IMPL_DFT(SCOPE, ID, T, REALLOC, FREE, CMP) \
  VEC_IMPL_DFT(SCOPE, ID, T, 32, REALLOC, FREE, CMP)
#define VEC64_IMPL_DFT(SCOPE, ID, T, REALLOC, FREE, CMP) \
  VEC_IMPL_DFT(SCOPE, ID, T, 64, REALLOC, FREE, CMP)

#define VEC_IMPL_X(SCOPE, ID, T, BITS, CMP) \
  VEC_IMPL_DFT(SCOPE, ID, T, BITS, mem_realloc, mem_free, CMP)
#define VEC8_IMPL(SCOPE, ID, T, CMP) \
  VEC_IMPL_X(SCOPE, ID, T, 8, CMP)
#define VEC16_IMPL(SCOPE, ID, T, CMP) \
  VEC_IMPL_X(SCOPE, ID, T, 16, CMP)
#define VEC32_IMPL(SCOPE, ID, T, CMP) \
  VEC_IMPL_X(SCOPE, ID, T, 32, CMP)
#define VEC64_IMPL(SCOPE, ID, T, CMP) \
  VEC_IMPL_X(SCOPE, ID, T, 64, CMP)
#define VEC_IMPL(SCOPE, ID, T, CMP) \
  VEC_IMPL_X(SCOPE, ID, T, size, CMP)

#define VEC_DEFINE_DFT(ID, T, BITS, REALLOC, FREE, CMP) \
  typedef vecof(T, BITS) seq_tyof(ID); \
  VEC_IMPL_DFT(static, ID, T, BITS, REALLOC, FREE, CMP)
#define VEC8_DEFINE_DFT(ID, T, REALLOC, FREE, CMP) \
  VEC_DEFINE_DFT(ID, T, 8, REALLOC, FREE, CMP)
#define VEC16_DEFINE_DFT(ID, T, REALLOC, FREE, CMP) \
  VEC_DEFINE_DFT(ID, T, 16, REALLOC, FREE, CMP)
#define VEC32_DEFINE_DFT(ID, T, REALLOC, FREE, CMP) \
  VEC_DEFINE_DFT(ID, T, 32, REALLOC, FREE, CMP)
#define VEC64_DEFINE_DFT(ID, T, REALLOC, FREE, CMP) \
  VEC_DEFINE_DFT(ID, T, 64, REALLOC, FREE, CMP)

#define VEC_DEFINE_X(ID, T, BITS, CMP) \
  VEC_DEFINE_DFT(ID, T, BITS, mem_realloc, mem_free, CMP)
#define VEC8_DEFINE(ID, T, CMP) VEC_DEFINE_X(ID, T, 8, CMP)
#define VEC16_DEFINE(ID, T, CMP) VEC_DEFINE_X(ID, T, 16, CMP)
#define VEC32_DEFINE(ID, T, CMP) VEC_DEFINE_X(ID, T, 32, CMP)
#define VEC_DEFINE(ID, T, CMP) VEC_DEFINE_X(ID, T, size, CMP)

VEC_DECL(__api__, i8vec, i8_t);
VEC_DECL(__api__, u8vec, u8_t);
VEC_DECL(__api__, i16vec, i16_t);
VEC_DECL(__api__, u16vec, u16_t);
VEC_DECL(__api__, i32vec, i32_t);
VEC_DECL(__api__, u32vec, u32_t);
VEC_DECL(__api__, charvec, char_t);
VEC_DECL(__api__, strvec, char_t *);
VEC_DECL(__api__, errs, ex_t);

#endif /* !__DS_VEC_H */
