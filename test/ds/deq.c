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

#include "ds/deq.h"

#define NOMEM_REALLOC(x, y) ((errno = ENOMEM), nil)
DEQ8_DEFINE_DFT(i8deq_nomem, i8_t, NOMEM_REALLOC, mem_free)

static i8_t arr1234[4] = { 1, 2, 3, 4 };
static i8_t arr4321[4] = { 4, 3, 2, 1 };

typedef struct {
  f64_t x, y;
} point_t;

DEQ_DEFINE(line, point_t)

CUTEST_DATA {
  i8deq_t i8deq;
  i8deq_nomem_t i8deq_nomem;
  line_t line;
};

CUTEST_SETUP
{
  i8deq_ctor(&self->i8deq);
  i8deq_nomem_ctor(&self->i8deq_nomem);
  line_ctor(&self->line);
}

CUTEST_TEARDOWN
{
  i8deq_dtor(&self->i8deq, nil);
  i8deq_nomem_dtor(&self->i8deq_nomem, nil);
  line_dtor(&self->line, nil);
}

CUTEST(deq, ensure)
{
  ex_t *e;

  i8deq_ensure(&self->i8deq, 0);
  ASSERT_EQ(0, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_EQ(nil, self->i8deq.buf);
  i8deq_ensure(&self->i8deq, 1);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  i8deq_ensure(&self->i8deq, 1);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  i8deq_ensure(&self->i8deq, 2);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  i8deq_ensure(&self->i8deq, SEQ_MIN_CAP + 1);
  ASSERT_EQ(pow2_next8(SEQ_MIN_CAP + 1), self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);

  TRY {
    i8deq_nomem_ensure(&self->i8deq_nomem, 1);
    ASSERT(false);
  } CATCH(e) (void) e;
  ASSERT_EQ(0, self->i8deq_nomem.cap);
  ASSERT_EQ(0, i8deq_nomem_size(&self->i8deq_nomem));
  ASSERT_EQ(nil, self->i8deq_nomem.buf);

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
  ASSERT_EQ(SEQ_MIN_CAP, self->line.cap);
  ASSERT_EQ(0, line_size(&self->line));
  ASSERT_NEQ(nil, self->line.buf);
  line_dtor(&self->line, nil);
  line_ensure(&self->line, pow2_nextsize(SEQ_MIN_CAP + 1));
  ASSERT_EQ(pow2_next64(SEQ_MIN_CAP + 1), self->line.cap);
  ASSERT_EQ(0, line_size(&self->line));
  ASSERT_NEQ(nil, self->line.buf);
  return CUTE_SUCCESS;
}

CUTEST(deq, grow)
{
  i8deq_grow(&self->i8deq, 0);
  ASSERT_EQ(0, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_EQ(nil, self->i8deq.buf);
  i8deq_grow(&self->i8deq, 1);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  i8deq_grow(&self->i8deq, 2);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  i8deq_grow(&self->i8deq, SEQ_MIN_CAP);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  i8deq_grow(&self->i8deq, SEQ_MIN_CAP + 1);
  ASSERT_EQ(SEQ_MIN_CAP * 2, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  return CUTE_SUCCESS;
}

CUTEST(deq, shrink)
{
  i8deq_ensure(&self->i8deq, 2);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  i8deq_shrink(&self->i8deq, 2);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  i8deq_shrink(&self->i8deq, SEQ_MIN_CAP / 2);
  ASSERT_EQ(SEQ_MIN_CAP / 2, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  i8deq_shrink(&self->i8deq, SEQ_MIN_CAP / 4);
  ASSERT_EQ(SEQ_MIN_CAP / 4, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  i8deq_shrink(&self->i8deq, SEQ_MIN_CAP / 8);
  ASSERT_EQ(SEQ_MIN_CAP / 8, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  return CUTE_SUCCESS;
}

CUTEST(deq, trim)
{
  i8deq_ensure(&self->i8deq, 1);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  self->i8deq.len = SEQ_MIN_CAP;
  i8deq_trim(&self->i8deq);
  ASSERT_EQ(SEQ_MIN_CAP, self->i8deq.cap);
  ASSERT_EQ(SEQ_MIN_CAP, i8deq_size(&self->i8deq));
  ASSERT_NEQ(nil, self->i8deq.buf);
  while (self->i8deq.len) {
    --self->i8deq.len;
    i8deq_trim(&self->i8deq);
    ASSERT_EQ(self->i8deq.len, self->i8deq.cap);
    if (self->i8deq.len) {
      ASSERT_NEQ(nil, self->i8deq.buf);
    }
  }
  ASSERT_EQ(0, self->i8deq.cap);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_EQ(nil, self->i8deq.buf);
  return CUTE_SUCCESS;
}

CUTEST(deq, pushncpy)
{
  ex_t *e;

  ASSERT_EQ(nil, i8deq_pushncpy(&self->i8deq, arr4321, 0));
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_EQ(nil, self->i8deq.buf);
  ASSERT_NEQ(nil, i8deq_pushncpy(&self->i8deq, arr4321, 1));
  ASSERT_EQ(1, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_NEQ(nil, i8deq_pushncpy(&self->i8deq, arr4321, 2));
  ASSERT_EQ(3, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 2));
  ASSERT_NEQ(nil, i8deq_pushncpy(&self->i8deq, arr4321, 3));
  ASSERT_EQ(6, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 3));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 4));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 5));
  ASSERT_NEQ(nil, i8deq_pushncpy(&self->i8deq, arr4321, 4));
  ASSERT_EQ(10, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 3));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 4));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 5));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 6));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 7));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 8));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 9));
  TRY {
    i8deq_nomem_pushncpy(&self->i8deq_nomem, arr4321, 4);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(deq, push)
{
  u8_t i, j;
  ex_t *e;

  for (i = 0; i < 100; ++i) {
    *i8deq_push(&self->i8deq) = i;
    ASSERT_EQ(i + 1, i8deq_size(&self->i8deq));
    for (j = 0; j < i8deq_size(&self->i8deq); ++j) {
      ASSERT_EQ(j, *i8deq_at(&self->i8deq, (u8_t) j));
    }
  }
  TRY {
    i8deq_nomem_push(&self->i8deq_nomem);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(deq, pushcpy)
{
  u8_t i, j;
  ex_t *e;

  for (i = 0; i < 100; ++i) {
    i8deq_pushcpy(&self->i8deq, i);
    ASSERT_EQ(i + 1, i8deq_size(&self->i8deq));
    for (j = 0; j < i8deq_size(&self->i8deq); ++j) {
      ASSERT_EQ(j, *i8deq_at(&self->i8deq, (u8_t) j));
    }
  }
  TRY {
    i8deq_nomem_pushcpy(&self->i8deq_nomem, 0);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(deq, unshiftncpy)
{
  ex_t *e;

  ASSERT_EQ(nil, i8deq_unshiftncpy(&self->i8deq, arr4321, 0));
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_EQ(nil, self->i8deq.buf);
  ASSERT_NEQ(nil, i8deq_unshiftncpy(&self->i8deq, arr4321, 1));
  ASSERT_EQ(1, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_NEQ(nil, i8deq_unshiftncpy(&self->i8deq, arr4321, 2));
  ASSERT_EQ(3, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 2));
  ASSERT_NEQ(nil, i8deq_unshiftncpy(&self->i8deq, arr4321, 3));
  ASSERT_EQ(6, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 3));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 4));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 5));
  ASSERT_NEQ(nil, i8deq_unshiftncpy(&self->i8deq, arr4321, 4));
  ASSERT_EQ(10, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 3));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 4));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 5));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 6));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 7));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 8));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 9));
  TRY {
    i8deq_nomem_unshiftncpy(&self->i8deq_nomem, arr4321, 4);
    ASSERT(false);
  } CATCH(e) (void) e;
  ASSERT_EQ(0, self->i8deq.cur);
  ASSERT_EQ(4, i8deq_shiftn(&self->i8deq, 4, nil));
  ASSERT_EQ(6, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, self->i8deq.cur);
  i8deq_unshiftn(&self->i8deq, 2);
  ASSERT_EQ(8, i8deq_size(&self->i8deq));
  ASSERT_EQ(2, self->i8deq.cur);
  i8deq_unshiftn(&self->i8deq, 4);
  ASSERT_EQ(12, i8deq_size(&self->i8deq));
  ASSERT_EQ(0, self->i8deq.cur);
  ASSERT_EQ(2, i8deq_shiftn(&self->i8deq, 2, nil));
  ASSERT_EQ(10, i8deq_size(&self->i8deq));
  ASSERT_EQ(2, self->i8deq.cur);
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 3));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 4));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 5));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 6));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 7));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 8));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 9));
  return CUTE_SUCCESS;
}

