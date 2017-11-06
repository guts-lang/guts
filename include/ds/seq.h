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
  ((VAL) = SEQ_M(ID, begin)(SEQ), (IDX) = 0), \
  ((VAL) != SEQ_M(ID, end)(SEQ)), \
  (++(VAL), ++(IDX))

#define trait_ty(ID) PP_JOIN(ID, _t)
#define trait_mem(ID, MEM) PP_JOIN(PP_JOIN(ID, _), MEM)
#define trait_prototype(ID, MEM, RTY, ARGS) RTY trait_mem(ID, MEM) ARGS
#define trait_memdecl_fn(TRAIT, MEM) PP_JOIN(PP_JOIN(TRAIT, _decl_), MEM)
#define trait_decl_fn(TRAIT) PP_JOIN(TRAIT, _decl)
#define trait_memimpl_fn(TRAIT, MEM) PP_JOIN(PP_JOIN(TRAIT, _body_), MEM)
#define trait_memdecl_as(TRAIT, MEM, AS, ID, ...) \
  PP_MCALL(trait_memdecl_fn(TRAIT, MEM), AS, ID, __VA_ARGS__)
#define trait_memdecl(TRAIT, MEM, ID, ...) \
  trait_memdecl_as(TRAIT, MEM, MEM, ID, __VA_ARGS__)
#define trait_memimpl_as(TRAIT, MEM, AS, ID, ...) \
  trait_memdecl_as(TRAIT, MEM, AS, ID, __VA_ARGS__) \
  PP_MCALL(trait_memimpl_fn(TRAIT, MEM), ID, __VA_ARGS__)
#define trait_memimpl(TRAIT, MEM, ID, ...) \
  trait_memimpl_as(TRAIT, MEM, MEM, ID, __VA_ARGS__)
#define trait_decl(TRAIT, SCOPE, ID, ...) \
  PP_MCALL(trait_decl_fn(TRAIT), SCOPE, ID, __VA_ARGS__)

#define seq_decl_ctor(AS, ID, T, BITS) \
  trait_prototype(ID, AS, void, (trait_ty(ID) *self))
#define seq_body_ctor(ID, T, BITS) { \
    init(self, SEQ_TY(ID)); \
  }
#define seq_impl_ctor(SCOPE, ID, T, BITS, BODY_FN) \
  SCOPE FORCEINLINE seq_decl_ctor(ctor, ID, T, BITS)

#define seq_decl(SCOPE, ID, T, BITS) \
  SCOPE seq_decl_ctor(ctor, ID, T, BITS)

#define seq_impl(SCOPE, ID, T, BITS) \
  SCOPE FORCEINLINE seq_decl_ctor(ctor, ID, T, BITS) seq_body_ctor(ID, T, BITS)

typedef struct {
  i32_t dummy;
} trait_ty(test);

__api__ trait_memdecl(seq, ctor, test);
trait_decl(seq, __api__, test, char_t, 8);
FORCEINLINE trait_memimpl(seq, ctor, test)

#define SEQ_DECL_ctor(AS, ID, T, BITS) \
  void SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self)

#define SEQ_BODY_ctor(ID, T, BITS, ...) { \
    init(self, SEQ_TY(ID)); \
  }

#define SEQ_DECL_dtor(AS, ID, T, BITS) \
  void SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, void (*idtor)(T *))

#define SEQ_BODY_dtor(ID, T, BITS, FREE) { \
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

#define SEQ_DECL_size(AS, ID, T, BITS) \
  UTY(BITS) SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self)

#define SEQ_BODY_size(ID, T, BITS, ...) { \
    return self->len; \
  }

#define SEQ_DECL_begin(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self)

#define SEQ_BODY_begin(ID, T, BITS, ...) { \
    return self->buf; \
  }

#define SEQ_DECL_end(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self)

#define SEQ_BODY_end(ID, T, BITS, ...) { \
    return self->buf + self->len; \
  }

#define SEQ_DECL_at(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) idx)

#define SEQ_BODY_at(ID, T, BITS, ...) { \
    return SEQ_M(ID, begin)(self) + idx; \
  }

