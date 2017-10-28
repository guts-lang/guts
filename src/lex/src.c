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

#include "lex/src.h"

FORCEINLINE void
src_init_file(src_t *self, char_t __const *filename)
{
  init(self, src_t);
  self->loc.col = self->loc.line = 1;
  istream_open(&self->stream, filename);
}

FORCEINLINE void
src_init_str(src_t *self, char_t __const *str)
{
  init(self, src_t);
  self->loc.col = self->loc.line = 1;
  istream_memopen(&self->stream, str);
}

FORCEINLINE void
src_init_nstr(src_t *self, char_t __const *str, usize_t n)
{
  init(self, src_t);
  self->loc.col = self->loc.line = 1;
  istream_nmemopen(&self->stream, str, n);
}

FORCEINLINE char_t
src_peek(src_t *self, usize_t n)
{
  return istream_peek(&self->stream, n);
}

FORCEINLINE usize_t
src_get(src_t *self, char_t *buf, usize_t n)
{
  return istream_get(&self->stream, buf, n);
}

FORCEINLINE char_t
src_next(src_t *self)
{
  char_t c;

  getc_one:
  if (!(c = istream_getc(&self->stream))) {
    return c;
  }
  ++self->loc.cursor;
  switch (c) {
    case '\r': goto getc_one;
    case '\v': case '\f': case '\n':
      ++self->loc.line;
      self->loc.col = 1;
      break;
    default: ++self->loc.col;
      break;
  }
  return src_peek(self, 0);
}

FORCEINLINE void
src_dtor(src_t *self)
{
  istream_close(&self->stream);
}