CUTEST(deq, unshift)
{
  u8_t i, j;
  i8_t k;
  ex_t *e;

  for (i = 0; i < 100; ++i) {
    *i8deq_unshift(&self->i8deq) = i;
    ASSERT_EQ(i + 1, i8deq_size(&self->i8deq));
    for (j = 0, k = i; j < i8deq_size(&self->i8deq); ++j) {
      ASSERT_EQ(k--, *i8deq_at(&self->i8deq, (u8_t) j));
    }
  }
  TRY {
    i8deq_nomem_unshift(&self->i8deq_nomem);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(deq, unshiftcpy)
{
  u8_t i, j;
  i8_t k;
  ex_t *e;

  for (i = 0; i < 100; ++i) {
    i8deq_unshiftcpy(&self->i8deq, i);
    ASSERT_EQ(i + 1, i8deq_size(&self->i8deq));
    for (j = 0, k = i; j < i8deq_size(&self->i8deq); ++j) {
      ASSERT_EQ(k--, *i8deq_at(&self->i8deq, (u8_t) j));
    }
  }
  TRY {
    i8deq_nomem_unshiftcpy(&self->i8deq_nomem, 0);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(deq, putncpy)
{
  ex_t *e;

  ASSERT_EQ(nil, i8deq_putncpy(&self->i8deq, 0, arr4321, 0));
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_EQ(nil, self->i8deq.buf);
  ASSERT_NEQ(nil, i8deq_putncpy(&self->i8deq, 0, arr4321, 1));
  ASSERT_EQ(1, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_NEQ(nil, i8deq_putncpy(&self->i8deq, 1, arr4321, 2));
  ASSERT_EQ(3, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 2));
  ASSERT_NEQ(nil, i8deq_putncpy(&self->i8deq, 0, arr4321, 3));
  ASSERT_EQ(6, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 3));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 4));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 5));
  ASSERT_NEQ(nil, i8deq_putncpy(&self->i8deq, 3, arr4321, 4));
  ASSERT_EQ(10, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 3));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 4));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 5));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 6));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 7));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 8));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 9));
  TRY {
    i8deq_nomem_putncpy(&self->i8deq_nomem, 0, arr4321, 1);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(deq, put)
{
  ex_t *e;
  i8_t *ptr;

  ASSERT_NEQ(nil, ptr = i8deq_put(&self->i8deq, 0));
  ASSERT_EQ(1, i8deq_size(&self->i8deq));
  ASSERT_EQ(ptr, i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(*ptr = 2, *i8deq_at(&self->i8deq, 0));
  ASSERT_NEQ(nil, ptr = i8deq_put(&self->i8deq, 1));
  ASSERT_EQ(2, i8deq_size(&self->i8deq));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(ptr, i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(*ptr = 4, *i8deq_at(&self->i8deq, 1));
  ASSERT_NEQ(nil, ptr = i8deq_put(&self->i8deq, 0));
  ASSERT_EQ(3, i8deq_size(&self->i8deq));
  ASSERT_EQ(ptr, i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(*ptr = 1, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 2));
  ASSERT_NEQ(nil, ptr = i8deq_put(&self->i8deq, 2));
  ASSERT_EQ(4, i8deq_size(&self->i8deq));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(ptr, i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(*ptr = 3, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 3));
  ASSERT_EQ(nil, i8deq_put(&self->i8deq, 5));
  ASSERT_EQ(0, memcmp(i8deq_begin(&self->i8deq), arr1234, 4));
  TRY {
    i8deq_nomem_put(&self->i8deq_nomem, 0);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(deq, putcpy)
{
  ex_t *e;

  ASSERT_NEQ(nil, i8deq_putcpy(&self->i8deq, 0, 2));
  ASSERT_EQ(1, i8deq_size(&self->i8deq));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 0));
  ASSERT_NEQ(nil, i8deq_putcpy(&self->i8deq, 1, 4));
  ASSERT_EQ(2, i8deq_size(&self->i8deq));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 1));
  ASSERT_NEQ(nil, i8deq_putcpy(&self->i8deq, 0, 1));
  ASSERT_EQ(3, i8deq_size(&self->i8deq));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 2));
  ASSERT_NEQ(nil, i8deq_putcpy(&self->i8deq, 2, 3));
  ASSERT_EQ(4, i8deq_size(&self->i8deq));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 3));
  ASSERT_EQ(nil, i8deq_putcpy(&self->i8deq, 5, 5));
  TRY {
    i8deq_nomem_putcpy(&self->i8deq_nomem, 0, 1);
    ASSERT(false);
  } CATCH(e) (void) e;
  return CUTE_SUCCESS;
}

