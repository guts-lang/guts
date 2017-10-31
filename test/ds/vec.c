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

#include <cute.h>

#include "ds/vec.h"

#define NOMEM_REALLOC(x, y) ((errno = ENOMEM), nil)
VEC8_DEFINE_DFT(i8vec_nomem, i8_t, NOMEM_REALLOC, mem_free)

static i8_t arr4321[4] = { 4, 3, 2, 1 };

typedef struct {
  f64_t x, y;
} point_t;

VEC_DEFINE(line, point_t)

CUTEST_DATA {
  i8vec_t i8vec;
  i8vec_nomem_t i8vec_nomem;
  line_t line;
};

CUTEST_SETUP { }

CUTEST_TEARDOWN { }

void test_ctor(CUTEST_t *self)
{
  i8vec_ctor(&self->i8vec);
  i8vec_nomem_ctor(&self->i8vec_nomem);
  line_ctor(&self->line);
}

void test_dtor(CUTEST_t *self)
{
  i8vec_dtor(&self->i8vec, nil);
  i8vec_nomem_dtor(&self->i8vec_nomem, nil);
  line_dtor(&self->line, nil);
}

void test_reset(CUTEST_t *self)
{
  i8vec_dtor(&self->i8vec, nil);
  i8vec_nomem_dtor(&self->i8vec_nomem, nil);
  line_dtor(&self->line, nil);
  i8vec_ctor(&self->i8vec);
  i8vec_nomem_ctor(&self->i8vec_nomem);
  line_ctor(&self->line);
}

