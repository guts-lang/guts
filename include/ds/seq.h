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
  ID##_ctor(ID##_t *__restrict self)

#define SEQ_IMPL_ctor(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_ctor(SCOPE, ID, T, BITS) { \
    init(self, ID##_t); \
  }

#define SEQ_DECL_dtor(SCOPE, ID, T, BITS) \
  SCOPE void \
  ID##_dtor(ID##_t *__restrict self, void (*idtor)(T *))

#define SEQ_IMPL_dtor(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_dtor(SCOPE, ID, T, BITS) { \
    if (self->BUF) { \
      if (idtor) { \
        u##BITS##_t i; \
        T *item; \
        foreach_nx(i, item, self->BUF, self->LEN) { \
          idtor(item); \
        } \
      } \
      FREE(self->BUF); \
    } \
    init(self, ID##_t); \
  }

#define SEQ_DECL_cap(SCOPE, ID, T, BITS) \
  SCOPE u##BITS##_t \
  ID##_cap(ID##_t *__restrict self)

#define SEQ_IMPL_cap(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_cap(SCOPE, ID, T, BITS) { \
    return self->CAP; \
  }

#define SEQ_DECL_size(SCOPE, ID, T, BITS) \
  SCOPE u##BITS##_t \
  ID##_size(ID##_t *__restrict self)

#define SEQ_IMPL_size(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_size(SCOPE, ID, T, BITS) { \
    return self->LEN; \
  }

#define SEQ_DECL_at(SCOPE, ID, T, BITS) \
  SCOPE T* \
  ID##_at(ID##_t *__restrict self, u##BITS##_t idx)

#define SEQ_IMPL_at(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_at(SCOPE, ID, T, BITS) { \
    return self->BUF + idx; \
  }

#define SEQ_DECL_front(SCOPE, ID, T, BITS) \
  SCOPE T* \
  ID##_front(ID##_t *__restrict self)

#define SEQ_IMPL_front(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_front(SCOPE, ID, T, BITS) { \
    return self->BUF; \
  }

#define SEQ_DECL_back(SCOPE, ID, T, BITS) \
  SCOPE T* \
  ID##_back(ID##_t *__restrict self)

#define SEQ_IMPL_back(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_back(SCOPE, ID, T, BITS) { \
    return self->LEN ? self->BUF + self->LEN - 1 : self->BUF; \
  }

#define SEQ_DECL_realloc(SCOPE, ID, T, BITS) \
  SCOPE void \
  ID##_realloc(ID##_t *__restrict self, u##BITS##_t n)

#define SEQ_IMPL_realloc(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_realloc(SCOPE, ID, T, BITS) { \
    if (self->CAP != n) { \
      void *buf; \
      if ((buf = REALLOC(self->BUF, (usize_t) n * sizeof(T))) == nil) { \
        THROW (ex_errno(ERRLVL_NOTICE, ENOMEM, nil)); \
      } \
      self->BUF = buf; \
      if (self->LEN > (self->CAP = n)) self->LEN = n; \
    } \
  }

#define SEQ_DECL_ensure(SCOPE, ID, T, BITS) \
  SCOPE void \
  ID##_ensure(ID##_t *__restrict self, u##BITS##_t n)

#define SEQ_IMPL_ensure(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_ensure(SCOPE, ID, T, BITS) { \
    if (self->CAP < n) { \
      if (n < SEQ_MIN_CAP) n = SEQ_MIN_CAP; \
      else if (!ISPOW2(n)) n = pow2_next##BITS(n); \
      ID##_realloc(self, n); \
    } \
  }

#define SEQ_IMPL_ensure_strict(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_ensure(SCOPE, ID, T, BITS) { \
    if (self->CAP < n) { \
      ID##_realloc(self, n); \
    } \
  }

#define SEQ_DECL_grow(SCOPE, ID, T, BITS) \
  SCOPE void \
  ID##_grow(ID##_t *__restrict self, u##BITS##_t n)