CUTEST(deq, pop)
{
  i8_t i;

  ASSERT_NEQ(nil, i8deq_pushncpy(&self->i8deq, arr4321, 4));
  ASSERT_EQ(4, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 3));
  ASSERT_EQ(true, i8deq_pop(&self->i8deq, nil));
  ASSERT_EQ(3, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(true, i8deq_pop(&self->i8deq, &i));
  ASSERT_EQ(2, i);
  ASSERT_EQ(2, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(true, i8deq_pop(&self->i8deq, &i));
  ASSERT_EQ(3, i);
  ASSERT_EQ(1, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(true, i8deq_pop(&self->i8deq, &i));
  ASSERT_EQ(4, i);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_EQ(false, i8deq_pop(&self->i8deq, &i));
  return CUTE_SUCCESS;
}

CUTEST(deq, shift)
{
  i8_t i;

  ASSERT_NEQ(nil, i8deq_pushncpy(&self->i8deq, arr4321, 4));
  ASSERT_EQ(4, i8deq_size(&self->i8deq));
  ASSERT_EQ(4, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 3));
  ASSERT_EQ(true, i8deq_shift(&self->i8deq, nil));
  ASSERT_EQ(3, i8deq_size(&self->i8deq));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(true, i8deq_shift(&self->i8deq, &i));
  ASSERT_EQ(3, i);
  ASSERT_EQ(2, i8deq_size(&self->i8deq));
  ASSERT_EQ(2, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(true, i8deq_shift(&self->i8deq, &i));
  ASSERT_EQ(2, i);
  ASSERT_EQ(1, i8deq_size(&self->i8deq));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(true, i8deq_shift(&self->i8deq, &i));
  ASSERT_EQ(1, i);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_EQ(false, i8deq_shift(&self->i8deq, &i));
  return CUTE_SUCCESS;
}

CUTEST(deq, rem)
{
  i8_t i;
  i8_t vi8[4] = { 0, 1, 2, 3 };

  ASSERT_EQ(false, i8deq_remove(&self->i8deq, 0, nil));
  ASSERT_NEQ(nil, i8deq_pushncpy(&self->i8deq, vi8, 4));
  ASSERT_EQ(false, i8deq_remove(&self->i8deq, 4, nil));
  ASSERT_EQ(true, i8deq_remove(&self->i8deq, 2, nil));
  ASSERT_EQ(3, i8deq_size(&self->i8deq));
  ASSERT_EQ(0, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(1, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 2));
  ASSERT_EQ(true, i8deq_remove(&self->i8deq, 1, &i));
  ASSERT_EQ(1, i);
  ASSERT_EQ(2, i8deq_size(&self->i8deq));
  ASSERT_EQ(0, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(3, *i8deq_at(&self->i8deq, 1));
  ASSERT_EQ(true, i8deq_remove(&self->i8deq, 1, &i));
  ASSERT_EQ(3, i);
  ASSERT_EQ(1, i8deq_size(&self->i8deq));
  ASSERT_EQ(0, *i8deq_at(&self->i8deq, 0));
  ASSERT_EQ(true, i8deq_remove(&self->i8deq, 0, &i));
  ASSERT_EQ(0, i);
  ASSERT_EQ(0, i8deq_size(&self->i8deq));
  ASSERT_EQ(false, i8deq_remove(&self->i8deq, 0, &i));
  return CUTE_SUCCESS;
}

i32_t
main(void)
{
  CUTEST_DATA test;

  CUTEST_PASS(deq, ensure);
  CUTEST_PASS(deq, grow);
  CUTEST_PASS(deq, shrink);
  CUTEST_PASS(deq, trim);
  CUTEST_PASS(deq, pushncpy);
  CUTEST_PASS(deq, push);
  CUTEST_PASS(deq, pushcpy);
  CUTEST_PASS(deq, unshiftncpy);
  CUTEST_PASS(deq, unshift);
  CUTEST_PASS(deq, unshiftcpy);
  CUTEST_PASS(deq, putncpy);
  CUTEST_PASS(deq, put);
  CUTEST_PASS(deq, putcpy);
  CUTEST_PASS(deq, pop);
  CUTEST_PASS(deq, shift);
  CUTEST_PASS(deq, rem);
  return EXIT_SUCCESS;
}