#define SEQ_DECL_realloc(AS, ID, T, BITS) \
  void SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) n)

#define SEQ_BODY_realloc(ID, T, BITS, REALLOC) { \
    if (self->cap != n) { \
      void *buf; \
      if ((buf = REALLOC(self->buf, (usize_t) n * sizeof(T))) == nil) { \
        THROW(ex_errno(ERRLVL_FATAL, ENOMEM, nil)); \
      } \
      self->buf = buf; \
      if (self->len > (self->cap = n)) self->len = n; \
    } \
  }

#define SEQ_DECL_ensure(AS, ID, T, BITS) \
  void SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) n)

#define SEQ_BODY_ensure(ID, T, BITS, GROW) { \
    if (n && self->cap < ++n) { \
      SEQ_M(ID, realloc)(self, (UTY(BITS)) GROW(n, BITS)); \
    } \
  }

#define SEQ_DECL_grow(AS, ID, T, BITS) \
  void SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) n)

#define SEQ_BODY_grow(ID, T, BITS, GROW) { \
    n += self->len; \
    if (n && self->cap < ++n) { \
      SEQ_M(ID, realloc)(self, (UTY(BITS)) GROW(n, BITS)); \
    } \
  }

#define SEQ_DECL_shrink(AS, ID, T, BITS) \
  void SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) n)

#define SEQ_BODY_shrink(ID, T, BITS, ...) { \
    n = self->cap - n; \
    if (n < self->cap && ISPOW2(n)) { \
      SEQ_M(ID, realloc)(self, n); \
    } \
  }

#define SEQ_DECL_trim(AS, ID, T, BITS) \
  void SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self)

#define SEQ_BODY_trim(ID, T, BITS, ...) { \
    if (self->len == 0 && self->cap) SEQ_M(ID, dtor)(self, nil); \
    else if (self->cap > self->len) { \
      SEQ_M(ID, realloc)(self, self->len); \
    } \
  }

#define SEQ_DECL_pushn(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) n)

#define SEQ_BODY_pushn(ID, T, BITS, ...) { \
    if (n == 0) \
      return nil; \
    else { \
      T *it; \
      SEQ_M(ID, grow)(self, n); \
      it = SEQ_M(ID, end)(self); \
      self->len += n; \
      return it; \
    } \
  }

#define SEQ_DECL_pushncpy(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, T __const *items, UTY(BITS) n)

#define SEQ_BODY_pushncpy(ID, T, BITS, ...) { \
      T *it; \
      if ((it = SEQ_M(ID, pushn)(self, n)) == nil) \
        return nil; \
      memcpy(it, items, (usize_t) n * sizeof(T)); \
      return it; \
  }

#define SEQ_DECL_push(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self)

#define SEQ_BODY_push(ID, T, BITS, ...) { \
    return SEQ_M(ID, pushn)(self, 1); \
  }

#define SEQ_DECL_pushcpy(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, T item)

#define SEQ_BODY_pushcpy(ID, T, BITS, ...) { \
    T *it; \
    it = SEQ_M(ID, push)(self); \
    *it = item; \
    return it; \
  }

#define SEQ_DECL_unshiftn(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) n)

#define SEQ_BODY_unshiftn(ID, T, BITS, ...) { \
    if (n == 0) \
      return nil; \
    else { \
      UTY(BITS) len; \
      T *it; \
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
    } \
  }

#define SEQ_DECL_unshiftncpy(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, T __const *items, \
    UTY(BITS) n)

#define SEQ_BODY_unshiftncpy(ID, T, BITS, ...) { \
    T *it; \
    if ((it = SEQ_M(ID, unshiftn)(self, n)) == nil) \
      return nil; \
    memcpy(it, items, (usize_t) n * sizeof(T)); \
    return it; \
  }

#define SEQ_DECL_unshift(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self)

#define SEQ_BODY_unshift(ID, T, BITS, ...) { \
    return SEQ_M(ID, unshiftn)(self, 1); \
  }

#define SEQ_DECL_unshiftcpy(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, T item)

#define SEQ_BODY_unshiftcpy(ID, T, BITS, ...) { \
    T *it; \
    it = SEQ_M(ID, unshift)(self); \
    *it = item; \
    return it; \
  }