#define SEQ_IMPL_grow(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_grow(SCOPE, ID, T, BITS) { \
    n += self->LEN; \
    if (self->CAP < n) { \
      if (n < SEQ_MIN_CAP) n = SEQ_MIN_CAP; \
      else if (!ISPOW2(n)) n = pow2_next##BITS(n); \
      ID##_realloc(self, n); \
    } \
  }

#define SEQ_IMPL_grow_strict(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_grow(SCOPE, ID, T, BITS) { \
    n += self->LEN; \
    if (self->CAP < n) { \
      ID##_realloc(self, n); \
    } \
  }

#define SEQ_DECL_shrink(SCOPE, ID, T, BITS) \
  SCOPE void \
  ID##_shrink(ID##_t *__restrict self, u##BITS##_t n)

#define SEQ_IMPL_shrink(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_shrink(SCOPE, ID, T, BITS) { \
    n = self->CAP - n; \
    if (n < self->CAP && ISPOW2(n)) { \
      ID##_realloc(self, n); \
    } \
  }

#define SEQ_DECL_trim(SCOPE, ID, T, BITS) \
  SCOPE void \
  ID##_trim(ID##_t *__restrict self)

#define SEQ_IMPL_trim(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_trim(SCOPE, ID, T, BITS) { \
    if (self->LEN == 0 && self->CAP) ID##_dtor(self, nil); \
    else if (self->CAP > self->LEN) { \
      ID##_realloc(self, self->LEN); \
    } \
  }

#define SEQ_DECL_insert(SCOPE, ID, T, BITS) \
  SCOPE T* \
  ID##_insert(ID##_t *__restrict self, __const u##BITS##_t idx)

#define SEQ_IMPL_insert(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_insert(SCOPE, ID, T, BITS) { \
    if (idx > ID##_size(self)) { \
      return nil; \
    } \
    ID##_grow(self, 1); \
    if (idx == ID##_size(self)) { \
      ++self->LEN; \
    } else { \
      memmove( \
        self->BUF + idx + 1, \
        self->BUF + idx, \
        (usize_t) (self->LEN++ - idx) * sizeof(T) \
      ); \
    } \
    return self->BUF + idx; \
  }

#define SEQ_DECL_emplace(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_emplace(ID##_t *__restrict self, __const u##BITS##_t idx, T *items, \
    __const u##BITS##_t n)

#define SEQ_IMPL_emplace(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_emplace(SCOPE, ID, T, BITS) { \
    if (idx > ID##_size(self)) { \
      return false; \
    } \
    ID##_grow(self, n); \
    if (idx != ID##_size(self)) { \
      memmove( \
        self->BUF + idx + n, \
        self->BUF + idx, \
        (usize_t) (ID##_size(self) - idx) * sizeof(T) \
      ); \
    } \
    memcpy(self->BUF + idx, items, (usize_t) n * sizeof(T)); \
    self->LEN += n; \
    return true; \
  }

#define SEQ_DECL_nemplace(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_nemplace(ID##_t *__restrict self, __const u##BITS##_t idx, T *items, \
    __const u##BITS##_t n)

#define SEQ_IMPL_nemplace(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_nemplace(SCOPE, ID, T, BITS) { \
    if (idx > ID##_size(self)) { \
      return false; \
    } \
    ID##_grow(self, n); \
    if (idx != ID##_size(self)) { \
      memmove( \
        self->BUF + idx + n, \
        self->BUF + idx, \
        (usize_t) (ID##_size(self) - idx) * sizeof(T) \
      ); \
    } \
    memcpy(self->BUF + idx, items, (usize_t) n * sizeof(T)); \
    self->LEN += n; \
    return true; \
  }

#define SEQ_DECL_emplace_nt(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_emplace(ID##_t *__restrict self, __const u##BITS##_t idx, T *items)

