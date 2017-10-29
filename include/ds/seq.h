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

/*!@file ds/seq.h
 * @author uael
 */
#ifndef __DS_SEQ_H
# define __DS_SEQ_H

#include <nt/tys.h>
#include <nt/ex.h>
#include <nt/math.h>
#include <nt/hash.h>

#ifndef SEQ_MIN_CAP
# define SEQ_MIN_CAP 32
#endif

#define seq_tyof(ID) PP_JOIN(ID, _t)
#define seq_meth(ID, M) PP_JOIN(PP_JOIN(ID, _), M)

#define foreach(idx, val, in) \
  PP_MCALL(foreach_nx, idx, val, PP_EVAL(in))

#define foreach_nx(idx, val, buf, len) \
  for ( \
    (val) = (buf), (idx) = 0; \
    (idx) < (len); \
    (val) = (buf) + ++(idx) \
  )

#define SEQ_DECL_ctor(SCOPE, ID, T, BITS) \
  SCOPE void \
  seq_meth(ID, ctor)(seq_tyof(ID) *__restrict self)

#define SEQ_IMPL_ctor(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_ctor(SCOPE, ID, T, BITS) { \
    init(self, seq_tyof(ID)); \
  }

#define SEQ_DECL_dtor(SCOPE, ID, T, BITS) \
  SCOPE void \
  seq_meth(ID, dtor)(seq_tyof(ID) *__restrict self, void (*idtor)(T *))

#define SEQ_IMPL_dtor(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_dtor(SCOPE, ID, T, BITS) { \
    if (self->BUF) { \
      if (idtor) { \
        UTY(BITS) i; \
        T *item; \
        foreach_nx(i, item, self->BUF, self->LEN) { \
          idtor(item); \
        } \
      } \
      FREE(self->BUF); \
    } \
    init(self, seq_tyof(ID)); \
  }

#define SEQ_DECL_size(SCOPE, ID, T, BITS) \
  SCOPE UTY(BITS) \
  seq_meth(ID, size)(seq_tyof(ID) *__restrict self)

#define SEQ_IMPL_size(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_size(SCOPE, ID, T, BITS) { \
    return self->LEN; \
  }

#define SEQ_DECL_begin(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, begin)(seq_tyof(ID) *__restrict self)

#define SEQ_IMPL_begin(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_begin(SCOPE, ID, T, BITS) { \
    return self->BUF; \
  }

#define SEQ_DECL_end(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, end)(seq_tyof(ID) *__restrict self)

#define SEQ_IMPL_end(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_end(SCOPE, ID, T, BITS) { \
    return self->BUF + self->LEN; \
  }

#define SEQ_DECL_at(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, at)(seq_tyof(ID) *__restrict self, UTY(BITS) idx)

#define SEQ_IMPL_at(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_at(SCOPE, ID, T, BITS) { \
    return seq_meth(ID, begin)(self) + idx; \
  }

#define SEQ_DECL_realloc(SCOPE, ID, T, BITS) \
  SCOPE void \
  seq_meth(ID, realloc)(seq_tyof(ID) *__restrict self, UTY(BITS) n)

#define SEQ_IMPL_realloc(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_realloc(SCOPE, ID, T, BITS) { \
    if (self->CAP != n) { \
      void *buf; \
      if ((buf = REALLOC(self->BUF, (usize_t) n * sizeof(T))) == nil) { \
        THROW(ex_errno(ERRLVL_FATAL, ENOMEM, nil)); \
      } \
      self->BUF = buf; \
      if (self->LEN > (self->CAP = n)) self->LEN = n; \
    } \
  }

#define SEQ_DECL_ensure(SCOPE, ID, T, BITS) \
  SCOPE void \
  seq_meth(ID, ensure)(seq_tyof(ID) *__restrict self, UTY(BITS) n)

#define SEQ_IMPL_ensure(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_ensure(SCOPE, ID, T, BITS) { \
    if (self->CAP < n) { \
      if (n < SEQ_MIN_CAP) n = SEQ_MIN_CAP; \
      else if (!ISPOW2(n)) n = pow2_next##BITS(n); \
      seq_meth(ID, realloc)(self, n); \
    } \
  }

#define SEQ_DECL_grow(SCOPE, ID, T, BITS) \
  SCOPE void \
  seq_meth(ID, grow)(seq_tyof(ID) *__restrict self, UTY(BITS) n)

#define SEQ_IMPL_grow(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_grow(SCOPE, ID, T, BITS) { \
    n += self->LEN; \
    if (self->CAP < n) { \
      if (n < SEQ_MIN_CAP) n = SEQ_MIN_CAP; \
      else if (!ISPOW2(n)) n = pow2_next##BITS(n); \
      seq_meth(ID, realloc)(self, n); \
    } \
  }

