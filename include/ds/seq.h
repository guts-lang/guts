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

#define SEQ_GROW_STRICT(N, BITS) (N)
#define SEQ_GROW_POW2(N, BITS) ( \
    (N) < SEQ_MIN_CAP \
      ? SEQ_MIN_CAP \
      : (ISPOW2(N) ? (N) : PP_JOIN(pow2_next, BITS)(N)) \
  )

#define SEQ_TY(ID) PP_JOIN(ID, _t)
#define SEQ_SELF_TY(ID) SEQ_TY(ID) *__restrict
#define SEQ_M(ID, M) PP_JOIN(PP_JOIN(ID, _), M)
#define SEQ_IT(VAL, SEQ, ID) \
  ((VAL) = SEQ_M(ID, begin)(SEQ)), \
  ((VAL) != SEQ_M(ID, end)(SEQ)), \
  (++(VAL))
#define seqit_kv(IDX, VAL, SEQ, ID) \
  ((VAL) = SEQ_M(ID, begin)(SEQ), (IDX = 0)), \
  ((VAL) != SEQ_M(ID, end)(SEQ)), \
  (++(VAL), ++(IDX))

#define SEQ_DECL_ctor(SCOPE, ID, T, BITS) \
  SCOPE void \
  SEQ_M(ID, ctor)(SEQ_SELF_TY(ID) self)

#define SEQ_IMPL_ctor(SCOPE, ID, T, BITS) \
  SEQ_DECL_ctor(SCOPE, ID, T, BITS) { \
    init(self, SEQ_TY(ID)); \
  }

#define SEQ_DECL_dtor(SCOPE, ID, T, BITS) \
  SCOPE void \
  SEQ_M(ID, dtor)(SEQ_SELF_TY(ID) self, void (*idtor)(T *))

#define SEQ_IMPL_dtor(SCOPE, ID, T, BITS, FREE) \
  SEQ_DECL_dtor(SCOPE, ID, T, BITS) { \
    if (self->buf) { \
      if (idtor) { \
        T *item; \
        foreach (SEQ_IT(item, self, ID)) { \
          idtor(item); \
        } \
      } \
      FREE(self->buf); \
    } \
    init(self, SEQ_TY(ID)); \
  }

#define SEQ_DECL_size(SCOPE, ID, T, BITS) \
  SCOPE UTY(BITS) \
  SEQ_M(ID, size)(SEQ_SELF_TY(ID) self)

#define SEQ_IMPL_size(SCOPE, ID, T, BITS) \
  SEQ_DECL_size(SCOPE, ID, T, BITS) { \
    return self->len; \
  }

#define SEQ_DECL_begin(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, begin)(SEQ_SELF_TY(ID) self)

#define SEQ_IMPL_begin(SCOPE, ID, T, BITS) \
  SEQ_DECL_begin(SCOPE, ID, T, BITS) { \
    return self->buf; \
  }

#define SEQ_DECL_end(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, end)(SEQ_SELF_TY(ID) self)

#define SEQ_IMPL_end(SCOPE, ID, T, BITS) \
  SEQ_DECL_end(SCOPE, ID, T, BITS) { \
    return self->buf + self->len; \
  }

#define SEQ_DECL_at(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, at)(SEQ_SELF_TY(ID) self, UTY(BITS) idx)

#define SEQ_IMPL_at(SCOPE, ID, T, BITS) \
  SEQ_DECL_at(SCOPE, ID, T, BITS) { \
    return SEQ_M(ID, begin)(self) + idx; \
  }

#define SEQ_DECL_realloc(SCOPE, ID, T, BITS) \
  SCOPE void \
  SEQ_M(ID, realloc)(SEQ_SELF_TY(ID) self, UTY(BITS) n)

#define SEQ_IMPL_realloc(SCOPE, ID, T, BITS, REALLOC) \
  SEQ_DECL_realloc(SCOPE, ID, T, BITS) { \
    if (self->cap != n) { \
      void *buf; \
      if ((buf = REALLOC(self->buf, (usize_t) n * sizeof(T))) == nil) { \
        THROW(ex_errno(ERRLVL_FATAL, ENOMEM, nil)); \
      } \
      self->buf = buf; \
      if (self->len > (self->cap = n)) self->len = n; \
    } \
  }

