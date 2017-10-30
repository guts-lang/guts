/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
 *
 * Permission is hereby granted, free of chargeo any person obtaining a copy
 * of this software and associated documentation files (the "Software")o deal
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACTORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!@file ds/dstr.h
 * @author uael
 */
#ifndef __DS_DSTR_H
# define __DS_DSTR_H

#include <mem.h>

#include "seq.h"

#define DSTR_ITY char_t

#define dstrof(BITS) \
  struct { \
    DSTR_ITY *buf; \
    UTY(BITS) cap, len; \
  }
#define dstrit(VAL, SEQ) \
  ((VAL) = (SEQ).buf), \
  (*(VAL)), \
  (++(VAL))
#define dstrit_kv(IDX, VAL, SEQ) \
  ((VAL) = (SEQ).buf, (IDX) = 0), \
  ((IDX) < (SEQ).len), \
  ((VAL) = (SEQ).buf + ++(IDX))

#define DSTR_DECL_len(SCOPE, ID, BITS) \
  SCOPE UTY(BITS) \
  SEQ_M(ID, len)(SEQ_SELF_TY(ID) self)

#define DSTR_DECL_append(SCOPE, ID, BITS) \
  SCOPE DSTR_ITY * \
  SEQ_M(ID, append)(SEQ_SELF_TY(ID) self, DSTR_ITY __const *buf)

#define DSTR_DECL_prepend(SCOPE, ID, BITS) \
  SCOPE DSTR_ITY * \
  SEQ_M(ID, prepend)(SEQ_SELF_TY(ID) self, DSTR_ITY __const *buf)

#define DSTR_DECL_emplace(SCOPE, ID, BITS) \
  SCOPE DSTR_ITY * \
  SEQ_M(ID, emplace)(SEQ_SELF_TY(ID) self, UTY(BITS) idx, DSTR_ITY __const *buf)

#define DSTR_DECL_DFT(SCOPE, ID, BITS) \
  typedef dstrof(BITS) SEQ_TY(ID); \
  SEQ_DECL(SCOPE, ID, DSTR_ITY, BITS); \
  DSTR_DECL_len(SCOPE, ID, BITS); \
  DSTR_DECL_append(SCOPE, ID, BITS); \
  DSTR_DECL_prepend(SCOPE, ID, BITS); \
  DSTR_DECL_emplace(SCOPE, ID, BITS)

#define DSTR8_DECL(SCOPE, ID) DSTR_DECL_DFT(SCOPE, ID, 8)
#define DSTR16_DECL(SCOPE, ID) DSTR_DECL_DFT(SCOPE, ID, 16)
#define DSTR32_DECL(SCOPE, ID) DSTR_DECL_DFT(SCOPE, ID, 32)
#define DSTR_DECL(SCOPE, ID) DSTR_DECL_DFT(SCOPE, ID, size)

#define DSTR_IMPL_len(SCOPE, ID, BITS) \
  DSTR_DECL_len(SCOPE, ID, BITS) { \
    return self->len; \
  }

#define DSTR_IMPL_append(SCOPE, ID, BITS) \
  DSTR_DECL_append(SCOPE, ID, BITS) { \
    return SEQ_M(ID, pushncpy)(self, buf, strlen(buf)); \
  }

#define DSTR_IMPL_prepend(SCOPE, ID, BITS) \
  DSTR_DECL_prepend(SCOPE, ID, BITS) { \
    return SEQ_M(ID, unshiftncpy)(self, buf, strlen(buf)); \
  }

#define DSTR_IMPL_emplace(SCOPE, ID, BITS) \
  DSTR_DECL_emplace(SCOPE, ID, BITS) { \
    return SEQ_M(ID, putncpy)(self, idx, buf, strlen(buf)); \
  }

#define DSTR_IMPL_pushn(SCOPE, ID, T, BITS) \
  SEQ_DECL_pushn(SCOPE, ID, T, BITS) { \
    T* it; \
    SEQ_M(ID, grow)(self, n); \
    it = SEQ_M(ID, end)(self); \
    self->len += n; \
    it[n] = '\0'; \
    return it; \
  }

#define DSTR_IMPL_unshiftn(SCOPE, ID, T, BITS) \
  SEQ_DECL_unshiftn(SCOPE, ID, T, BITS) { \
    UTY(BITS) len; \
    T* it; \
    SEQ_M(ID, grow)(self, n); \
    it = SEQ_M(ID, begin)(self); \
    if ((len = SEQ_M(ID, size)(self)) > 0) { \
      memmove( \
        it + n, \
        it, \
        (usize_t) len + 1 * sizeof(T) \
      ); \
    } else { \
      it[n] = '\0'; \
    } \
    self->len += n; \
    return it; \
  }