#define SEQ_DECL_shrink(SCOPE, ID, T, BITS) \
  SCOPE void \
  seq_meth(ID, shrink)(seq_tyof(ID) *__restrict self, UTY(BITS) n)

#define SEQ_IMPL_shrink(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_shrink(SCOPE, ID, T, BITS) { \
    n = self->CAP - n; \
    if (n < self->CAP && ISPOW2(n)) { \
      seq_meth(ID, realloc)(self, n); \
    } \
  }

#define SEQ_DECL_trim(SCOPE, ID, T, BITS) \
  SCOPE void \
  seq_meth(ID, trim)(seq_tyof(ID) *__restrict self)

#define SEQ_IMPL_trim(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_trim(SCOPE, ID, T, BITS) { \
    if (self->LEN == 0 && self->CAP) seq_meth(ID, dtor)(self, nil); \
    else if (self->CAP > self->LEN) { \
      seq_meth(ID, realloc)(self, self->LEN); \
    } \
  }

#define SEQ_DECL_pushn(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, pushn)(seq_tyof(ID) *__restrict self, UTY(BITS) n)

#define SEQ_IMPL_pushn(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_pushn(SCOPE, ID, T, BITS) { \
    T* it; \
    seq_meth(ID, grow)(self, n); \
    it = seq_meth(ID, end)(self); \
    self->LEN += n; \
    return it; \
  }

#define SEQ_DECL_unshiftn(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, unshiftn)(seq_tyof(ID) *__restrict self, UTY(BITS) n)

#define SEQ_IMPL_unshiftn(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_unshiftn(SCOPE, ID, T, BITS) { \
    T* it; \
    seq_meth(ID, grow)(self, n); \
    it = seq_meth(ID, begin)(self); \
    memmove( \
      it + n, \
      it, \
      (usize_t) seq_meth(ID, size)(self) * sizeof(T) \
    ); \
    self->LEN += n; \
    return it; \
  }

#define SEQ_DECL_putn(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, putn)(seq_tyof(ID) *__restrict self, UTY(BITS) idx, UTY(BITS) n)

#define SEQ_IMPL_putn(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_putn(SCOPE, ID, T, BITS) { \
    UTY(BITS) len; \
    if (idx > (len = seq_meth(ID, size)(self))) \
      return nil; \
    else if (idx == len) \
      return seq_meth(ID, pushn)(self, n); \
    else if (idx == 0) \
      return seq_meth(ID, unshiftn)(self, n); \
    else { \
      T* it; \
      seq_meth(ID, grow)(self, n); \
      it = seq_meth(ID, begin)(self) + idx; \
      memmove( \
        it + n, \
        it, \
        (usize_t) (len - idx) * sizeof(T) \
      ); \
      self->LEN += n; \
      return it; \
    } \
  }

#define SEQ_DECL_popn(SCOPE, ID, T, BITS) \
  SCOPE UTY(BITS) \
  seq_meth(ID, popn)(seq_tyof(ID) *__restrict self, UTY(BITS) n, T* out)

#define SEQ_IMPL_popn(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_popn(SCOPE, ID, T, BITS) { \
    UTY(BITS) len; \
    if ((len = seq_meth(ID, size)(self)) == 0) \
      return 0; \
    else { \
      if (n > len) n = len; \
      self->LEN -= n; \
      if (out != nil) { \
        memcpy(out, seq_meth(ID, end)(self), (usize_t) n * sizeof(T)); \
      } \
      return n; \
    } \
  }

#define SEQ_DECL_shiftn(SCOPE, ID, T, BITS) \
  SCOPE UTY(BITS) \
  seq_meth(ID, shiftn)(seq_tyof(ID) *__restrict self, UTY(BITS) n, T* out)

#define SEQ_IMPL_shiftn(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_shiftn(SCOPE, ID, T, BITS) { \
    UTY(BITS) len; \
    if ((len = seq_meth(ID, size)(self)) == 0) \
      return 0; \
    else { \
      T* it; \
      if (n > len) n = len; \
      it = seq_meth(ID, begin)(self); \
      self->LEN -= n; \
      if (out != nil) { \
        memcpy(out, it, (usize_t) n * sizeof(T)); \
      } \
      memmove( \
        it, \
        it + n, \
        (usize_t) (len - n) * sizeof(T) \
      ); \
      return n; \
    } \
  }

#define SEQ_DECL_removen(SCOPE, ID, T, BITS) \
  SCOPE UTY(BITS) \
  seq_meth(ID, removen)(seq_tyof(ID) *__restrict self, UTY(BITS) idx, \
    UTY(BITS) n, T* out)