#define SEQ_IMPL_emplace_nt(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_emplace_nt(SCOPE, ID, T, BITS) { \
    u##BITS##_t n; \
    if (idx == ID##_size(self)) { \
      return ID##_append(self, items); \
    } \
    if (idx == 0) { \
      return ID##_prepend(self, items); \
    } \
    if (idx > ID##_size(self)) { \
      return false; \
    } \
    n = (u##BITS##_t) strlen((char_t __const *) items); \
    ID##_grow(self, n + 1); \
    if (idx != ID##_size(self)) { \
      memmove( \
        self->BUF + idx + n, \
        self->BUF + idx, \
        (usize_t) (ID##_size(self) - idx + 1) * sizeof(T) \
      ); \
    } \
    memcpy(self->BUF + idx, items, (usize_t) n * sizeof(T)); \
    self->LEN += n; \
    return true; \
  }

#define SEQ_DECL_push(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_push(ID##_t *__restrict self, T item)

#define SEQ_IMPL_push(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_push(SCOPE, ID, T, BITS) { \
    ID##_grow(self, 1); \
    self->BUF[self->LEN++] = item; \
    return true; \
  }

#define SEQ_IMPL_push_nt(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_push(SCOPE, ID, T, BITS) { \
    ID##_grow(self, 2); \
    self->BUF[self->LEN++] = item; \
    self->BUF[self->LEN] = '\0'; \
    return true; \
  }

#define SEQ_DECL_append(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_append(ID##_t *__restrict self, T *items, __const u##BITS##_t n)

#define SEQ_IMPL_append(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_append(SCOPE, ID, T, BITS) { \
    if (n == 0) { \
      return true; \
    } \
    ID##_grow(self, n); \
    memcpy(self->BUF + self->LEN, items, (usize_t) n * sizeof(T)); \
    self->LEN += n; \
    return true; \
  }

#define SEQ_DECL_nappend(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_nappend(ID##_t *__restrict self, T *items, __const u##BITS##_t n)

#define SEQ_IMPL_nappend(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_nappend(SCOPE, ID, T, BITS) { \
    if (n == 0) { \
      return true; \
    } \
    ID##_grow(self, n); \
    memcpy(self->BUF + self->LEN, items, (usize_t) n * sizeof(T)); \
    self->LEN += n; \
    return true; \
  }

#define SEQ_DECL_append_nt(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_append(ID##_t *__restrict self, T *items)

#define SEQ_IMPL_append_nt(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_append_nt(SCOPE, ID, T, BITS) { \
    u##BITS##_t n; \
    if ((n = (u##BITS##_t) strlen((char_t __const *) items)) == 0) { \
      return true; \
    } \
    ID##_grow(self, n + 1); \
    memcpy(self->BUF + self->LEN, items, (usize_t) (n + 1) * sizeof(T)); \
    self->LEN += n; \
    return true; \
  }

#define SEQ_DECL_pop(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_pop(ID##_t *__restrict self, T *__restrict out)

#define SEQ_IMPL_pop(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_pop(SCOPE, ID, T, BITS) { \
    if (ID##_size(self) == 0) { \
      return false; \
    } \
    --self->LEN; \
    if (out != nil) { \
      *out = self->BUF[self->LEN]; \
    } \
    return true; \
  }

#define SEQ_IMPL_pop_nt(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_pop(SCOPE, ID, T, BITS) { \
    if (ID##_size(self) == 0) { \
      return false; \
    } \
    --self->LEN; \
    if (out != nil) { \
      *out = self->BUF[self->LEN]; \
    } \
    self->BUF[self->LEN] = '\0'; \
    return true; \
  }

#define SEQ_DECL_unshift(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_unshift(ID##_t *__restrict self, T item)

