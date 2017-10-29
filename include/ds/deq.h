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

/*!@file ds/deq.h
 * @author uael
 */
#ifndef __DS_DEQ_H
# define __DS_DEQ_H

#include <mem.h>

#include "seq.h"

#define deqof(T, BITS) \
  struct { \
    T *buf; \
    UTY(BITS) cap, len, cur; \
  }
#define deqit(VAL, SEQ) \
  ((VAL) = (SEQ).buf + (SEQ).cur), \
  ((VAL) != (SEQ).buf + (SEQ).len), \
  (++(VAL))
#define deqit_kv(IDX, VAL, SEQ) \
  ((VAL) = (SEQ).buf + (SEQ).cur, (IDX) = 0), \
  ((IDX) < (SEQ).len - (SEQ).cur), \
  ((VAL) = (SEQ).buf + ++(IDX))

#define DEQ_DECL_DFT(SCOPE, ID, T, BITS) \
  typedef deqof(T, BITS) SEQ_TY(ID); \
  SEQ_DECL(SCOPE, ID, T, BITS)

#define DEQ8_DECL(SCOPE, ID, T) DEQ_DECL_DFT(SCOPE, ID, T, 8)
#define DEQ16_DECL(SCOPE, ID, T) DEQ_DECL_DFT(SCOPE, ID, T, 16)
#define DEQ32_DECL(SCOPE, ID, T) DEQ_DECL_DFT(SCOPE, ID, T, 32)
#define DEQ64_DECL(SCOPE, ID, T) DEQ_DECL_DFT(SCOPE, ID, T, 64)
#define DEQ_DECL(SCOPE, ID, T) DEQ_DECL_DFT(SCOPE, ID, T, size)

#define DEQ_IMPL_size(SCOPE, ID, T, BITS) \
  SEQ_DECL_size(SCOPE, ID, T, BITS) { \
    return self->len - self->cur; \
  }

#define DEQ_IMPL_begin(SCOPE, ID, T, BITS) \
  SEQ_DECL_begin(SCOPE, ID, T, BITS) { \
    return self->buf + self->cur; \
  }

#define DEQ_IMPL_end(SCOPE, ID, T, BITS) \
  SEQ_DECL_end(SCOPE, ID, T, BITS) { \
    return self->buf + self->len - self->cur; \
  }

#define DEQ_IMPL_unshiftn(SCOPE, ID, T, BITS) \
  SEQ_DECL_unshiftn(SCOPE, ID, T, BITS) { \
    UTY(BITS) len; \
    T* it; \
    SEQ_M(ID, grow)(self, n); \
    if (self->cur >= n) { \
      self->cur -= n; \
      return SEQ_M(ID, begin)(self); \
    } else if (self->cur) { \
      n -= self->cur; \
      self->cur = 0; \
    } \
    it = SEQ_M(ID, begin)(self); \
    if ((len = SEQ_M(ID, size)(self)) > 0) { \
      memmove( \
        it + n, \
        it, \
        (usize_t) len * sizeof(T) \
      ); \
    } \
    self->len += n; \
    return it; \
  }

#define DEQ_IMPL_shiftn(SCOPE, ID, T, BITS) \
  SEQ_DECL_shiftn(SCOPE, ID, T, BITS) { \
    UTY(BITS) len; \
    if ((len = SEQ_M(ID, size)(self)) == 0) \
      return 0; \
    else { \
      T* it; \
      if (n > len) n = len; \
      it = SEQ_M(ID, begin)(self); \
      if (out != nil) { \
        memcpy(out, it, (usize_t) n * sizeof(T)); \
      } \
      self->cur += n; \
      return n; \
    } \
  }

#define DEQ_IMPL_DFT_X(SCOPE, ID, T, BITS, REALLOC, FREE) \
  SEQ_IMPL(SCOPE, ID, T, BITS, SEQ_GROW_POW2, REALLOC, FREE, \
    DEQ_IMPL_size, \
    DEQ_IMPL_begin, \
    DEQ_IMPL_end, \
    SEQ_IMPL_pushn, \
    DEQ_IMPL_unshiftn, \
    SEQ_IMPL_popn, \
    DEQ_IMPL_shiftn \
  )