#define SEQ_IMPL_removen(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_removen(SCOPE, ID, T, BITS) { \
    UTY(BITS) len; \
    if (idx >= (len = seq_meth(ID, size)(self))) \
      return 0; \
    else if (idx == len - 1) \
      return seq_meth(ID, popn)(self, n, out); \
    else if (idx == 0) \
      return seq_meth(ID, shiftn)(self, n, out); \
    else { \
      T* it; \
      if (n > len) n = len; \
      it = seq_meth(ID, at)(self, idx); \
      self->LEN -= n; \
      if (out != nil) { \
        memcpy(out, it, (usize_t) n * sizeof(T)); \
      } \
      memmove( \
        it, \
        it + n, \
        (usize_t) (len - idx - n) * sizeof(T) \
      ); \
      return n; \
    } \
  }

#define SEQ_DECL_push(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, push)(seq_tyof(ID) *__restrict self)

#define SEQ_IMPL_push(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_push(SCOPE, ID, T, BITS) { \
    return seq_meth(ID, pushn)(self, 1); \
  }

#define SEQ_DECL_unshift(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, unshift)(seq_tyof(ID) *__restrict self)

#define SEQ_IMPL_unshift(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_unshift(SCOPE, ID, T, BITS) { \
    return seq_meth(ID, unshiftn)(self, 1); \
  }

#define SEQ_DECL_put(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, put)(seq_tyof(ID) *__restrict self, UTY(BITS) idx)

#define SEQ_IMPL_put(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_put(SCOPE, ID, T, BITS) { \
    return seq_meth(ID, putn)(self, idx, 1); \
  }

#define SEQ_DECL_pop(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  seq_meth(ID, pop)(seq_tyof(ID) *__restrict self, T *__restrict out)

#define SEQ_IMPL_pop(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_pop(SCOPE, ID, T, BITS) { \
    return seq_meth(ID, popn)(self, 1, out) == 1; \
  }

#define SEQ_DECL_shift(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  seq_meth(ID, shift)(seq_tyof(ID) *__restrict self, T *__restrict out)

#define SEQ_IMPL_shift(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_shift(SCOPE, ID, T, BITS) { \
    return seq_meth(ID, shiftn)(self, 1, out) == 1; \
  }

#define SEQ_DECL_remove(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  seq_meth(ID, remove)(seq_tyof(ID) *__restrict self, UTY(BITS) idx, T* out)

#define SEQ_IMPL_remove(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_remove(SCOPE, ID, T, BITS) { \
    return seq_meth(ID, removen)(self, idx, 1, out) == 1; \
  }

#define SEQ_DECL_pushncpy(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, pushncpy)(seq_tyof(ID) *__restrict self, T *items, UTY(BITS) n)

#define SEQ_IMPL_pushncpy(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_pushncpy(SCOPE, ID, T, BITS) { \
    T *it; \
    it = seq_meth(ID, pushn)(self, n); \
    memcpy(it, items, (usize_t) n * sizeof(T)); \
    return it; \
  }

#define SEQ_DECL_unshiftncpy(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, unshiftncpy)(seq_tyof(ID) *__restrict self, T *items, UTY(BITS) n)

#define SEQ_IMPL_unshiftncpy(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_unshiftncpy(SCOPE, ID, T, BITS) { \
    T *it; \
    it = seq_meth(ID, unshiftn)(self, n); \
    memcpy(it, items, (usize_t) n * sizeof(T)); \
    return it; \
  }

#define SEQ_DECL_putncpy(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, putncpy)(seq_tyof(ID) *__restrict self, UTY(BITS) idx, T *items, \
    UTY(BITS) n)

#define SEQ_IMPL_putncpy(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_putncpy(SCOPE, ID, T, BITS) { \
    T *it; \
    if ((it = seq_meth(ID, putn)(self, idx, n)) == nil) return nil; \
    memcpy(it, items, (usize_t) n * sizeof(T)); \
    return it; \
  }

#define SEQ_DECL_pushcpy(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, pushcpy)(seq_tyof(ID) *__restrict self, T item)

#define SEQ_IMPL_pushcpy(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_pushcpy(SCOPE, ID, T, BITS) { \
    T *it; \
    it = seq_meth(ID, push)(self); \
    *it = item; \
    return it; \
  }

#define SEQ_DECL_unshiftcpy(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, unshiftcpy)(seq_tyof(ID) *__restrict self, T item)

#define SEQ_IMPL_unshiftcpy(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_unshiftcpy(SCOPE, ID, T, BITS) { \
    T *it; \
    it = seq_meth(ID, unshift)(self); \
    *it = item; \
    return it; \
  }

#define SEQ_DECL_putcpy(SCOPE, ID, T, BITS) \
  SCOPE T* \
  seq_meth(ID, putcpy)(seq_tyof(ID) *__restrict self, UTY(BITS) idx, T item)