#define SEQ_DECL_putn(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) idx, UTY(BITS) n)

#define SEQ_BODY_putn(ID, T, BITS, ...) { \
    UTY(BITS) len; \
    if (idx == 0) \
      return SEQ_M(ID, unshiftn)(self, n); \
    else if (idx == (len = SEQ_M(ID, size)(self))) \
      return SEQ_M(ID, pushn)(self, n); \
    else if (n == 0 || idx > len) \
      return nil; \
    else { \
      T *it; \
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

#define SEQ_DECL_putncpy(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) idx, \
    T __const *items, UTY(BITS) n)

#define SEQ_BODY_putncpy(ID, T, BITS, ...) { \
      T *it; \
      if ((it = SEQ_M(ID, putn)(self, idx, n)) == nil) \
        return nil; \
      memcpy(it, items, (usize_t) n * sizeof(T)); \
      return it; \
  }

#define SEQ_DECL_put(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) idx)

#define SEQ_BODY_put(ID, T, BITS, ...) { \
    return SEQ_M(ID, putn)(self, idx, 1); \
  }

#define SEQ_DECL_putcpy(AS, ID, T, BITS) \
  T *SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) idx, T item)

#define SEQ_BODY_putcpy(ID, T, BITS, ...) { \
    T *it; \
    if ((it = SEQ_M(ID, put)(self, idx)) == nil) \
      return nil; \
    *it = item; \
    return it; \
  }

#define SEQ_DECL_popn(AS, ID, T, BITS) \
  UTY(BITS) SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) n, T *out)

#define SEQ_BODY_popn(ID, T, BITS, ...) { \
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

#define SEQ_DECL_pop(AS, ID, T, BITS) \
  bool_t SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, T *__restrict out)

#define SEQ_BODY_pop(ID, T, BITS, ...) { \
    return SEQ_M(ID, popn)(self, 1, out) == 1; \
  }

#define SEQ_DECL_shiftn(AS, ID, T, BITS) \
  UTY(BITS) SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) n, T *out)

#define SEQ_BODY_shiftn(ID, T, BITS, ...) { \
    UTY(BITS) len; \
    if ((len = SEQ_M(ID, size)(self)) == 0) \
      return 0; \
    else { \
      T *it; \
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

#define SEQ_DECL_shift(AS, ID, T, BITS) \
  bool_t SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, T *__restrict out)

#define SEQ_BODY_shift(ID, T, BITS, ...) { \
    return SEQ_M(ID, shiftn)(self, 1, out) == 1; \
  }

#define SEQ_DECL_removen(AS, ID, T, BITS) \
  UTY(BITS) SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) idx, \
    UTY(BITS) n, T *out)

#define SEQ_BODY_removen(ID, T, BITS, ...) { \
    UTY(BITS) len; \
    if (idx >= (len = SEQ_M(ID, size)(self))) \
      return 0; \
    else if (idx == len - 1) \
      return SEQ_M(ID, popn)(self, n, out); \
    else if (idx == 0) \
      return SEQ_M(ID, shiftn)(self, n, out); \
    else { \
      T *it; \
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

#define SEQ_DECL_remove(AS, ID, T, BITS) \
  bool_t SEQ_M(ID, AS)(SEQ_SELF_TY(ID) self, UTY(BITS) idx, T *out)

#define SEQ_BODY_remove(ID, T, BITS, ...) { \
    return SEQ_M(ID, removen)(self, idx, 1, out) == 1; \
  }

#define SEQ_DECL_BASE(SCOPE, ID, T, BITS) \
  SCOPE SEQ_DECL_ctor(ctor, ID, T, BITS); \
  SCOPE SEQ_DECL_dtor(dtor, ID, T, BITS); \
  SCOPE SEQ_DECL_size(size, ID, T, BITS); \
  SCOPE SEQ_DECL_begin(begin, ID, T, BITS); \
  SCOPE SEQ_DECL_end(end, ID, T, BITS); \
  SCOPE SEQ_DECL_at(at, ID, T, BITS)