CUTEST(vec, ensure)
{
  ex_t *e;

  test_reset(self);
  i8vec_ensure(&self->i8vec, 0);
  ASSERT_EQ(0, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_EQ(nil, self->i8vec.buf);
  i8vec_ensure(&self->i8vec, 1);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  i8vec_ensure(&self->i8vec, 1);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  i8vec_ensure(&self->i8vec, 2);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  i8vec_ensure(&self->i8vec, SEQ_MIN_CAP + 1);
  ASSERT_EQ(pow2_next8(SEQ_MIN_CAP + 1), self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);

  TRY {
    i8vec_nomem_ensure(&self->i8vec_nomem, 1);
    ASSERT(false);
  } CATCH(e) (void) e;
  ASSERT_EQ(0, self->i8vec_nomem.cap);
  ASSERT_EQ(0, i8vec_nomem_size(&self->i8vec_nomem));
  ASSERT_EQ(nil, self->i8vec_nomem.buf);

  line_ensure(&self->line, 0);
  ASSERT_EQ(0, self->line.cap);
  ASSERT_EQ(0, line_size(&self->line));
  ASSERT_EQ(nil, self->line.buf);
  line_ensure(&self->line, 1);
  ASSERT_EQ(SEQ_MIN_CAP, self->line.cap);
  ASSERT_EQ(0, line_size(&self->line));
  ASSERT_NEQ(nil, self->line.buf);
  line_ensure(&self->line, 1);
  ASSERT_EQ(SEQ_MIN_CAP, self->line.cap);
  ASSERT_EQ(0, line_size(&self->line));
  ASSERT_NEQ(nil, self->line.buf);
  line_ensure(&self->line, 2);
  ASSERT_EQ(SEQ_MIN_CAP, self->line.cap);
  ASSERT_EQ(0, line_size(&self->line));
  ASSERT_NEQ(nil, self->line.buf);
  line_ensure(&self->line, SEQ_MIN_CAP + 1);
  ASSERT_EQ(pow2_next64(SEQ_MIN_CAP + 1), self->line.cap);
  ASSERT_EQ(0, line_size(&self->line));
  ASSERT_NEQ(nil, self->line.buf);
  line_dtor(&self->line, nil);
  line_ensure(&self->line, SEQ_MIN_CAP);
  ASSERT_EQ(SEQ_MIN_CAP * 2, self->line.cap);
  ASSERT_EQ(0, line_size(&self->line));
  ASSERT_NEQ(nil, self->line.buf);
  line_dtor(&self->line, nil);
  line_ensure(&self->line, pow2_nextsize(SEQ_MIN_CAP + 1));
  ASSERT_EQ(pow2_next64(SEQ_MIN_CAP + 1) * 2, self->line.cap);
  ASSERT_EQ(0, line_size(&self->line));
  ASSERT_NEQ(nil, self->line.buf);
  return CUTE_SUCCESS;
}

CUTEST(vec, grow)
{
  test_reset(self);
  i8vec_grow(&self->i8vec, 0);
  ASSERT_EQ(0, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_EQ(nil, self->i8vec.buf);
  i8vec_grow(&self->i8vec, 1);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  i8vec_grow(&self->i8vec, 2);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  i8vec_grow(&self->i8vec, SEQ_MIN_CAP);
  ASSERT_EQ(SEQ_MIN_CAP * 2, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  i8vec_grow(&self->i8vec, SEQ_MIN_CAP + 1);
  ASSERT_EQ(SEQ_MIN_CAP * 2, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  return CUTE_SUCCESS;
}

CUTEST(vec, shrink)
{
  test_reset(self);
  i8vec_ensure(&self->i8vec, 2);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  i8vec_shrink(&self->i8vec, 2);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  i8vec_shrink(&self->i8vec, SEQ_MIN_CAP / 2);
  ASSERT_EQ(SEQ_MIN_CAP / 2, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  i8vec_shrink(&self->i8vec, SEQ_MIN_CAP / 4);
  ASSERT_EQ(SEQ_MIN_CAP / 4, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  i8vec_shrink(&self->i8vec, SEQ_MIN_CAP / 8);
  ASSERT_EQ(SEQ_MIN_CAP / 8, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  return CUTE_SUCCESS;
}

CUTEST(vec, trim)
{
  test_reset(self);
  i8vec_ensure(&self->i8vec, 1);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  self->i8vec.len = SEQ_MIN_CAP;
  i8vec_trim(&self->i8vec);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8vec.cap);
  ASSERT_EQ(SEQ_MIN_CAP, i8vec_size(&self->i8vec));
  ASSERT_NEQ(nil, self->i8vec.buf);
  while (self->i8vec.len) {
    --self->i8vec.len;
    i8vec_trim(&self->i8vec);
    ASSERT_EQ(self->i8vec.len, self->i8vec.cap);
    if (self->i8vec.len) {
      ASSERT_NEQ(nil, self->i8vec.buf);
    }
  }
  ASSERT_EQ(0, self->i8vec.cap);
  ASSERT_EQ(0, i8vec_size(&self->i8vec));
  ASSERT_EQ(nil, self->i8vec.buf);
  return CUTE_SUCCESS;
}

CUTEST(vec, pushncpy)
{
  i8vec_t state;
  ex_t *e;

  state = self->i8vec;
  ASSERT_EQ(nil, i8vec_pushncpy(&self->i8vec, arr4321, 0));
  ASSERT_EQ(i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(state.buf, self->i8vec.buf);
  ASSERT_NEQ(nil, i8vec_pushncpy(&self->i8vec, arr4321, 1));
  ASSERT_EQ(i8vec_size(&state) + 1, i8vec_size(&self->i8vec));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 0 + i8vec_size(&state)));
  ASSERT_NEQ(nil, i8vec_pushncpy(&self->i8vec, arr4321, 2));
  ASSERT_EQ(3 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 0 + i8vec_size(&state)));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 1 + i8vec_size(&state)));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 2 + i8vec_size(&state)));
  ASSERT_NEQ(nil, i8vec_pushncpy(&self->i8vec, arr4321, 3));
  ASSERT_EQ(6 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 0 + i8vec_size(&state)));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 1 + i8vec_size(&state)));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 2 + i8vec_size(&state)));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 3 + i8vec_size(&state)));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 4 + i8vec_size(&state)));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 5 + i8vec_size(&state)));
  ASSERT_NEQ(nil, i8vec_pushncpy(&self->i8vec, arr4321, 4));
  ASSERT_EQ(10 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 0 + i8vec_size(&state)));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 1 + i8vec_size(&state)));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 2 + i8vec_size(&state)));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 3 + i8vec_size(&state)));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 4 + i8vec_size(&state)));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 5 + i8vec_size(&state)));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 6 + i8vec_size(&state)));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 7 + i8vec_size(&state)));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 8 + i8vec_size(&state)));
  ASSERT_EQ(1, *i8vec_at(&self->i8vec, 9 + i8vec_size(&state)));
  TRY {
    i8vec_nomem_pushncpy(&self->i8vec_nomem, arr4321, 4);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(vec, push)
{
  u8_t i;
  usize_t j;
  i8vec_t state;
  ex_t *e;

  state = self->i8vec;
  for (i = 0; i < 100; ++i) {
    *i8vec_push(&self->i8vec) = i;
    ASSERT_EQ(i + 1 + i8vec_size(&state), i8vec_size(&self->i8vec));
    for (j = i8vec_size(&state); j < i8vec_size(&self->i8vec); ++j) {
      ASSERT_EQ(j - i8vec_size(&state), *i8vec_at(&self->i8vec, j));
    }
  }
  TRY {
    i8vec_nomem_push(&self->i8vec_nomem);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(vec, pushcpy)
{
  u8_t i;
  usize_t j;
  i8vec_t state;
  ex_t *e;

  state = self->i8vec;
  for (i = 0; i < 100; ++i) {
    i8vec_pushcpy(&self->i8vec, i);
    ASSERT_EQ(i + 1 + i8vec_size(&state), i8vec_size(&self->i8vec));
    for (j = i8vec_size(&state); j < i8vec_size(&self->i8vec); ++j) {
      ASSERT_EQ(j - i8vec_size(&state), *i8vec_at(&self->i8vec, j));
    }
  }
  TRY {
    i8vec_nomem_pushcpy(&self->i8vec_nomem, 0);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(vec, unshiftncpy)
{
  i8vec_t state;
  ex_t *e;

  state = self->i8vec;
  ASSERT_EQ(nil, i8vec_unshiftncpy(&self->i8vec, arr4321, 0));
  ASSERT_EQ(i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(state.buf, self->i8vec.buf);
  ASSERT_NEQ(nil, i8vec_unshiftncpy(&self->i8vec, arr4321, 1));
  ASSERT_EQ(1 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 0));
  ASSERT_NEQ(nil, i8vec_unshiftncpy(&self->i8vec, arr4321, 2));
  ASSERT_EQ(3 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 1));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 2));
  ASSERT_NEQ(nil, i8vec_unshiftncpy(&self->i8vec, arr4321, 3));
  ASSERT_EQ(6 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 1));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 2));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 3));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 4));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 5));
  ASSERT_NEQ(nil, i8vec_unshiftncpy(&self->i8vec, arr4321, 4));
  ASSERT_EQ(10 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 1));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 2));
  ASSERT_EQ(1, *i8vec_at(&self->i8vec, 3));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 4));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 5));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 6));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 7));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 8));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 9));
  TRY {
    i8vec_nomem_unshiftncpy(&self->i8vec_nomem, arr4321, 4);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(vec, unshift)
{
  u8_t i;
  i8_t k;
  usize_t j;
  i8vec_t state;
  ex_t *e;

  state = self->i8vec;
  for (i = 0; i < 100; ++i) {
    *i8vec_unshift(&self->i8vec) = i;
    ASSERT_EQ(i + 1 + i8vec_size(&state), i8vec_size(&self->i8vec));
    for (j = 0, k = i; j < i8vec_size(&self->i8vec) - i8vec_size(&state); ++j) {
      ASSERT_EQ(k--, *i8vec_at(&self->i8vec, (u8_t) j));
    }
  }
  TRY {
    i8vec_nomem_unshift(&self->i8vec_nomem);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(vec, unshiftcpy)
{
  u8_t i;
  i8_t k;
  usize_t j;
  i8vec_t state;
  ex_t *e;

  state = self->i8vec;
  for (i = 0; i < 100; ++i) {
    i8vec_unshiftcpy(&self->i8vec, i);
    ASSERT_EQ(i + 1 + i8vec_size(&state), i8vec_size(&self->i8vec));
    for (j = 0, k = i; j < i8vec_size(&self->i8vec) - i8vec_size(&state); ++j) {
      ASSERT_EQ(k--, *i8vec_at(&self->i8vec, (u8_t) j));
    }
  }
  TRY {
    i8vec_nomem_unshiftcpy(&self->i8vec_nomem, 0);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(vec, putncpy)
{
  ex_t *e;
  i8vec_t state;

  state = self->i8vec;
  ASSERT_EQ(nil, i8vec_putncpy(&self->i8vec, 0, arr4321, 0));
  ASSERT_EQ(i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(state.buf, self->i8vec.buf);
  ASSERT_NEQ(nil, i8vec_putncpy(&self->i8vec, 0, arr4321, 1));
  ASSERT_EQ(1 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 0));
  ASSERT_NEQ(nil, i8vec_putncpy(&self->i8vec, 1, arr4321, 2));
  ASSERT_EQ(3 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 1));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 2));
  ASSERT_NEQ(nil, i8vec_putncpy(&self->i8vec, 0, arr4321, 3));
  ASSERT_EQ(6 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 1));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 2));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 3));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 4));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 5));
  ASSERT_NEQ(nil, i8vec_putncpy(&self->i8vec, 3, arr4321, 4));
  ASSERT_EQ(10 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 1));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 2));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 3));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 4));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 5));
  ASSERT_EQ(1, *i8vec_at(&self->i8vec, 6));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 7));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 8));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 9));
  TRY {
    i8vec_nomem_putncpy(&self->i8vec_nomem, 0, arr4321, 1);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(vec, put)
{
  ex_t *e;
  i8_t *ptr;
  i8vec_t state;

  state = self->i8vec;
  ASSERT_NEQ(nil, ptr = i8vec_put(&self->i8vec, 0));
  ASSERT_EQ(1 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(ptr, i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(*ptr = 2, *i8vec_at(&self->i8vec, 0));
  ASSERT_NEQ(nil, ptr = i8vec_put(&self->i8vec, 1));
  ASSERT_EQ(2 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(ptr, i8vec_at(&self->i8vec, 1));
  ASSERT_EQ(*ptr = 4, *i8vec_at(&self->i8vec, 1));
  ASSERT_NEQ(nil, ptr = i8vec_put(&self->i8vec, 0));
  ASSERT_EQ(3 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(ptr, i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(*ptr = 1, *i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 1));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 2));
  ASSERT_NEQ(nil, ptr = i8vec_put(&self->i8vec, 2));
  ASSERT_EQ(4 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(1, *i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 1));
  ASSERT_EQ(ptr, i8vec_at(&self->i8vec, 2));
  ASSERT_EQ(*ptr = 3, *i8vec_at(&self->i8vec, 2));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 3));
  ASSERT_EQ(nil, i8vec_put(&self->i8vec, 5 + i8vec_size(&state)));
  TRY {
    i8vec_nomem_put(&self->i8vec_nomem, 0);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(vec, putcpy)
{
  ex_t *e;
  i8vec_t state;

  state = self->i8vec;
  ASSERT_NEQ(nil, i8vec_putcpy(&self->i8vec, 0, 2));
  ASSERT_EQ(1 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 0));
  ASSERT_NEQ(nil, i8vec_putcpy(&self->i8vec, 1, 4));
  ASSERT_EQ(2 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 1));
  ASSERT_NEQ(nil, i8vec_putcpy(&self->i8vec, 0, 1));
  ASSERT_EQ(3 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(1, *i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 1));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 2));
  ASSERT_NEQ(nil, i8vec_putcpy(&self->i8vec, 2, 3));
  ASSERT_EQ(4 + i8vec_size(&state), i8vec_size(&self->i8vec));
  ASSERT_EQ(1, *i8vec_at(&self->i8vec, 0));
  ASSERT_EQ(2, *i8vec_at(&self->i8vec, 1));
  ASSERT_EQ(3, *i8vec_at(&self->i8vec, 2));
  ASSERT_EQ(4, *i8vec_at(&self->i8vec, 3));
  ASSERT_EQ(nil, i8vec_putcpy(&self->i8vec, 5 + i8vec_size(&state), 5));
  TRY {
    i8vec_nomem_putcpy(&self->i8vec_nomem, 0, 1);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(vec, pop)
{
  i8_t i;
  i8vec_t state;

  state = self->i8vec;
  if (i8vec_size(&state) < 4) {
    ASSERT_NEQ(nil, i8vec_pushncpy(&self->i8vec, arr4321, 4));
    state = self->i8vec;
  }
  ASSERT_EQ(true, i8vec_pop(&self->i8vec, nil));
  ASSERT_EQ(i8vec_size(&state) - 1, i8vec_size(&self->i8vec));
  ASSERT_EQ(true, i8vec_pop(&self->i8vec, &i));
  ASSERT_EQ(*i8vec_at(&state, i8vec_size(&state) - 2), i);
  ASSERT_EQ(i8vec_size(&state) - 2, i8vec_size(&self->i8vec));
  ASSERT_EQ(true, i8vec_pop(&self->i8vec, &i));
  ASSERT_EQ(*i8vec_at(&state, i8vec_size(&state) - 3), i);
  ASSERT_EQ(i8vec_size(&state) - 3, i8vec_size(&self->i8vec));
  ASSERT_EQ(true, i8vec_pop(&self->i8vec, &i));
  ASSERT_EQ(*i8vec_at(&state, i8vec_size(&state) - 4), i);
  ASSERT_EQ(i8vec_size(&state) - 4, i8vec_size(&self->i8vec));
  ASSERT_EQ(i8vec_size(&state) != 4, i8vec_pop(&self->i8vec, &i));
  return CUTE_SUCCESS;
}

CUTEST(vec, shift)
{
  i8_t i;
  i8vec_t state;
  i8_t buf[4];

  state = self->i8vec;
  if (i8vec_size(&state) < 4) {
    ASSERT_NEQ(nil, i8vec_pushncpy(&self->i8vec, arr4321, 4));
    state = self->i8vec;
  }
  memcpy(buf, i8vec_begin(&state), 4 * sizeof(i8_t));
  ASSERT_EQ(true, i8vec_shift(&self->i8vec, nil));
  ASSERT_EQ(i8vec_size(&state) - 1, i8vec_size(&self->i8vec));
  ASSERT_EQ(true, i8vec_shift(&self->i8vec, &i));
  ASSERT_EQ(buf[1], i);
  ASSERT_EQ(i8vec_size(&state) - 2, i8vec_size(&self->i8vec));
  ASSERT_EQ(true, i8vec_shift(&self->i8vec, &i));
  ASSERT_EQ(buf[2], i);
  ASSERT_EQ(i8vec_size(&state) - 3, i8vec_size(&self->i8vec));
  ASSERT_EQ(true, i8vec_shift(&self->i8vec, &i));
  ASSERT_EQ(buf[3], i);
  ASSERT_EQ(i8vec_size(&state) - 4, i8vec_size(&self->i8vec));
  ASSERT_EQ(i8vec_size(&state) != 4, i8vec_shift(&self->i8vec, &i));
  return CUTE_SUCCESS;
}

CUTEST(vec, rem)
{
  i8_t i;
  i8_t vi8[4] = { 0, 1, 2, 3 };
  i8vec_t state;
  i8_t buf[4];

  state = self->i8vec;
  if (i8vec_size(&state) < 4) {
    ASSERT_NEQ(nil, i8vec_pushncpy(&self->i8vec, vi8, 4));
    state = self->i8vec;
  }
  memcpy(buf, i8vec_begin(&state), 4 * sizeof(i8_t));
  ASSERT_EQ(false, i8vec_remove(&self->i8vec, i8vec_size(&state), nil));
  ASSERT_EQ(true, i8vec_remove(&self->i8vec, 2, nil));
  ASSERT_EQ(i8vec_size(&state) - 1, i8vec_size(&self->i8vec));
  ASSERT_EQ(true, i8vec_remove(&self->i8vec, 1, &i));
  ASSERT_EQ(buf[1], i);
  ASSERT_EQ(i8vec_size(&state) - 2, i8vec_size(&self->i8vec));
  ASSERT_EQ(true, i8vec_remove(&self->i8vec, 1, &i));
  ASSERT_EQ(buf[3], i);
  ASSERT_EQ(i8vec_size(&state) - 3, i8vec_size(&self->i8vec));
  ASSERT_EQ(true, i8vec_remove(&self->i8vec, 0, &i));
  ASSERT_EQ(buf[0], i);
  ASSERT_EQ(i8vec_size(&state) - 4, i8vec_size(&self->i8vec));
  ASSERT_EQ(i8vec_size(&state) != 4, i8vec_remove(&self->i8vec, 0, &i));
  return CUTE_SUCCESS;
}

i32_t
main(void)
{
  CUTEST_DATA test;

  test_ctor(&test);
  CUTEST_PASS(vec, ensure);
  test_reset(&test);
  CUTEST_PASS(vec, grow);
  test_reset(&test);
  CUTEST_PASS(vec, shrink);
  test_reset(&test);
  CUTEST_PASS(vec, trim);
  test_reset(&test);
  CUTEST_PASS(vec, pushncpy);
  test_reset(&test);
  CUTEST_PASS(vec, push);
  test_reset(&test);
  CUTEST_PASS(vec, pushcpy);
  test_reset(&test);
  CUTEST_PASS(vec, unshiftncpy);
  test_reset(&test);
  CUTEST_PASS(vec, unshift);
  test_reset(&test);
  CUTEST_PASS(vec, unshiftcpy);
  test_reset(&test);
  CUTEST_PASS(vec, putncpy);
  test_reset(&test);
  CUTEST_PASS(vec, put);
  test_reset(&test);
  CUTEST_PASS(vec, putcpy);
  test_reset(&test);
  CUTEST_PASS(vec, pop);
  test_reset(&test);
  CUTEST_PASS(vec, shift);
  test_reset(&test);
  CUTEST_PASS(vec, rem);
  test_reset(&test);
  CUTEST_PASS(vec, ensure);
  CUTEST_PASS(vec, grow);
  CUTEST_PASS(vec, shrink);
  CUTEST_PASS(vec, trim);
  CUTEST_PASS(vec, pushncpy);
  CUTEST_PASS(vec, push);
  CUTEST_PASS(vec, pushcpy);
  CUTEST_PASS(vec, unshiftncpy);
  CUTEST_PASS(vec, unshift);
  CUTEST_PASS(vec, unshiftcpy);
  CUTEST_PASS(vec, putncpy);
  CUTEST_PASS(vec, put);
  CUTEST_PASS(vec, putcpy);
  CUTEST_PASS(vec, pop);
  CUTEST_PASS(vec, shift);
  CUTEST_PASS(vec, rem);
  test_reset(&test);
  CUTEST_PASS(vec, pushncpy);
  CUTEST_PASS(vec, pop);
  CUTEST_PASS(vec, putncpy);
  CUTEST_PASS(vec, shift);
  CUTEST_PASS(vec, unshiftncpy);
  CUTEST_PASS(vec, rem);
  CUTEST_PASS(vec, push);
  CUTEST_PASS(vec, pop);
  CUTEST_PASS(vec, put);
  CUTEST_PASS(vec, shift);
  CUTEST_PASS(vec, unshift);
  CUTEST_PASS(vec, rem);
  CUTEST_PASS(vec, unshiftcpy);
  CUTEST_PASS(vec, pop);
  CUTEST_PASS(vec, putcpy);
  CUTEST_PASS(vec, shift);
  CUTEST_PASS(vec, pushcpy);
  CUTEST_PASS(vec, rem);
  test_reset(&test);
  CUTEST_PASS(vec, pop);
  CUTEST_PASS(vec, pushncpy);
  CUTEST_PASS(vec, shift);
  CUTEST_PASS(vec, putncpy);
  CUTEST_PASS(vec, rem);
  CUTEST_PASS(vec, unshiftncpy);
  CUTEST_PASS(vec, pop);
  CUTEST_PASS(vec, push);
  CUTEST_PASS(vec, shift);
  CUTEST_PASS(vec, put);
  CUTEST_PASS(vec, rem);
  CUTEST_PASS(vec, unshift);
  CUTEST_PASS(vec, pop);
  CUTEST_PASS(vec, unshiftcpy);
  CUTEST_PASS(vec, shift);
  CUTEST_PASS(vec, putcpy);
  CUTEST_PASS(vec, rem);
  CUTEST_PASS(vec, pushcpy);
  test_reset(&test);
  CUTEST_PASS(vec, put);
  CUTEST_PASS(vec, rem);
  CUTEST_PASS(vec, unshift);
  CUTEST_PASS(vec, pop);
  CUTEST_PASS(vec, unshiftcpy);
  CUTEST_PASS(vec, shift);
  CUTEST_PASS(vec, putcpy);
  CUTEST_PASS(vec, rem);
  CUTEST_PASS(vec, pushcpy);
  CUTEST_PASS(vec, pop);
  CUTEST_PASS(vec, pushncpy);
  CUTEST_PASS(vec, shift);
  CUTEST_PASS(vec, putncpy);
  CUTEST_PASS(vec, rem);
  CUTEST_PASS(vec, unshiftncpy);
  CUTEST_PASS(vec, pop);
  CUTEST_PASS(vec, push);
  CUTEST_PASS(vec, shift);
  test_dtor(&test);
  return EXIT_SUCCESS;
}