#define SEQ_IMPL_unshift(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_unshift(SCOPE, ID, T, BITS) { \
    ID##_grow(self, 1); \
    memmove( \
      self->BUF + 1, \
      self->BUF, \
      (usize_t) self->LEN++ * sizeof(T) \
    ); \
    self->BUF[0] = item; \
    return true; \
  }

#define SEQ_DECL_prepend(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_prepend(ID##_t *__restrict self, T *items, __const u##BITS##_t n)

#define SEQ_IMPL_prepend(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_prepend(SCOPE, ID, T, BITS) { \
    if (n == 0) { \
      return true; \
    } \
    ID##_grow(self, n); \
    memmove( \
      self->BUF + n, \
      self->BUF, \
      (usize_t) ID##_size(self) * sizeof(T) \
    ); \
    memcpy(self->BUF, items, (usize_t) n * sizeof(T)); \
    self->LEN += n; \
    return true; \
  }

#define SEQ_DECL_nprepend(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_nprepend(ID##_t *__restrict self, T *items, __const u##BITS##_t n)

#define SEQ_IMPL_nprepend(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_nprepend(SCOPE, ID, T, BITS) { \
    if (n == 0) { \
      return true; \
    } \
    ID##_grow(self, n); \
    memmove( \
      self->BUF + n, \
      self->BUF, \
      (usize_t) ID##_size(self) * sizeof(T) \
    ); \
    memcpy(self->BUF, items, (usize_t) n * sizeof(T)); \
    self->LEN += n; \
    return true; \
  }

#define SEQ_DECL_prepend_nt(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_prepend(ID##_t *__restrict self, T *items)

#define SEQ_IMPL_prepend_nt(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_prepend_nt(SCOPE, ID, T, BITS) { \
    u##BITS##_t n; \
    n = (u##BITS##_t) strlen((char_t __const *) items); \
    if (n == 0) { \
      return true; \
    } \
    if (ID##_size(self) == 0) { \
      return ID##_append(self, items); \
    } \
    ID##_grow(self, n); \
    memmove( \
      self->BUF + n, \
      self->BUF, \
      (usize_t) ID##_size(self) * sizeof(T) \
    ); \
    memcpy(self->BUF, items, (usize_t) n * sizeof(T)); \
    self->LEN += n; \
    return true; \
  }

#define SEQ_DECL_shift(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_shift(ID##_t *__restrict self, T *__restrict out)

#define SEQ_IMPL_shift(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_shift(SCOPE, ID, T, BITS) { \
    if (ID##_size(self) == 0) { \
      return false; \
    } \
    if (out != nil) { \
      *out = self->BUF[0]; \
    } \
    if (self->LEN == 1) { \
      --self->LEN; \
    } else { \
      memmove( \
        self->BUF, \
        self->BUF + 1, \
        (usize_t) --self->LEN * sizeof(T) \
      ); \
    } \
    return true; \
  }

#define SEQ_DECL_remove(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_remove(ID##_t *__restrict self, __const u##BITS##_t idx, T* out)

#define SEQ_IMPL_remove(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_remove(SCOPE, ID, T, BITS) { \
    if (idx >= ID##_size(self)) { \
      return false; \
    } \
    if (out != nil) { \
      *out = self->BUF[idx]; \
    } \
    if (idx + 1 == self->LEN) { \
      --self->LEN; \
    } else { \
      memmove( \
        self->BUF + idx, \
        self->BUF + idx + 1, \
        (usize_t) (--self->LEN - idx) * sizeof(T) \
      ); \
    } \
    return true; \
  }

#define SEQ_DECL_nshift(SCOPE, ID, T, BITS) \
  SCOPE u##BITS##_t \
  ID##_nshift(ID##_t *__restrict self, __const u##BITS##_t n, T* out[])