#define SEQ_DECL_GROW(SCOPE, ID, T, BITS) \
  SCOPE SEQ_DECL_realloc(realloc, ID, T, BITS); \
  SCOPE SEQ_DECL_grow(grow, ID, T, BITS); \
  SCOPE SEQ_DECL_ensure(ensure, ID, T, BITS); \
  SCOPE SEQ_DECL_shrink(shrink, ID, T, BITS)

#define SEQ_DECL_TRIM(SCOPE, ID, T, BITS) \
  SCOPE SEQ_DECL_trim(trim, ID, T, BITS)

#define SEQ_DECL_PUSH(SCOPE, ID, T, BITS) \
  SCOPE SEQ_DECL_pushn(pushn, ID, T, BITS); \
  SCOPE SEQ_DECL_pushncpy(pushncpy, ID, T, BITS); \
  SCOPE SEQ_DECL_push(push, ID, T, BITS); \
  SCOPE SEQ_DECL_pushcpy(pushcpy, ID, T, BITS)

#define SEQ_DECL_UNSHIFT(SCOPE, ID, T, BITS) \
  SCOPE SEQ_DECL_unshiftn(unshiftn, ID, T, BITS); \
  SCOPE SEQ_DECL_unshiftncpy(unshiftncpy, ID, T, BITS); \
  SCOPE SEQ_DECL_unshift(unshift, ID, T, BITS); \
  SCOPE SEQ_DECL_unshiftcpy(unshiftcpy, ID, T, BITS)

#define SEQ_DECL_PUT(SCOPE, ID, T, BITS) \
  SCOPE SEQ_DECL_putn(putn, ID, T, BITS); \
  SCOPE SEQ_DECL_putncpy(putncpy, ID, T, BITS); \
  SCOPE SEQ_DECL_put(put, ID, T, BITS); \
  SCOPE SEQ_DECL_putcpy(putcpy, ID, T, BITS)

#define SEQ_DECL_POP(SCOPE, ID, T, BITS) \
  SCOPE SEQ_DECL_popn(popn, ID, T, BITS); \
  SCOPE SEQ_DECL_pop(pop, ID, T, BITS)

#define SEQ_DECL_SHIFT(SCOPE, ID, T, BITS) \
  SCOPE SEQ_DECL_shiftn(shiftn, ID, T, BITS); \
  SCOPE SEQ_DECL_shift(shift, ID, T, BITS)

#define SEQ_DECL_REMOVE(SCOPE, ID, T, BITS) \
  SCOPE SEQ_DECL_removen(removen, ID, T, BITS); \
  SCOPE SEQ_DECL_remove(remove, ID, T, BITS)

#define seq_impl(DECL, BODY, ID, T, BITS, ...) \
  PP_MCALL(PP_JOIN(SEQ_DECL_, DECL), DECL, ID, T, BITS) \
    BODY(ID, T, BITS, __VA_ARGS__)

#define seq_impl_as(DECL, BODY, AS, ID, T, BITS, ...) \
  PP_MCALL(PP_JOIN(SEQ_DECL_, DECL), AS, ID, T, BITS) \
    BODY(ID, T, BITS, __VA_ARGS__)

#define SEQ_DECL(SCOPE, ID, T, BITS) \
  SEQ_DECL_BASE(SCOPE, ID, T, BITS); \
  SEQ_DECL_GROW(SCOPE, ID, T, BITS); \
  SEQ_DECL_TRIM(SCOPE, ID, T, BITS); \
  SEQ_DECL_PUSH(SCOPE, ID, T, BITS); \
  SEQ_DECL_UNSHIFT(SCOPE, ID, T, BITS); \
  SEQ_DECL_PUT(SCOPE, ID, T, BITS); \
  SEQ_DECL_POP(SCOPE, ID, T, BITS); \
  SEQ_DECL_SHIFT(SCOPE, ID, T, BITS); \
  SEQ_DECL_REMOVE(SCOPE, ID, T, BITS)