#define DEQ8_IMPL_DFT(SCOPE, ID, T, REALLOC, FREE) \
  DEQ_IMPL_DFT_X(SCOPE, ID, T, 8, REALLOC, FREE)
#define DEQ16_IMPL_DFT(SCOPE, ID, T, REALLOC, FREE) \
  DEQ_IMPL_DFT_X(SCOPE, ID, T, 16, REALLOC, FREE)
#define DEQ32_IMPL_DFT(SCOPE, ID, T, REALLOC, FREE) \
  DEQ_IMPL_DFT_X(SCOPE, ID, T, 32, REALLOC, FREE)
#define DEQ_IMPL_DFT(SCOPE, ID, T, REALLOC, FREE) \
  DEQ_IMPL_DFT_X(SCOPE, ID, T, size, REALLOC, FREE)

#define DEQ_IMPL_X(SCOPE, ID, T, BITS) \
  DEQ_IMPL_DFT_X(SCOPE, ID, T, BITS, mem_realloc, mem_free)
#define DEQ8_IMPL(SCOPE, ID, T) \
  DEQ_IMPL_X(SCOPE, ID, T, 8)
#define DEQ16_IMPL(SCOPE, ID, T) \
  DEQ_IMPL_X(SCOPE, ID, T, 16)
#define DEQ32_IMPL(SCOPE, ID, T) \
  DEQ_IMPL_X(SCOPE, ID, T, 32)
#define DEQ_IMPL(SCOPE, ID, T) \
  DEQ_IMPL_X(SCOPE, ID, T, size)

#define DEQ_DEFINE_DFT_X(ID, T, BITS, REALLOC, FREE) \
  typedef deqof(T, BITS) SEQ_TY(ID); \
  DEQ_IMPL_DFT_X(static, ID, T, BITS, REALLOC, FREE)
#define DEQ8_DEFINE_DFT(ID, T, REALLOC, FREE) \
  DEQ_DEFINE_DFT_X(ID, T, 8, REALLOC, FREE)
#define DEQ16_DEFINE_DFT(ID, T, REALLOC, FREE) \
  DEQ_DEFINE_DFT_X(ID, T, 16, REALLOC, FREE)
#define DEQ32_DEFINE_DFT(ID, T, REALLOC, FREE) \
  DEQ_DEFINE_DFT_X(ID, T, 32, REALLOC, FREE)
#define DEQ_DEFINE_DFT(ID, T, REALLOC, FREE) \
  DEQ_DEFINE_DFT_X(ID, T, size, REALLOC, FREE)

#define DEQ_DEFINE_X(ID, T, BITS) \
  DEQ_DEFINE_DFT_X(ID, T, BITS, mem_realloc, mem_free)
#define DEQ8_DEFINE(ID, T) DEQ_DEFINE_X(ID, T, 8)
#define DEQ16_DEFINE(ID, T) DEQ_DEFINE_X(ID, T, 16)
#define DEQ32_DEFINE(ID, T) DEQ_DEFINE_X(ID, T, 32)
#define DEQ_DEFINE(ID, T) DEQ_DEFINE_X(ID, T, size)

DEQ_DECL(__api__, i8deq, i8_t);
DEQ_DECL(__api__, u8deq, u8_t);
DEQ_DECL(__api__, i16deq, i16_t);
DEQ_DECL(__api__, u16deq, u16_t);
DEQ_DECL(__api__, i32deq, i32_t);
DEQ_DECL(__api__, u32deq, u32_t);
DEQ_DECL(__api__, chardeq, char_t);
DEQ_DECL(__api__, strdeq, char_t *);
DEQ_DECL(__api__, errs, ex_t);

#endif /* !__DS_DEQ_H */