#define SEQ_IMPL_nshift(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_nshift(SCOPE, ID, T, BITS) { \
    u##BITS##_t n2; \
    if ((n2 = ID##_size(self)) == 0) { \
      return 0; \
    } \
    if (n2 > n) n2 = n; \
    if (out != nil) { \
      memcpy(*out, ID##_at(self, 0), (usize_t) n2); \
    } \
    memmove( \
      self->BUF, \
      self->BUF + n2, \
      (usize_t) ((self->LEN -= n2)) * sizeof(T) \
    ); \
    return n; \
  }

#define SEQ_DECL_removen(SCOPE, ID, T, BITS) \
  SCOPE u##BITS##_t \
  ID##_removen(ID##_t *__restrict self, __const u##BITS##_t idx, \
    __const u##BITS##_t n, T* out[])

#define SEQ_IMPL_removen(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_removen(SCOPE, ID, T, BITS) { \
    u##BITS##_t n2; \
    if (idx >= (n2 = ID##_size(self))) { \
      return 0; \
    } \
    if (n2 > n) n2 = n; \
    if (out != nil) { \
      memcpy(*out, ID##_at(self, idx), (usize_t) n2); \
    } \
    if (idx + n2 > self->LEN) { \
      self->LEN = idx; \
    } else { \
      memmove( \
        self->BUF + idx, \
        self->BUF + idx + n2, \
        (usize_t) ((self->LEN -= n2) - idx) * sizeof(T) \
      ); \
    } \
    return n2; \
  }

#define SEQ_DECL_erase(SCOPE, ID, T, BITS) \
  SCOPE u##BITS##_t \
  ID##_erase(ID##_t *__restrict self, T item)

#define SEQ_IMPL_erase(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_erase(SCOPE, ID, T, BITS) { \
    u##BITS##_t len, i, j, n; \
    if ((len = ID##_size(self)) == 0) { \
      return 0; \
    } \
    for (i = 0; i < self->LEN; ++i) { \
      if (CMP(item, self->BUF[i]) == 0) { \
        j = i; \
        n = 1; \
        while (i + 1 < self->LEN && CMP(item, self->BUF[i + 1]) == 0) { \
          ++i; \
          ++n; \
        } \
        memmove( \
          self->BUF + j, \
          self->BUF + j + n, \
          (usize_t) ((self->LEN -= n) - j) * sizeof(T) \
        ); \
      } \
    } \
    return len - ID##_size(self); \
  }

#define SEQ_DECL_erasen(SCOPE, ID, T, BITS) \
  SCOPE u##BITS##_t \
  ID##_erasen(ID##_t *__restrict self, T item, u##BITS##_t n)

#define SEQ_IMPL_erasen(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_erasen(SCOPE, ID, T, BITS) { \
    u##BITS##_t len, i, j, c; \
    if (n == 0 || (len = ID##_size(self)) == 0) { \
      return 0; \
    } \
    for (i = 0; i < self->LEN && n > 0; ++i) { \
      if (CMP(item, self->BUF[i]) == 0) { \
        --n; \
        j = i; \
        c = 1; \
        while (n > 0 && i + 1 < self->LEN && \
          CMP(item, self->BUF[i + 1]) == 0) { \
          --n; \
          ++i; \
          ++c; \
        } \
        memmove( \
          self->BUF + j, \
          self->BUF + j + c, \
          (usize_t) ((self->LEN -= c) - j) * sizeof(T) \
        ); \
      } \
    } \
    return len - ID##_size(self); \
  }

#define SEQ_DECL_eraseonce(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_eraseonce(ID##_t *__restrict self, T item)

#define SEQ_IMPL_eraseonce(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, \
  CMP) \
  SEQ_DECL_eraseonce(SCOPE, ID, T, BITS) { \
    u##BITS##_t i; \
    for (i = 0; i < self->LEN; ++i) { \
      if (CMP(item, self->BUF[i]) == 0) { \
        memmove( \
          self->BUF + i, \
          self->BUF + i + 1, \
          (usize_t) (--self->LEN - i) * sizeof(T) \
        ); \
        return true; \
      } \
    } \
    return false; \
  }

#define SEQ_DECL_cpy(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_cpy(ID##_t *__restrict self, ID##_t *__restrict src)

#define SEQ_IMPL_cpy(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_cpy(SCOPE, ID, T, BITS) { \
    ID##_ensure(self, src->LEN); \
    memcpy(self->BUF, src->BUF, (usize_t) (self->LEN = src->LEN)); \
    return true; \
  }