#define SEQ_IMPL_BASE(SCOPE, ID, T, BITS, FREE, SIZE, BEGIN, END) \
  seq_impl(size, SIZE, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(begin, BEGIN, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(end, END, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(at, SEQ_BODY_at, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(ctor, SEQ_BODY_ctor, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(dtor, SEQ_BODY_dtor, SCOPE FORCEINLINE, ID, T, BITS, FREE)

#define SEQ_IMPL_GROW(SCOPE, ID, T, BITS, GROW, REALLOC) \
  seq_impl(realloc, SEQ_BODY_realloc, SCOPE FORCEINLINE, ID, T, BITS, REALLOC) \
  seq_impl(grow, SEQ_BODY_grow, SCOPE FORCEINLINE, ID, T, BITS, GROW) \
  seq_impl(ensure, SEQ_BODY_ensure, SCOPE FORCEINLINE, ID, T, BITS, GROW) \
  seq_impl(shrink, SEQ_BODY_shrink, SCOPE FORCEINLINE, ID, T, BITS)

#define SEQ_IMPL_TRIM(SCOPE, ID, T, BITS) \
  seq_impl(trim, SEQ_BODY_trim, SCOPE FORCEINLINE, ID, T, BITS)

#define SEQ_IMPL_PUSH(SCOPE, ID, T, BITS, PUSHN) \
  seq_impl(pushn, PUSHN, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(pushncpy, SEQ_BODY_pushncpy, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(push, SEQ_BODY_push, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(pushcpy, SEQ_BODY_pushcpy, SCOPE FORCEINLINE, ID, T, BITS)

#define SEQ_IMPL_UNSHIFT(SCOPE, ID, T, BITS, UNSHIFTN) \
  seq_impl(unshiftn, UNSHIFTN, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(unshiftncpy, SEQ_BODY_unshiftncpy, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(unshift, SEQ_BODY_unshift, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(unshiftcpy, SEQ_BODY_unshiftcpy, SCOPE FORCEINLINE, ID, T, BITS)

#define SEQ_IMPL_PUT(SCOPE, ID, T, BITS) \
  seq_impl(putn, SEQ_BODY_putn, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(putncpy, SEQ_BODY_putncpy, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(put, SEQ_BODY_put, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(putcpy, SEQ_BODY_putcpy, SCOPE FORCEINLINE, ID, T, BITS)

#define SEQ_IMPL_POP(SCOPE, ID, T, BITS, POPN) \
  seq_impl(popn, POPN, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(pop, SEQ_BODY_pop, SCOPE FORCEINLINE, ID, T, BITS)

#define SEQ_IMPL_SHIFT(SCOPE, ID, T, BITS, SHIFTN) \
  seq_impl(shiftn, SHIFTN, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(shift, SEQ_BODY_shift, SCOPE FORCEINLINE, ID, T, BITS)

#define SEQ_IMPL_REMOVE(SCOPE, ID, T, BITS) \
  seq_impl(removen, SEQ_BODY_removen, SCOPE FORCEINLINE, ID, T, BITS) \
  seq_impl(remove, SEQ_BODY_remove, SCOPE FORCEINLINE, ID, T, BITS)

#define SEQ_IMPL( \
    SCOPE, ID, T, BITS, GROW, REALLOC, FREE, \
    SIZE, BEGIN, END, \
    PUSHN, \
    UNSHIFTN, \
    POPN, \
    SHIFTN) \
  SEQ_IMPL_BASE(SCOPE, ID, T, BITS, FREE, SIZE, BEGIN, END) \
  SEQ_IMPL_GROW(SCOPE, ID, T, BITS, GROW, REALLOC) \
  SEQ_IMPL_TRIM(SCOPE, ID, T, BITS) \
  SEQ_IMPL_PUSH(SCOPE, ID, T, BITS, PUSHN) \
  SEQ_IMPL_UNSHIFT(SCOPE, ID, T, BITS, UNSHIFTN) \
  SEQ_IMPL_PUT(SCOPE, ID, T, BITS) \
  SEQ_IMPL_POP(SCOPE, ID, T, BITS, POPN) \
  SEQ_IMPL_SHIFT(SCOPE, ID, T, BITS, SHIFTN) \
  SEQ_IMPL_REMOVE(SCOPE, ID, T, BITS)

#endif /* !__DS_SEQ_H */
