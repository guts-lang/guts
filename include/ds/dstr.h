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

/*!@file ds/dstr.h
 * @author uael
 */
#ifndef __DS_DSTR_H
# define __DS_DSTR_H

#include <mem.h>

#include "seq.h"

#define indstr(D) (D).buf, (D).len
#define dstrof(TSizeBits) \
  struct { \
    char_t *buf; \
    u##TSizeBits##_t len, cap; \
  }

#define DSTR_IMPL_trim(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_trim(SCOPE, ID, T, BITS) { \
    (void) self; \
  }

#define DSTR_DECL(SCOPE, ID, BITS) \
  SEQ_DECL(SCOPE, ID, char_t, BITS, \
    SEQ_DECL_ctor, \
    SEQ_DECL_dtor, \
    SEQ_DECL_cap, \
    SEQ_DECL_size, \
    SEQ_DECL_at, \
    SEQ_DECL_offset, \
    SEQ_DECL_realloc, \
    SEQ_DECL_ensure, \
    SEQ_DECL_grow, \
    SEQ_DECL_shrink, \
    SEQ_DECL_trim, \
    SEQ_DECL_push, \
    SEQ_DECL_append_nt, \
    SEQ_DECL_pop, \
    SEQ_DECL_unshift, \
    SEQ_DECL_prepend_nt, \
    SEQ_DECL_shift, \
    SEQ_DECL_nshift, \
    SEQ_DECL_insert, \
    SEQ_DECL_emplace_nt, \
    SEQ_DECL_remove, \
    SEQ_DECL_removen, \
    SEQ_DECL_erase, \
    SEQ_DECL_erasen, \
    SEQ_DECL_eraseonce, \
    SEQ_DECL_cpy, \
    SEQ_DECL_ncpy \
  )
#define DSTR8_DECL(SCOPE, ID) DSTR_DECL(SCOPE, ID, 8)
#define DSTR16_DECL(SCOPE, ID) DSTR_DECL(SCOPE, ID, 16)
#define DSTR32_DECL(SCOPE, ID) DSTR_DECL(SCOPE, ID, 32)
#define DSTR64_DECL(SCOPE, ID) DSTR_DECL(SCOPE, ID, 64)

#define DSTR_IMPL_DFT(SCOPE, ID, BITS, REALLOC, FREE) \
  SEQ_IMPL(SCOPE, ID, char_t, BITS, cap, len, buf, REALLOC, FREE, i8cmp, \
    SEQ_IMPL_ctor, \
    SEQ_IMPL_dtor, \
    SEQ_IMPL_cap, \
    SEQ_IMPL_size, \
    SEQ_IMPL_at, \
    SEQ_IMPL_offset, \
    SEQ_IMPL_realloc, \
    SEQ_IMPL_ensure_strict, \
    SEQ_IMPL_grow_strict, \
    SEQ_IMPL_shrink, \
    DSTR_IMPL_trim, \
    SEQ_IMPL_push_nt, \
    SEQ_IMPL_append_nt, \
    SEQ_IMPL_pop_nt, \
    SEQ_IMPL_unshift, \
    SEQ_IMPL_prepend_nt, \
    SEQ_IMPL_shift, \
    SEQ_IMPL_nshift, \
    SEQ_IMPL_insert, \
    SEQ_IMPL_emplace_nt, \
    SEQ_IMPL_remove, \
    SEQ_IMPL_removen, \
    SEQ_IMPL_erase, \
    SEQ_IMPL_erasen, \
    SEQ_IMPL_eraseonce, \
    SEQ_IMPL_cpy_nt, \
    SEQ_IMPL_ncpy_nt \
  )
#define DSTR8_IMPL_DFT(SCOPE, ID, REALLOC, FREE) \
  DSTR_IMPL_DFT(SCOPE, ID, 8, REALLOC, FREE)
#define DSTR16_IMPL_DFT(SCOPE, ID, REALLOC, FREE) \
  DSTR_IMPL_DFT(SCOPE, ID, 16, REALLOC, FREE)
#define DSTR32_IMPL_DFT(SCOPE, ID, REALLOC, FREE) \
  DSTR_IMPL_DFT(SCOPE, ID, 32, REALLOC, FREE)
#define DSTR64_IMPL_DFT(SCOPE, ID, REALLOC, FREE) \
  DSTR_IMPL_DFT(SCOPE, ID, 64, REALLOC, FREE)

#define DSTR_IMPL(SCOPE, ID, BITS) \
  DSTR_IMPL_DFT(SCOPE, ID, BITS, mem_realloc, mem_free)
#define DSTR8_IMPL(SCOPE, ID) \
  DSTR_IMPL(SCOPE, ID, 8)
#define DSTR16_IMPL(SCOPE, ID) \
  DSTR_IMPL(SCOPE, ID, 16)
#define DSTR32_IMPL(SCOPE, ID) \
  DSTR_IMPL(SCOPE, ID, 32)
#define DSTR64_IMPL(SCOPE, ID) \
  DSTR_IMPL(SCOPE, ID, 64)

#define DSTR_DEFINE_DFT(ID, BITS, REALLOC, FREE) \
  typedef dstrof(BITS) ID##_t; \
  DSTR_IMPL_DFT(static FORCEINLINE, ID, BITS, REALLOC, FREE)
#define DSTR8_DEFINE_DFT(ID, REALLOC, FREE) \
  DSTR_DEFINE_DFT(ID, 8, REALLOC, FREE)
#define DSTR16_DEFINE_DFT(ID, REALLOC, FREE) \
  DSTR_DEFINE_DFT(ID, 16, REALLOC, FREE)
#define DSTR32_DEFINE_DFT(ID, REALLOC, FREE) \
  DSTR_DEFINE_DFT(ID, 32, REALLOC, FREE)
#define DSTR64_DEFINE_DFT(ID, REALLOC, FREE) \
  DSTR_DEFINE_DFT(ID, 64, REALLOC, FREE)

#define DSTR_DEFINE_X(ID, BITS) \
  DSTR_DEFINE_DFT(ID, BITS, mem_realloc, mem_free)
#define DSTR8_DEFINE(ID) DSTR_DEFINE_X(ID, 8)
#define DSTR16_DEFINE(ID) DSTR_DEFINE_X(ID, 16)
#define DSTR32_DEFINE(ID) DSTR_DEFINE_X(ID, 32)
#define DSTR_DEFINE(ID) DSTR_DEFINE_X(ID, size)

DSTR8_DEFINE(dstr8)
DSTR16_DEFINE(dstr16)
DSTR32_DEFINE(dstr32)
DSTR_DEFINE(dstr)

#endif /* !__DS_DSTR_H */
