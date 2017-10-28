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

#include "ds/dstr.h"

#define NOMEM_REALLOC(x, y) ((errno = ENOMEM), nil)
DSTR_DEFINE_DFT(dstr8_nomem, 8, NOMEM_REALLOC, mem_free)

CUTEST_DATA { dstr8_t dstr8;dstr8_nomem_t dstr8_nomem; };

CUTEST_SETUP
{
  dstr8_ctor(&self->dstr8);
  dstr8_nomem_ctor(&self->dstr8_nomem);
}

CUTEST_TEARDOWN
{
  dstr8_dtor(&self->dstr8, nil);
  dstr8_nomem_dtor(&self->dstr8_nomem, nil);
}

CUTEST(dstr, ensure)
{
  ex_t *e;

  dstr8_ensure(&self->dstr8, 0);
  ASSERT_EQ(0, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_EQ(nil, self->dstr8.buf);
  dstr8_ensure(&self->dstr8, 1);
  ASSERT_EQ(1, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  dstr8_ensure(&self->dstr8, 1);
  ASSERT_EQ(1, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  dstr8_ensure(&self->dstr8, 2);
  ASSERT_EQ(2, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  dstr8_ensure(&self->dstr8, SEQ_MIN_CAP + 1);
  ASSERT_EQ(SEQ_MIN_CAP + 1, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  dstr8_ensure(&self->dstr8, U8_MAX);
  ASSERT_EQ(U8_MAX, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  dstr8_ensure(&self->dstr8, U8_MAX);
  ASSERT_EQ(U8_MAX, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);

  TRY {
    dstr8_nomem_ensure(&self->dstr8_nomem, 1);
    ASSERT(false);
  } CATCH(e);
  ASSERT_EQ(0, dstr8_nomem_cap(&self->dstr8_nomem));
  ASSERT_EQ(0, dstr8_nomem_size(&self->dstr8_nomem));
  ASSERT_EQ(nil, self->dstr8_nomem.buf);
  return CUTE_SUCCESS;
}

CUTEST(dstr, grow)
{
  dstr8_grow(&self->dstr8, 0);
  ASSERT_EQ(0, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_EQ(nil, self->dstr8.buf);
  dstr8_grow(&self->dstr8, 1);
  ASSERT_EQ(1, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  dstr8_grow(&self->dstr8, 2);
  ASSERT_EQ(2, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  dstr8_grow(&self->dstr8, SEQ_MIN_CAP);
  ASSERT_EQ(SEQ_MIN_CAP, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  dstr8_grow(&self->dstr8, SEQ_MIN_CAP + 1);
  ASSERT_EQ(SEQ_MIN_CAP + 1, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  return CUTE_SUCCESS;
}

CUTEST(dstr, shrink)
{
  dstr8_ensure(&self->dstr8, SEQ_MIN_CAP);
  ASSERT_EQ(SEQ_MIN_CAP, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  dstr8_shrink(&self->dstr8, 2);
  ASSERT_EQ(SEQ_MIN_CAP, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  dstr8_shrink(&self->dstr8, SEQ_MIN_CAP / 2);
  ASSERT_EQ(SEQ_MIN_CAP / 2, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  dstr8_shrink(&self->dstr8, SEQ_MIN_CAP / 4);
  ASSERT_EQ(SEQ_MIN_CAP / 4, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  dstr8_shrink(&self->dstr8, SEQ_MIN_CAP / 8);
  ASSERT_EQ(SEQ_MIN_CAP / 8, dstr8_cap(&self->dstr8));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_NEQ(nil, self->dstr8.buf);
  return CUTE_SUCCESS;
}

CUTEST(dstr, trim)
{
  return CUTE_SUCCESS;
}

CUTEST(dstr, insert)
{
  ex_t *e;

  ASSERT_EQ(true, dstr8_insert(&self->dstr8, 0, 2));
  ASSERT_EQ(1, dstr8_size(&self->dstr8));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(true, dstr8_insert(&self->dstr8, 1, 4));
  ASSERT_EQ(2, dstr8_size(&self->dstr8));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(true, dstr8_insert(&self->dstr8, 0, 1));
  ASSERT_EQ(3, dstr8_size(&self->dstr8));
  ASSERT_EQ(1, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(true, dstr8_insert(&self->dstr8, 2, 3));
  ASSERT_EQ(4, dstr8_size(&self->dstr8));
  ASSERT_EQ(1, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 3));
  ASSERT_EQ(false, dstr8_insert(&self->dstr8, 5, 5));
  TRY {
    dstr8_nomem_insert(&self->dstr8_nomem, 0, 1);
    ASSERT(false);
  } CATCH(e);
  return CUTE_SUCCESS;
}

CUTEST(dstr, emplace)
{
  ex_t *e;

  ASSERT_EQ(true, dstr8_emplace(&self->dstr8, 0, "a"));
  ASSERT_EQ(1, dstr8_size(&self->dstr8));
  ASSERT_EQ('a', dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(true, dstr8_emplace(&self->dstr8, 1, "ab"));
  ASSERT_EQ(3, dstr8_size(&self->dstr8));
  ASSERT_EQ('a', dstr8_at(&self->dstr8, 0));
  ASSERT_EQ('a', dstr8_at(&self->dstr8, 1));
  ASSERT_EQ('b', dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(true, dstr8_emplace(&self->dstr8, 0, "abc"));
  ASSERT_EQ(6, dstr8_size(&self->dstr8));
  ASSERT_EQ('a', dstr8_at(&self->dstr8, 0));
  ASSERT_EQ('b', dstr8_at(&self->dstr8, 1));
  ASSERT_EQ('c', dstr8_at(&self->dstr8, 2));
  ASSERT_EQ('a', dstr8_at(&self->dstr8, 3));
  ASSERT_EQ('a', dstr8_at(&self->dstr8, 4));
  ASSERT_EQ('b', dstr8_at(&self->dstr8, 5));
  ASSERT_EQ(true, dstr8_emplace(&self->dstr8, 3, "abcd"));
  ASSERT_EQ(10, dstr8_size(&self->dstr8));
  ASSERT_EQ('a', dstr8_at(&self->dstr8, 0));
  ASSERT_EQ('b', dstr8_at(&self->dstr8, 1));
  ASSERT_EQ('c', dstr8_at(&self->dstr8, 2));
  ASSERT_EQ('a', dstr8_at(&self->dstr8, 3));
  ASSERT_EQ('b', dstr8_at(&self->dstr8, 4));
  ASSERT_EQ('c', dstr8_at(&self->dstr8, 5));
  ASSERT_EQ('d', dstr8_at(&self->dstr8, 6));
  ASSERT_EQ('a', dstr8_at(&self->dstr8, 7));
  ASSERT_EQ('a', dstr8_at(&self->dstr8, 8));
  ASSERT_EQ('b', dstr8_at(&self->dstr8, 9));
  TRY {
    dstr8_nomem_emplace(&self->dstr8_nomem, 0, "a");
    ASSERT(false);
  } CATCH(e);
  return CUTE_SUCCESS;
}

CUTEST(dstr, push)
{
  u8_t i, j;
  ex_t *e;

  for (i = 0; i < 10; ++i) {
    ASSERT_EQ(true, dstr8_push(&self->dstr8, i));
    ASSERT_EQ(i + 1, dstr8_size(&self->dstr8));
    for (j = 0; j < dstr8_size(&self->dstr8); ++j) {
      ASSERT_EQ(j, dstr8_at(&self->dstr8, (u8_t) j));
    }
  }
  TRY {
    dstr8_nomem_push(&self->dstr8_nomem, 0);
    ASSERT(false);
  } CATCH(e);
  return CUTE_SUCCESS;
}

static char_t vchar0[1] = { 0 };
static char_t vchar40[2] = { 4, 0 };
static char_t vchar430[3] = { 4, 3, 0 };
static char_t vchar4320[4] = { 4, 3, 2, 0 };
static char_t vchar43210[5] = { 4, 3, 2, 1, 0 };

CUTEST(dstr, append)
{
  ex_t *e;

  ASSERT_EQ(true, dstr8_append(&self->dstr8, vchar0));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_EQ(nil, self->dstr8.buf);
  ASSERT_EQ(true, dstr8_append(&self->dstr8, vchar40));
  ASSERT_EQ(1, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(true, dstr8_append(&self->dstr8, vchar430));
  ASSERT_EQ(3, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(true, dstr8_append(&self->dstr8, vchar4320));
  ASSERT_EQ(6, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 3));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 4));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 5));
  ASSERT_EQ(true, dstr8_append(&self->dstr8, vchar43210));
  ASSERT_EQ(10, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 3));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 4));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 5));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 6));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 7));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 8));
  ASSERT_EQ(1, dstr8_at(&self->dstr8, 9));
  TRY {
    dstr8_nomem_append(&self->dstr8_nomem, vchar40);
    ASSERT(false);
  } CATCH(e);
  return CUTE_SUCCESS;
}

CUTEST(dstr, pop)
{
  char_t i;

  ASSERT_EQ(true, dstr8_append(&self->dstr8, vchar43210));
  ASSERT_EQ(4, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(1, dstr8_at(&self->dstr8, 3));
  ASSERT_EQ(true, dstr8_pop(&self->dstr8, nil));
  ASSERT_EQ(3, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(true, dstr8_pop(&self->dstr8, &i));
  ASSERT_EQ(2, i);
  ASSERT_EQ(2, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(true, dstr8_pop(&self->dstr8, &i));
  ASSERT_EQ(3, i);
  ASSERT_EQ(1, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(true, dstr8_pop(&self->dstr8, &i));
  ASSERT_EQ(4, i);
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_EQ(false, dstr8_pop(&self->dstr8, &i));
  return CUTE_SUCCESS;
}

CUTEST(dstr, unshift)
{
  u8_t i, j;
  i8_t k;
  ex_t *e;

  for (i = 0; i < 10; ++i) {
    ASSERT_EQ(true, dstr8_unshift(&self->dstr8, i));
    ASSERT_EQ(i + 1, dstr8_size(&self->dstr8));
    for (j = 0, k = i; j < dstr8_size(&self->dstr8); ++j) {
      ASSERT_EQ(k--, dstr8_at(&self->dstr8, (u8_t) j));
    }
  }
  TRY {
    dstr8_nomem_unshift(&self->dstr8_nomem, 0);
    ASSERT(false);
  } CATCH(e);
  return CUTE_SUCCESS;
}

CUTEST(dstr, prepend)
{
  ex_t *e;

  ASSERT_EQ(true, dstr8_prepend(&self->dstr8, vchar0));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_EQ(nil, self->dstr8.buf);
  ASSERT_EQ(true, dstr8_prepend(&self->dstr8, vchar40));
  ASSERT_EQ(1, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(true, dstr8_prepend(&self->dstr8, vchar430));
  ASSERT_EQ(3, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(true, dstr8_prepend(&self->dstr8, vchar4320));
  ASSERT_EQ(6, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 3));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 4));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 5));
  ASSERT_EQ(true, dstr8_prepend(&self->dstr8, vchar43210));
  ASSERT_EQ(10, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(1, dstr8_at(&self->dstr8, 3));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 4));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 5));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 6));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 7));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 9));
  TRY {
    dstr8_nomem_prepend(&self->dstr8_nomem, vchar43210);
    ASSERT(false);
  } CATCH(e);
  return CUTE_SUCCESS;
}