#define SEQ_IMPL_putcpy(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_putcpy(SCOPE, ID, T, BITS) { \
    T *it; \
    if ((it = seq_meth(ID, put)(self, idx)) == nil) \
      return nil; \
    *it = item; \
    return it; \
  }

#define SEQ_DECL(SCOPE, ID, T, BITS) \
  SEQ_DECL_ctor(SCOPE, ID, T, BITS); \
  SEQ_DECL_dtor(SCOPE, ID, T, BITS); \
  SEQ_DECL_size(SCOPE, ID, T, BITS); \
  SEQ_DECL_begin(SCOPE, ID, T, BITS); \
  SEQ_DECL_end(SCOPE, ID, T, BITS); \
  SEQ_DECL_at(SCOPE, ID, T, BITS); \
  SEQ_DECL_realloc(SCOPE, ID, T, BITS); \
  SEQ_DECL_ensure(SCOPE, ID, T, BITS); \
  SEQ_DECL_grow(SCOPE, ID, T, BITS); \
  SEQ_DECL_shrink(SCOPE, ID, T, BITS); \
  SEQ_DECL_trim(SCOPE, ID, T, BITS); \
  SEQ_DECL_pushn(SCOPE, ID, T, BITS); \
  SEQ_DECL_unshiftn(SCOPE, ID, T, BITS); \
  SEQ_DECL_putn(SCOPE, ID, T, BITS); \
  SEQ_DECL_popn(SCOPE, ID, T, BITS); \
  SEQ_DECL_shiftn(SCOPE, ID, T, BITS); \
  SEQ_DECL_removen(SCOPE, ID, T, BITS); \
  SEQ_DECL_push(SCOPE, ID, T, BITS); \
  SEQ_DECL_unshift(SCOPE, ID, T, BITS); \
  SEQ_DECL_put(SCOPE, ID, T, BITS); \
  SEQ_DECL_pop(SCOPE, ID, T, BITS); \
  SEQ_DECL_shift(SCOPE, ID, T, BITS); \
  SEQ_DECL_remove(SCOPE, ID, T, BITS); \
  SEQ_DECL_pushncpy(SCOPE, ID, T, BITS); \
  SEQ_DECL_unshiftncpy(SCOPE, ID, T, BITS); \
  SEQ_DECL_putncpy(SCOPE, ID, T, BITS); \
  SEQ_DECL_pushcpy(SCOPE, ID, T, BITS); \
  SEQ_DECL_unshiftcpy(SCOPE, ID, T, BITS); \
  SEQ_DECL_putcpy(SCOPE, ID, T, BITS)

#define SEQ_IMPL_NX( \
    SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP, \
    ctor, dtor, \
    size, \
    begin, end, at, \
    realloc, ensure, grow, shrink, trim, \
    pushn, unshiftn, putn, \
    popn, shiftn, removen, \
    push, unshift, put, \
    pop, shift, remove, \
    pushncpy, unshiftncpy, putncpy, \
    pushcpy, unshiftcpy, putcpy) \
  ctor(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  dtor(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  size(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  begin(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  end(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  at(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  realloc(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  ensure(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  grow(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  shrink(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  trim(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  pushn(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  unshiftn(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  putn(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  popn(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  shiftn(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  removen(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  push(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  unshift(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  put(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  pop(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  shift(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  remove(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  pushncpy(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  unshiftncpy(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  putncpy(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  pushcpy(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  unshiftcpy(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  putcpy(SCOPE FORCEINLINE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP)


#define SEQ_IMPL( \
    SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP, \
    size, \
    begin, end, \
    pushn, unshiftn, \
    popn, shiftn) \
  SEQ_IMPL_NX( \
    SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP, \
    SEQ_IMPL_ctor, \
    SEQ_IMPL_dtor, \
    size, \
    begin, \
    end, \
    SEQ_IMPL_at, \
    SEQ_IMPL_realloc, \
    SEQ_IMPL_ensure, \
    SEQ_IMPL_grow, \
    SEQ_IMPL_shrink, \
    SEQ_IMPL_trim, \
    pushn, \
    unshiftn, \
    SEQ_IMPL_putn, \
    popn, \
    shiftn, \
    SEQ_IMPL_removen, \
    SEQ_IMPL_push, \
    SEQ_IMPL_unshift, \
    SEQ_IMPL_put, \
    SEQ_IMPL_pop, \
    SEQ_IMPL_shift, \
    SEQ_IMPL_remove, \
    SEQ_IMPL_pushncpy, \
    SEQ_IMPL_unshiftncpy, \
    SEQ_IMPL_putncpy, \
    SEQ_IMPL_pushcpy, \
    SEQ_IMPL_unshiftcpy, \
    SEQ_IMPL_putcpy)

#endif /* !__DS_SEQ_H */