#define SEQ_IMPL_cpy_nt(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_cpy(SCOPE, ID, T, BITS) { \
    ID##_ensure(self, src->LEN + 1); \
    memcpy(self->BUF, src->BUF, (usize_t) ((self->LEN = src->LEN) + 1)); \
    return true; \
  }

#define SEQ_DECL_ncpy(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_ncpy(ID##_t *__restrict self, ID##_t *__restrict src, \
    __const u##BITS##_t n)

#define SEQ_IMPL_ncpy(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_ncpy(SCOPE, ID, T, BITS) { \
    ID##_ensure(self, n); \
    memcpy(self->BUF, src->BUF, (usize_t) (self->LEN = n)); \
    return true; \
  }

#define SEQ_IMPL_ncpy_nt(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_ncpy(SCOPE, ID, T, BITS) { \
    ID##_ensure(self, n + 1); \
    memcpy(self->BUF, src->BUF, (usize_t) ((self->LEN = n) + 1)); \
    return true; \
  }

#define SEQ_DECL_has(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_has(ID##_t *__restrict self, T item, u##BITS##_t *out)

#define SEQ_IMPL_has(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_has(SCOPE, ID, T, BITS) { \
    u##BITS##_t i; \
    for (i = 0; i < self->LEN; ++i) { \
      if (CMP(item, self->BUF[i]) == 0) { \
        if (out != nil) { \
          *out = i; \
        } \
        return true; \
      } \
    } \
    return false; \
  }

#define SEQ_DECL_any(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_any(ID##_t *__restrict self, bool_t (*cb)(T *, void *), void *data)

#define SEQ_IMPL_any(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_any(SCOPE, ID, T, BITS) { \
    u##BITS##_t i; \
    for (i = 0; i < self->LEN; ++i) { \
      if (cb(self->BUF + i, data) == true) { \
        return true; \
      } \
    } \
    return false; \
  }

#define SEQ_DECL_counteq(SCOPE, ID, T, BITS) \
  SCOPE u##BITS##_t \
  ID##_counteq(ID##_t *__restrict self, T item)

#define SEQ_IMPL_counteq(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_counteq(SCOPE, ID, T, BITS) { \
    u##BITS##_t i, c; \
    for (c = i = 0; i < self->LEN; ++i) { \
      if (CMP(item, self->BUF[i]) == 0) { \
        ++c; \
      } \
    } \
    return c; \
  }

#define SEQ_DECL_count(SCOPE, ID, T, BITS) \
  SCOPE u##BITS##_t \
  ID##_count(ID##_t *__restrict self, bool_t (*cb)(T *, void *), void *data)

#define SEQ_IMPL_count(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_count(SCOPE, ID, T, BITS) { \
    u##BITS##_t i, c; \
    for (c = i = 0; i < self->LEN; ++i) { \
      if (cb(self->BUF + i, data) == true) { \
        ++c; \
      } \
    } \
    return c; \
  }

#define SEQ_DECL_select(SCOPE, ID, T, BITS) \
  SCOPE bool_t \
  ID##_select(ID##_t *__restrict self, ID##_t *__restrict dest, \
    bool_t (*cb)(T *, void *), void *data)

#define SEQ_IMPL_select(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  SEQ_DECL_select(SCOPE, ID, T, BITS) { \
    u##BITS##_t i; \
    bool_t ret; \
    ret = false; \
    for (i = 0; i < self->LEN; ++i) { \
      if (cb(self->BUF + i, data) == true) { \
        if (ID##_push(dest, self->BUF[i])) ret = true; \
      } \
    } \
    return ret; \
  }