#define SEQ_DECL_ensure(SCOPE, ID, T, BITS) \
  SCOPE void \
  SEQ_M(ID, ensure)(SEQ_SELF_TY(ID) self, UTY(BITS) n)

#define SEQ_IMPL_ensure(SCOPE, ID, T, BITS, GROW) \
  SEQ_DECL_ensure(SCOPE, ID, T, BITS) { \
    if (n && self->cap < ++n) { \
      SEQ_M(ID, realloc)(self, (UTY(BITS)) GROW(n, BITS)); \
    } \
  }

#define SEQ_DECL_grow(SCOPE, ID, T, BITS) \
  SCOPE void \
  SEQ_M(ID, grow)(SEQ_SELF_TY(ID) self, UTY(BITS) n)

#define SEQ_IMPL_grow(SCOPE, ID, T, BITS, GROW) \
  SEQ_DECL_grow(SCOPE, ID, T, BITS) { \
    n += self->len; \
    if (n && self->cap < ++n) { \
      SEQ_M(ID, realloc)(self, (UTY(BITS)) GROW(n, BITS)); \
    } \
  }

#define SEQ_DECL_shrink(SCOPE, ID, T, BITS) \
  SCOPE void \
  SEQ_M(ID, shrink)(SEQ_SELF_TY(ID) self, UTY(BITS) n)

#define SEQ_IMPL_shrink(SCOPE, ID, T, BITS) \
  SEQ_DECL_shrink(SCOPE, ID, T, BITS) { \
    n = self->cap - n; \
    if (n < self->cap && ISPOW2(n)) { \
      SEQ_M(ID, realloc)(self, n); \
    } \
  }

#define SEQ_DECL_trim(SCOPE, ID, T, BITS) \
  SCOPE void \
  SEQ_M(ID, trim)(SEQ_SELF_TY(ID) self)

#define SEQ_IMPL_trim(SCOPE, ID, T, BITS) \
  SEQ_DECL_trim(SCOPE, ID, T, BITS) { \
    if (self->len == 0 && self->cap) SEQ_M(ID, dtor)(self, nil); \
    else if (self->cap > self->len) { \
      SEQ_M(ID, realloc)(self, self->len); \
    } \
  }

#define SEQ_DECL_pushn(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, pushn)(SEQ_SELF_TY(ID) self, UTY(BITS) n)

#define SEQ_IMPL_pushn(SCOPE, ID, T, BITS) \
  SEQ_DECL_pushn(SCOPE, ID, T, BITS) { \
    T* it; \
    SEQ_M(ID, grow)(self, n); \
    it = SEQ_M(ID, end)(self); \
    self->len += n; \
    return it; \
  }

#define SEQ_DECL_pushncpy(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, pushncpy)(SEQ_SELF_TY(ID) self, T __const *items, UTY(BITS) n)

#define SEQ_IMPL_pushncpy(SCOPE, ID, T, BITS) \
  SEQ_DECL_pushncpy(SCOPE, ID, T, BITS) { \
    if (n == 0) \
      return nil; \
    else { \
      T *it; \
      it = SEQ_M(ID, pushn)(self, n); \
      memcpy(it, items, (usize_t) n * sizeof(T)); \
      return it; \
    } \
  }

#define SEQ_DECL_push(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, push)(SEQ_SELF_TY(ID) self)

#define SEQ_IMPL_push(SCOPE, ID, T, BITS) \
  SEQ_DECL_push(SCOPE, ID, T, BITS) { \
    return SEQ_M(ID, pushn)(self, 1); \
  }

#define SEQ_DECL_pushcpy(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, pushcpy)(SEQ_SELF_TY(ID) self, T item)

#define SEQ_IMPL_pushcpy(SCOPE, ID, T, BITS) \
  SEQ_DECL_pushcpy(SCOPE, ID, T, BITS) { \
    T *it; \
    it = SEQ_M(ID, push)(self); \
    *it = item; \
    return it; \
  }

#define SEQ_DECL_unshiftn(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, unshiftn)(SEQ_SELF_TY(ID) self, UTY(BITS) n)

#define SEQ_IMPL_unshiftn(SCOPE, ID, T, BITS) \
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
    } \
    self->len += n; \
    return it; \
  }

