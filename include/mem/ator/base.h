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

/*!@file mem/ator/base.h
 * @author uael
 */
#ifndef __MEM_ATOR_BASE_H
# define __MEM_ATOR_BASE_H

#include <nt/tys.h>
#include <nt/err.h>

#define ATOR(BITS, ...) \
  struct { \
    __VA_ARGS__ \
  }

#define ATOR_DECL_ctor(SCOPE, ID, BITS) \
  SCOPE void \
  ID##_ctor(ID##_t *__restrict self)

#define ATOR_IMPL_ctor(SCOPE, ID, BITS, ...) \
  ATOR_DECL_ctor(SCOPE, ID, BITS) { \
    init(self, ID##_t); \
    __VA_ARGS__ \
  }

#define ATOR_DECL_dtor(SCOPE, ID, BITS) \
  SCOPE void \
  ID##_dtor(ID##_t *__restrict self)

#define ATOR_IMPL_dtor(SCOPE, ID, BITS, ...) \
  ATOR_DECL_dtor(SCOPE, ID, BITS) __VA_ARGS__

#define ATOR_DECL_alloc(SCOPE, ID, BITS) \
  SCOPE ret_t \
  ID##_alloc(ID##_t *__restrict self, void **out, UTY(BITS) size, \
    UTY(BITS) align)

#define ATOR_IMPL_alloc(SCOPE, ID, BITS, ...) \
  ATOR_DECL_alloc(SCOPE, ID, BITS) { \
    __VA_ARGS__ \
    return RET_NOT_IMPL; \
  }

#define ATOR_DECL_free(SCOPE, ID, BITS) \
  SCOPE ret_t \
  ID##_free(ID##_t *__restrict self, void *ptr)

#define ATOR_IMPL_free(SCOPE, ID, BITS, ...) \
  ATOR_DECL_free(SCOPE, ID, BITS) { \
    __VA_ARGS__ \
    return RET_NOT_IMPL; \
  }

#define ATOR_DECL( \
    SCOPE, ID, BITS, \
    DECL_ctor, DECL_dtor, \
    DECL_alloc, DECL_free) \
  DECL_ctor(SCOPE, ID, BITS); \
  DECL_dtor(SCOPE, ID, BITS); \
  DECL_alloc(SCOPE, ID, BITS) MALLOC; \
  DECL_free(SCOPE, ID, BITS);

#define ATOR_IMPL( \
    SCOPE, ID, BITS, \
    IMPL_ctor, IMPL_dtor, \
    IMPL_alloc, IMPL_free) \
  IMPL_ctor(SCOPE, ID, BITS) \
  IMPL_dtor(SCOPE, ID, BITS) \
  IMPL_alloc(SCOPE, ID, BITS) \
  IMPL_free(SCOPE, ID, BITS)

#define ATOR_PADD(addr, align) \
  (((((addr) / (align)) + 1) * (align)) - (addr))

#endif /* !__MEM_ATOR_BASE_H */