#define SEQ_DECL( \
    SCOPE, ID, T, BITS, \
    DECL_ctor, DECL_dtor, \
    DECL_cap, DECL_size, \
    DECL_at, DECL_front, DECL_back, \
    DECL_realloc, DECL_ensure, DECL_grow, DECL_shrink, DECL_trim, \
    DECL_insert, DECL_emplace, \
    DECL_push, DECL_append, DECL_pop, \
    DECL_unshift, DECL_prepend, DECL_shift, DECL_nshift, \
    DECL_remove, DECL_removen, \
    DECL_erase, DECL_erasen, DECL_eraseonce, \
    DECL_cpy, DECL_ncpy) \
  DECL_ctor(SCOPE, ID, T, BITS); \
  DECL_dtor(SCOPE, ID, T, BITS); \
  DECL_cap(SCOPE, ID, T, BITS); \
  DECL_size(SCOPE, ID, T, BITS); \
  DECL_at(SCOPE, ID, T, BITS); \
  DECL_front(SCOPE, ID, T, BITS); \
  DECL_back(SCOPE, ID, T, BITS); \
  DECL_realloc(SCOPE, ID, T, BITS); \
  DECL_ensure(SCOPE, ID, T, BITS); \
  DECL_grow(SCOPE, ID, T, BITS); \
  DECL_shrink(SCOPE, ID, T, BITS); \
  DECL_trim(SCOPE, ID, T, BITS); \
  DECL_insert(SCOPE, ID, T, BITS); \
  DECL_emplace(SCOPE, ID, T, BITS); \
  DECL_push(SCOPE, ID, T, BITS); \
  DECL_append(SCOPE, ID, T, BITS); \
  DECL_pop(SCOPE, ID, T, BITS); \
  DECL_unshift(SCOPE, ID, T, BITS); \
  DECL_prepend(SCOPE, ID, T, BITS); \
  DECL_shift(SCOPE, ID, T, BITS); \
  DECL_nshift(SCOPE, ID, T, BITS); \
  DECL_remove(SCOPE, ID, T, BITS); \
  DECL_removen(SCOPE, ID, T, BITS); \
  DECL_erase(SCOPE, ID, T, BITS); \
  DECL_erasen(SCOPE, ID, T, BITS); \
  DECL_eraseonce(SCOPE, ID, T, BITS); \
  DECL_cpy(SCOPE, ID, T, BITS); \
  DECL_ncpy(SCOPE, ID, T, BITS);

#define SEQ_IMPL( \
    SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP, \
    IMPL_ctor, IMPL_dtor, \
    IMPL_cap, IMPL_size, \
    IMPL_at, IMPL_front, IMPL_back, \
    IMPL_realloc, IMPL_ensure, IMPL_grow, IMPL_shrink, IMPL_trim, \
    IMPL_insert, IMPL_emplace, \
    IMPL_push, IMPL_append, IMPL_pop, \
    IMPL_unshift, IMPL_prepend, IMPL_shift, IMPL_nshift, \
    IMPL_remove, IMPL_removen, \
    IMPL_erase, IMPL_erasen, IMPL_eraseonce, \
    IMPL_cpy, IMPL_ncpy) \
  IMPL_ctor(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_dtor(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_cap(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_size(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_at(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_front(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_back(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_realloc(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_ensure(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_grow(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_shrink(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_trim(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_insert(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_emplace(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_push(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_append(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_pop(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_unshift(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_prepend(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_shift(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_nshift(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_remove(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_removen(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_erase(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_erasen(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_eraseonce(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_cpy(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP) \
  IMPL_ncpy(SCOPE, ID, T, BITS, CAP, LEN, BUF, REALLOC, FREE, CMP)

#endif /* !__DS_SEQ_H */