#define SEQ_DECL_unshiftncpy(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, unshiftncpy)(SEQ_SELF_TY(ID) self, T __const *items, \
    UTY(BITS) n)

#define SEQ_IMPL_unshiftncpy(SCOPE, ID, T, BITS) \
  SEQ_DECL_unshiftncpy(SCOPE, ID, T, BITS) { \
    if (n == 0) \
      return nil; \
    else { \
      T *it; \
      it = SEQ_M(ID, unshiftn)(self, n); \
      memcpy(it, items, (usize_t) n * sizeof(T)); \
      return it; \
    } \
  }

#define SEQ_DECL_unshift(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, unshift)(SEQ_SELF_TY(ID) self)

#define SEQ_IMPL_unshift(SCOPE, ID, T, BITS) \
  SEQ_DECL_unshift(SCOPE, ID, T, BITS) { \
    return SEQ_M(ID, unshiftn)(self, 1); \
  }

#define SEQ_DECL_unshiftcpy(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, unshiftcpy)(SEQ_SELF_TY(ID) self, T item)

#define SEQ_IMPL_unshiftcpy(SCOPE, ID, T, BITS) \
  SEQ_DECL_unshiftcpy(SCOPE, ID, T, BITS) { \
    T *it; \
    it = SEQ_M(ID, unshift)(self); \
    *it = item; \
    return it; \
  }

#define SEQ_DECL_putn(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, putn)(SEQ_SELF_TY(ID) self, UTY(BITS) idx, UTY(BITS) n)

#define SEQ_IMPL_putn(SCOPE, ID, T, BITS) \
  SEQ_DECL_putn(SCOPE, ID, T, BITS) { \
    UTY(BITS) len; \
    if (idx > (len = SEQ_M(ID, size)(self))) \
      return nil; \
    else if (idx == len) \
      return SEQ_M(ID, pushn)(self, n); \
    else if (idx == 0) \
      return SEQ_M(ID, unshiftn)(self, n); \
    else { \
      T* it; \
      SEQ_M(ID, grow)(self, n); \
      it = SEQ_M(ID, begin)(self) + idx; \
      memmove( \
        it + n, \
        it, \
        (usize_t) (len - idx + 1) * sizeof(T) \
      ); \
      self->len += n; \
      return it; \
    } \
  }

#define SEQ_DECL_putncpy(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, putncpy)(SEQ_SELF_TY(ID) self, UTY(BITS) idx, \
    T __const *items, UTY(BITS) n)

#define SEQ_IMPL_putncpy(SCOPE, ID, T, BITS) \
  SEQ_DECL_putncpy(SCOPE, ID, T, BITS) { \
    if (n == 0) \
      return nil; \
    else { \
      T *it; \
      if ((it = SEQ_M(ID, putn)(self, idx, n)) == nil) return nil; \
      memcpy(it, items, (usize_t) n * sizeof(T)); \
      return it; \
    } \
  }

#define SEQ_DECL_put(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, put)(SEQ_SELF_TY(ID) self, UTY(BITS) idx)

#define SEQ_IMPL_put(SCOPE, ID, T, BITS) \
  SEQ_DECL_put(SCOPE, ID, T, BITS) { \
    return SEQ_M(ID, putn)(self, idx, 1); \
  }

#define SEQ_DECL_putcpy(SCOPE, ID, T, BITS) \
  SCOPE T* \
  SEQ_M(ID, putcpy)(SEQ_SELF_TY(ID) self, UTY(BITS) idx, T item)

#define SEQ_IMPL_putcpy(SCOPE, ID, T, BITS) \
  SEQ_DECL_putcpy(SCOPE, ID, T, BITS) { \
    T *it; \
    if ((it = SEQ_M(ID, put)(self, idx)) == nil) \
      return nil; \
    *it = item; \
    return it; \
  }

#define SEQ_DECL_popn(SCOPE, ID, T, BITS) \
  SCOPE UTY(BITS) \
  SEQ_M(ID, popn)(SEQ_SELF_TY(ID) self, UTY(BITS) n, T* out)

#define SEQ_IMPL_popn(SCOPE, ID, T, BITS) \
  SEQ_DECL_popn(SCOPE, ID, T, BITS) { \
    UTY(BITS) len; \
    if ((len = SEQ_M(ID, size)(self)) == 0) \
      return 0; \
    else { \
      if (n > len) n = len; \
      self->len -= n; \
      if (out != nil) { \
        memcpy(out, SEQ_M(ID, end)(self), (usize_t) n * sizeof(T)); \
      } \
      return n; \
    } \
  }

