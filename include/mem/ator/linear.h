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

/*!@file mem/ator/linear.h
 * @author uael
 */
#ifndef __MEM_ATOR_LINEAR_H
# define __MEM_ATOR_LINEAR_H

#include "base.h"

#define LINEAR_ATOR(BITS) \
  ATOR(BITS, \
    void *begin; \
    UTY(BITS) limit; \
    UTY(BITS) used; \
    UTY(BITS) peak; \
    UTY(BITS) offset; \
  )

#define LINEAR_ATOR_IMPL_dtor(SCOPE, ID, BITS, ...) \
  ATOR_IMPL_dtor(SCOPE, ID, BITS, { \
    if (self->begin) { \
      free(self->begin); \
      self->begin = nil; \
    } \
    __VA_ARGS__ \
  })

#define LINEAR_ATOR_IMPL_alloc(SCOPE, ID, BITS, ...) \
  ATOR_IMPL_alloc(SCOPE, ID, BITS, { \
    UTY(BITS) padding; \
    usize_t addr, next; \
    if (self->begin == nil) { \
      if (self->limit == 0) self->limit = UMAX(32); \
      if ((self->begin = malloc(self->limit)) == nil) { \
        return RET_ERRNO; \
      } \
    } \
    padding = 0; \
    addr = (usize_t) self->begin + (usize_t) self->offset; \
    if (align != 0 && self->offset % align != 0) { \
      padding = ATOR_PADD(addr, align); \
    } \
    self->offset += padding; \
    next = addr + padding; \
    self->offset += size; \
    self->used = self->offset; \
    if (self->used > self->peak) { \
      self->peak = self->used; \
    } \
    *out = (void *) next; \
    return RET_SUCCESS; \
  })

#define LINEAR_ATOR_IMPL_free(SCOPE, ID, BITS, ...) \
  ATOR_IMPL_free(SCOPE, ID, BITS, { \
    (void)self; \
    (void)ptr; \
    return RET_FAILURE; \
  })

#define LINEAR_ATOR_DECL(SCOPE, ID, BITS) \
  ATOR_DECL(SCOPE, ID, BITS, \
    ATOR_DECL_ctor, \
    ATOR_DECL_dtor, \
    ATOR_DECL_alloc, \
    ATOR_DECL_free \
  )

#define LINEAR_ATOR_IMPL(SCOPE, ID, BITS) \
  ATOR_IMPL(SCOPE, ID, BITS, \
    ATOR_IMPL_ctor, \
    LINEAR_ATOR_IMPL_dtor, \
    LINEAR_ATOR_IMPL_alloc, \
    LINEAR_ATOR_IMPL_free \
  )

#define LINEAR_ATOR_DEFINE(ID, BITS) \
  typedef LINEAR_ATOR(BITS) ID##_t; \
  LINEAR_ATOR_IMPL(static FORCEINLINE, ID, BITS)

LINEAR_ATOR_DEFINE(linear_ator32, 32)

#endif /* !__MEM_ATOR_LINEAR_H */