#define DSTR_IMPL_popn(SCOPE, ID, T, BITS) \
  SEQ_DECL_popn(SCOPE, ID, T, BITS) { \
    UTY(BITS) len; \
    if ((len = SEQ_M(ID, size)(self)) == 0) \
      return 0; \
    else { \
      T *it; \
      if (n > len) n = len; \
      self->len -= n; \
      it = SEQ_M(ID, end)(self); \
      if (out != nil) { \
        memcpy(out, it, (usize_t) n * sizeof(T)); \
      } \
      *it = '\0'; \
      return n; \
    } \
  }

#define DSTR_IMPL_DFT_X(SCOPE, ID, BITS, REALLOC, FREE) \
  SEQ_IMPL(SCOPE, ID, DSTR_ITY, BITS, SEQ_GROW_POW2, REALLOC, FREE, \
    SEQ_IMPL_size, \
    SEQ_IMPL_begin, \
    SEQ_IMPL_end, \
    DSTR_IMPL_pushn, \
    DSTR_IMPL_unshiftn, \
    DSTR_IMPL_popn, \
    SEQ_IMPL_shiftn \
  ) \
  DSTR_IMPL_len(SCOPE FORCEINLINE, ID, BITS) \
  DSTR_IMPL_append(SCOPE FORCEINLINE, ID, BITS) \
  DSTR_IMPL_prepend(SCOPE FORCEINLINE, ID, BITS) \
  DSTR_IMPL_emplace(SCOPE FORCEINLINE, ID, BITS)
#define DSTR8_IMPL_DFT(SCOPE, ID, REALLOC, FREE) \
  DSTR_IMPL_DFT_X(SCOPE, ID, 8, REALLOC, FREE)
#define DSTR16_IMPL_DFT(SCOPE, ID, REALLOC, FREE) \
  DSTR_IMPL_DFT_X(SCOPE, ID, 16, REALLOC, FREE)
#define DSTR32_IMPL_DFT(SCOPE, ID, REALLOC, FREE) \
  DSTR_IMPL_DFT_X(SCOPE, ID, 32, REALLOC, FREE)
#define DSTR_IMPL_DFT(SCOPE, ID, REALLOC, FREE) \
  DSTR_IMPL_DFT_X(SCOPE, ID, size, REALLOC, FREE)

#define DSTR_IMPL_X(SCOPE, ID, BITS) \
  DSTR_IMPL_DFT_X(SCOPE, ID, BITS, mem_realloc, mem_free)
#define DSTR8_IMPL(SCOPE, ID) \
  DSTR_IMPL_X(SCOPE, ID, 8)
#define DSTR16_IMPL(SCOPE, ID) \
  DSTR_IMPL_X(SCOPE, ID, 16)
#define DSTR32_IMPL(SCOPE, ID) \
  DSTR_IMPL_X(SCOPE, ID, 32)
#define DSTR_IMPL(SCOPE, ID) \
  DSTR_IMPL_X(SCOPE, ID, size)

#define DSTR_DEFINE_DFT_X(ID, BITS, REALLOC, FREE) \
  typedef dstrof(BITS) SEQ_TY(ID); \
  DSTR_IMPL_DFT_X(static, ID, BITS, REALLOC, FREE)
#define DSTR8_DEFINE_DFT(ID, REALLOC, FREE) \
  DSTR_DEFINE_DFT_X(ID, 8, REALLOC, FREE)
#define DSTR16_DEFINE_DFT(ID, REALLOC, FREE) \
  DSTR_DEFINE_DFT_X(ID, 16, REALLOC, FREE)
#define DSTR32_DEFINE_DFT(ID, REALLOC, FREE) \
  DSTR_DEFINE_DFT_X(ID, 32, REALLOC, FREE)
#define DSTR_DEFINE_DFT(ID, REALLOC, FREE) \
  DSTR_DEFINE_DFT_X(ID, size, REALLOC, FREE)

#define DSTR_DEFINE_X(ID, BITS) \
  DSTR_DEFINE_DFT_X(ID, BITS, mem_realloc, mem_free)
#define DSTR8_DEFINE(ID) DSTR_DEFINE_X(ID, 8)
#define DSTR16_DEFINE(ID) DSTR_DEFINE_X(ID, 16)
#define DSTR32_DEFINE(ID) DSTR_DEFINE_X(ID, 32)
#define DSTR_DEFINE(ID) DSTR_DEFINE_X(ID, size)

DSTR8_DECL(__api__, dstr8);
DSTR16_DECL(__api__, dstr16);
DSTR32_DECL(__api__, dstr32);
DSTR_DECL(__api__, dstr);

#endif /* !__DS_DSTR_H */