#define SEQ_DECL_pop(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  SEQ_M(ID, pop)(SEQ_SELF_TY(ID) self, T *__restrict out)

#define SEQ_IMPL_pop(SCOPE, ID, T, BITS) \
  SEQ_DECL_pop(SCOPE, ID, T, BITS) { \
    return SEQ_M(ID, popn)(self, 1, out) == 1; \
  }

#define SEQ_DECL_shiftn(SCOPE, ID, T, BITS) \
  SCOPE UTY(BITS) \
  SEQ_M(ID, shiftn)(SEQ_SELF_TY(ID) self, UTY(BITS) n, T* out)

#define SEQ_IMPL_shiftn(SCOPE, ID, T, BITS) \
  SEQ_DECL_shiftn(SCOPE, ID, T, BITS) { \
    UTY(BITS) len; \
    if ((len = SEQ_M(ID, size)(self)) == 0) \
      return 0; \
    else { \
      T* it; \
      if (n > len) n = len; \
      it = SEQ_M(ID, begin)(self); \
      self->len -= n; \
      if (out != nil) { \
        memcpy(out, it, (usize_t) n * sizeof(T)); \
      } \
      memmove( \
        it, \
        it + n, \
        (usize_t) (len - n + 1) * sizeof(T) \
      ); \
      return n; \
    } \
  }

#define SEQ_DECL_shift(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  SEQ_M(ID, shift)(SEQ_SELF_TY(ID) self, T *__restrict out)

#define SEQ_IMPL_shift(SCOPE, ID, T, BITS) \
  SEQ_DECL_shift(SCOPE, ID, T, BITS) { \
    return SEQ_M(ID, shiftn)(self, 1, out) == 1; \
  }

#define SEQ_DECL_removen(SCOPE, ID, T, BITS) \
  SCOPE UTY(BITS) \
  SEQ_M(ID, removen)(SEQ_SELF_TY(ID) self, UTY(BITS) idx, \
    UTY(BITS) n, T* out)

#define SEQ_IMPL_removen(SCOPE, ID, T, BITS) \
  SEQ_DECL_removen(SCOPE, ID, T, BITS) { \
    UTY(BITS) len; \
    if (idx >= (len = SEQ_M(ID, size)(self))) \
      return 0; \
    else if (idx == len - 1) \
      return SEQ_M(ID, popn)(self, n, out); \
    else if (idx == 0) \
      return SEQ_M(ID, shiftn)(self, n, out); \
    else { \
      T* it; \
      if (n > len) n = len; \
      it = SEQ_M(ID, at)(self, idx); \
      self->len -= n; \
      if (out != nil) { \
        memcpy(out, it, (usize_t) n * sizeof(T)); \
      } \
      memmove( \
        it, \
        it + n, \
        (usize_t) (len - idx - n + 1) * sizeof(T) \
      ); \
      return n; \
    } \
  }

#define SEQ_DECL_remove(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  SEQ_M(ID, remove)(SEQ_SELF_TY(ID) self, UTY(BITS) idx, T* out)