CUTEST(dstr, shift)
{
  char_t i;

  ASSERT_EQ(true, dstr8_append(&self->dstr8, vchar43210));
  ASSERT_EQ(4, dstr8_size(&self->dstr8));
  ASSERT_EQ(4, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(1, dstr8_at(&self->dstr8, 3));
  ASSERT_EQ(true, dstr8_shift(&self->dstr8, nil));
  ASSERT_EQ(3, dstr8_size(&self->dstr8));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(1, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(true, dstr8_shift(&self->dstr8, &i));
  ASSERT_EQ(3, i);
  ASSERT_EQ(2, dstr8_size(&self->dstr8));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(1, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(true, dstr8_shift(&self->dstr8, &i));
  ASSERT_EQ(2, i);
  ASSERT_EQ(1, dstr8_size(&self->dstr8));
  ASSERT_EQ(1, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(true, dstr8_shift(&self->dstr8, &i));
  ASSERT_EQ(1, i);
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_EQ(false, dstr8_shift(&self->dstr8, &i));
  return CUTE_SUCCESS;
}

CUTEST(dstr, remove)
{
  char_t i;
  char_t vchar[5] = { 5, 1, 2, 3, 0 };

  ASSERT_EQ(false, dstr8_remove(&self->dstr8, 0, nil));
  ASSERT_EQ(true, dstr8_append(&self->dstr8, vchar));
  ASSERT_EQ(false, dstr8_remove(&self->dstr8, 4, nil));
  ASSERT_EQ(true, dstr8_remove(&self->dstr8, 2, nil));
  ASSERT_EQ(3, dstr8_size(&self->dstr8));
  ASSERT_EQ(5, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(1, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(true, dstr8_remove(&self->dstr8, 1, &i));
  ASSERT_EQ(1, i);
  ASSERT_EQ(2, dstr8_size(&self->dstr8));
  ASSERT_EQ(5, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(true, dstr8_remove(&self->dstr8, 1, &i));
  ASSERT_EQ(3, i);
  ASSERT_EQ(1, dstr8_size(&self->dstr8));
  ASSERT_EQ(5, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(true, dstr8_remove(&self->dstr8, 0, &i));
  ASSERT_EQ(5, i);
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_EQ(false, dstr8_remove(&self->dstr8, 0, &i));
  return CUTE_SUCCESS;
}

CUTEST(dstr, removen)
{
  char_t buf[2], *ptr;
  char_t vchar[7] = { 7, 1, 2, 3, 5, 6, 0 };

  ptr = buf;
  ASSERT_EQ(0, dstr8_removen(&self->dstr8, 0, 0, nil));
  ASSERT_EQ(true, dstr8_append(&self->dstr8, vchar));
  ASSERT_EQ(0, dstr8_removen(&self->dstr8, 6, 0, nil));
  ASSERT_EQ(2, dstr8_removen(&self->dstr8, 2, 2, nil));
  ASSERT_EQ(4, dstr8_size(&self->dstr8));
  ASSERT_EQ(7, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(1, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(5, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(6, dstr8_at(&self->dstr8, 3));
  ASSERT_EQ(2, dstr8_removen(&self->dstr8, 1, 2, &ptr));
  ASSERT_EQ(2, dstr8_size(&self->dstr8));
  ASSERT_EQ(7, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(1, buf[0]);
  ASSERT_EQ(5, buf[1]);
  ASSERT_EQ(6, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(1, dstr8_removen(&self->dstr8, 0, 1, &ptr));
  ASSERT_EQ(1, dstr8_size(&self->dstr8));
  ASSERT_EQ(7, buf[0]);
  ASSERT_EQ(6, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(1, dstr8_removen(&self->dstr8, 0, 1, &ptr));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_EQ(6, buf[0]);
  ASSERT_EQ(0, dstr8_removen(&self->dstr8, 0, 1, &ptr));
  return CUTE_SUCCESS;
}

CUTEST(dstr, erase)
{
  char_t vchar[8] = { 1, 2, 1, 2, 3, 3, 2, 0 };

  ASSERT_EQ(0, dstr8_erase(&self->dstr8, 0));
  ASSERT_EQ(true, dstr8_append(&self->dstr8, vchar));
  ASSERT_EQ(2, dstr8_erase(&self->dstr8, 1));
  ASSERT_EQ(5, dstr8_size(&self->dstr8));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 3));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 4));
  ASSERT_EQ(3, dstr8_erase(&self->dstr8, 2));
  ASSERT_EQ(2, dstr8_size(&self->dstr8));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(2, dstr8_erase(&self->dstr8, 3));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_EQ(0, dstr8_erase(&self->dstr8, 3));
  return CUTE_SUCCESS;
}

CUTEST(dstr, erasen)
{
  char_t vchar[8] = { 1, 2, 1, 2, 3, 3, 2, 0 };

  ASSERT_EQ(0, dstr8_erasen(&self->dstr8, 0, 1));
  ASSERT_EQ(true, dstr8_append(&self->dstr8, vchar));
  ASSERT_EQ(0, dstr8_erasen(&self->dstr8, 1, 0));
  ASSERT_EQ(2, dstr8_erasen(&self->dstr8, 1, 3));
  ASSERT_EQ(5, dstr8_size(&self->dstr8));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 3));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 4));
  ASSERT_EQ(2, dstr8_erasen(&self->dstr8, 2, 2));
  ASSERT_EQ(3, dstr8_size(&self->dstr8));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(1, dstr8_erasen(&self->dstr8, 2, 2));
  ASSERT_EQ(2, dstr8_size(&self->dstr8));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(2, dstr8_erasen(&self->dstr8, 3, 2));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_EQ(0, dstr8_erasen(&self->dstr8, 3, 1));
  return CUTE_SUCCESS;
}

CUTEST(dstr, eraseonce)
{
  char_t vchar[8] = { 1, 2, 1, 2, 3, 3, 2, 0 };

  ASSERT_EQ(false, dstr8_eraseonce(&self->dstr8, 0));
  ASSERT_EQ(true, dstr8_append(&self->dstr8, vchar));
  ASSERT_EQ(true, dstr8_eraseonce(&self->dstr8, 1));
  ASSERT_EQ(true, dstr8_eraseonce(&self->dstr8, 1));
  ASSERT_EQ(false, dstr8_eraseonce(&self->dstr8, 1));
  ASSERT_EQ(5, dstr8_size(&self->dstr8));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 2));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 3));
  ASSERT_EQ(2, dstr8_at(&self->dstr8, 4));
  ASSERT_EQ(true, dstr8_eraseonce(&self->dstr8, 2));
  ASSERT_EQ(true, dstr8_eraseonce(&self->dstr8, 2));
  ASSERT_EQ(true, dstr8_eraseonce(&self->dstr8, 2));
  ASSERT_EQ(false, dstr8_eraseonce(&self->dstr8, 2));
  ASSERT_EQ(2, dstr8_size(&self->dstr8));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 0));
  ASSERT_EQ(3, dstr8_at(&self->dstr8, 1));
  ASSERT_EQ(true, dstr8_eraseonce(&self->dstr8, 3));
  ASSERT_EQ(true, dstr8_eraseonce(&self->dstr8, 3));
  ASSERT_EQ(false, dstr8_eraseonce(&self->dstr8, 3));
  ASSERT_EQ(0, dstr8_size(&self->dstr8));
  ASSERT_EQ(false, dstr8_eraseonce(&self->dstr8, 3));
  return CUTE_SUCCESS;
}

