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

/*!@file mem/ator/lockfree.h
 * @author uael
 */
#ifndef __MEM_ATOR_LOCKFREE_H
# define __MEM_ATOR_LOCKFREE_H

#include "base.h"

#define LOCKFREE_ATOR(BITS) \
  ATOR(BITS, \
    UTY(BITS) offset; \
  )

#define LOCKFREE_ATOR_IMPL_dtor(SCOPE, ID, BITS, ...) \
  ATOR_IMPL_dtor(SCOPE, ID, BITS, { \
    __VA_ARGS__ \
  })

#define LOCKFREE_ATOR_IMPL_alloc(SCOPE, ID, BITS, ...) \
  ATOR_IMPL_alloc(SCOPE, ID, BITS, { \
    (void)self; \
    (void)size; \
    (void)align; \
    return nil; \
  })

#define LOCKFREE_ATOR_IMPL_free(SCOPE, ID, BITS, ...) \
  ATOR_IMPL_free(SCOPE, ID, BITS, { \
    (void)self; \
    (void)ptr; \
  })

#define LOCKFREE_ATOR_DECL(SCOPE, ID, BITS) \
  ATOR_DECL(SCOPE, ID, BITS, \
    ATOR_DECL_ctor, \
    ATOR_DECL_dtor, \
    ATOR_DECL_alloc, \
    ATOR_DECL_free \
  )

#define LOCKFREE_ATOR_IMPL(SCOPE, ID, BITS) \
  ATOR_IMPL(SCOPE, ID, BITS, \
    ATOR_IMPL_ctor, \
    LOCKFREE_ATOR_IMPL_dtor, \
    LOCKFREE_ATOR_IMPL_alloc, \
    LOCKFREE_ATOR_IMPL_free \
  )

#define LOCKFREE_ATOR_DEFINE(ID, BITS) \
  typedef LOCKFREE_ATOR(BITS) ID##_t; \
  LOCKFREE_ATOR_IMPL(static FORCEINLINE, ID, BITS)

LOCKFREE_ATOR_DEFINE(lockfree_ator, size)

#endif /* !__MEM_ATOR_LOCKFREE_H */