#define SEQ_IMPL_remove(SCOPE, ID, T, BITS) \
  SEQ_DECL_remove(SCOPE, ID, T, BITS) { \
    return SEQ_M(ID, removen)(self, idx, 1, out) == 1; \
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
  SEQ_DECL_pushncpy(SCOPE, ID, T, BITS); \
  SEQ_DECL_push(SCOPE, ID, T, BITS); \
  SEQ_DECL_pushcpy(SCOPE, ID, T, BITS); \
  SEQ_DECL_unshiftn(SCOPE, ID, T, BITS); \
  SEQ_DECL_unshiftncpy(SCOPE, ID, T, BITS); \
  SEQ_DECL_unshift(SCOPE, ID, T, BITS); \
  SEQ_DECL_unshiftcpy(SCOPE, ID, T, BITS); \
  SEQ_DECL_putn(SCOPE, ID, T, BITS); \
  SEQ_DECL_putncpy(SCOPE, ID, T, BITS); \
  SEQ_DECL_put(SCOPE, ID, T, BITS); \
  SEQ_DECL_putcpy(SCOPE, ID, T, BITS); \
  SEQ_DECL_popn(SCOPE, ID, T, BITS); \
  SEQ_DECL_pop(SCOPE, ID, T, BITS); \
  SEQ_DECL_shiftn(SCOPE, ID, T, BITS); \
  SEQ_DECL_shift(SCOPE, ID, T, BITS); \
  SEQ_DECL_removen(SCOPE, ID, T, BITS); \
  SEQ_DECL_remove(SCOPE, ID, T, BITS)

#define SEQ_IMPL_NX( \
    SCOPE, ID, T, BITS, GROW, REALLOC, FREE, \
    ctor, dtor, \
    size, \
    begin, end, at, \
    realloc, ensure, grow, shrink, trim, \
    pushn, pushncpy, push, pushcpy, \
    unshiftn, unshiftncpy, unshift, unshiftcpy, \
    putn, putncpy, put, putcpy, \
    popn, pop, \
    shiftn, shift, \
    removen, remove) \
  size(SCOPE FORCEINLINE, ID, T, BITS) \
  begin(SCOPE FORCEINLINE, ID, T, BITS) \
  end(SCOPE FORCEINLINE, ID, T, BITS) \
  at(SCOPE FORCEINLINE, ID, T, BITS) \
  ctor(SCOPE FORCEINLINE, ID, T, BITS) \
  dtor(SCOPE FORCEINLINE, ID, T, BITS, FREE) \
  realloc(SCOPE FORCEINLINE, ID, T, BITS, REALLOC) \
  ensure(SCOPE FORCEINLINE, ID, T, BITS, GROW) \
  grow(SCOPE FORCEINLINE, ID, T, BITS, GROW) \
  shrink(SCOPE FORCEINLINE, ID, T, BITS) \
  trim(SCOPE FORCEINLINE, ID, T, BITS) \
  pushn(SCOPE FORCEINLINE, ID, T, BITS) \
  pushncpy(SCOPE FORCEINLINE, ID, T, BITS) \
  push(SCOPE FORCEINLINE, ID, T, BITS) \
  pushcpy(SCOPE FORCEINLINE, ID, T, BITS) \
  unshiftn(SCOPE FORCEINLINE, ID, T, BITS) \
  unshiftncpy(SCOPE FORCEINLINE, ID, T, BITS) \
  unshift(SCOPE FORCEINLINE, ID, T, BITS) \
  unshiftcpy(SCOPE FORCEINLINE, ID, T, BITS) \
  putn(SCOPE FORCEINLINE, ID, T, BITS) \
  putncpy(SCOPE FORCEINLINE, ID, T, BITS) \
  put(SCOPE FORCEINLINE, ID, T, BITS) \
  putcpy(SCOPE FORCEINLINE, ID, T, BITS) \
  popn(SCOPE FORCEINLINE, ID, T, BITS) \
  pop(SCOPE FORCEINLINE, ID, T, BITS) \
  shiftn(SCOPE FORCEINLINE, ID, T, BITS) \
  shift(SCOPE FORCEINLINE, ID, T, BITS) \
  removen(SCOPE FORCEINLINE, ID, T, BITS) \
  remove(SCOPE FORCEINLINE, ID, T, BITS)


#define SEQ_IMPL( \
    SCOPE, ID, T, BITS, GROW, REALLOC, FREE, \
    size, \
    begin, end, \
    pushn, unshiftn, \
    popn, shiftn) \
  SEQ_IMPL_NX( \
    SCOPE, ID, T, BITS, GROW, REALLOC, FREE, \
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
    SEQ_IMPL_pushncpy, \
    SEQ_IMPL_push, \
    SEQ_IMPL_pushcpy, \
    unshiftn, \
    SEQ_IMPL_unshiftncpy, \
    SEQ_IMPL_unshift, \
    SEQ_IMPL_unshiftcpy, \
    SEQ_IMPL_putn, \
    SEQ_IMPL_putncpy, \
    SEQ_IMPL_put, \
    SEQ_IMPL_putcpy, \
    popn, \
    SEQ_IMPL_pop, \
    shiftn, \
    SEQ_IMPL_shift, \
    SEQ_IMPL_removen, \
    SEQ_IMPL_remove)

#endif /* !__DS_SEQ_H */