CUTEST(dstr, cpy)
{
  dstr8_t src;
  char_t vchar[8] = { 1, 2, 1, 2, 3, 3, 2, 0 };

  dstr8_ctor(&src);
  ASSERT_EQ(true, dstr8_append(&src, vchar));
  ASSERT_EQ(true, dstr8_cpy(&self->dstr8, &src));
  ASSERT_EQ(7, self->dstr8.len);
  ASSERT_EQ(src.len, self->dstr8.len);
  ASSERT_NEQ(src.buf, self->dstr8.buf);
  return CUTE_SUCCESS;
}

CUTEST(dstr, ncpy)
{
  dstr8_t src;
  char_t vchar[8] = { 7, 1, 2, 3, 4, 5, 6, 0 };

  dstr8_ctor(&src);
  ASSERT_EQ(true, dstr8_append(&src, vchar));
  ASSERT_EQ(true, dstr8_ncpy(&self->dstr8, &src, 5));
  ASSERT_EQ(5, self->dstr8.len);
  ASSERT_NEQ(src.len, self->dstr8.len);
  ASSERT_NEQ(src.buf, self->dstr8.buf);
  return CUTE_SUCCESS;
}

CUTEST(dstr, general)
{
  dstr8_t str = { 0 };

  dstr8_append(&str, "Hello world !");
  ASSERT_EQ(0, strcmp(str.buf, "Hello world !"));
  ASSERT_EQ(0, strcmp("Hello world !", str.buf));
  dstr8_emplace(&str, 6, "god damn ");
  ASSERT_EQ(0, strcmp(str.buf, "Hello god damn world !"));
  ASSERT_EQ(0, strcmp("Hello god damn world !", str.buf));
  dstr8_prepend(&str, "< ");
  dstr8_append(&str, " >");
  ASSERT_EQ(0, strcmp(str.buf, "< Hello god damn world ! >"));
  ASSERT_EQ(0, strcmp("< Hello god damn world ! >", str.buf));
  return CUTE_SUCCESS;
}

i32_t
main(void)
{
  CUTEST_DATA test;

  CUTEST_PASS(dstr, ensure);
  CUTEST_PASS(dstr, grow);
  CUTEST_PASS(dstr, shrink);
  CUTEST_PASS(dstr, trim);
  CUTEST_PASS(dstr, insert);
  CUTEST_PASS(dstr, emplace);
  CUTEST_PASS(dstr, push);
  CUTEST_PASS(dstr, append);
  CUTEST_PASS(dstr, pop);
  CUTEST_PASS(dstr, unshift);
  CUTEST_PASS(dstr, prepend);
  CUTEST_PASS(dstr, shift);
  CUTEST_PASS(dstr, remove);
  CUTEST_PASS(dstr, removen);
  CUTEST_PASS(dstr, erase);
  CUTEST_PASS(dstr, erasen);
  CUTEST_PASS(dstr, eraseonce);
  CUTEST_PASS(dstr, cpy);
  CUTEST_PASS(dstr, ncpy);
  return EXIT_SUCCESS;
}
