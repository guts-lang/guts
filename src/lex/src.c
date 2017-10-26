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

#include <lex.h>
#include "lex/src.h"

FORCEINLINE void
src_init_file(src_t *self, char_t __const *filename)
{
  init(self, src_t);
  self->kind = LEX_SRC_FILE;
  self->loc.col = self->loc.line = 1;
  istream_open(&self->src.file, filename);
}

FORCEINLINE void
src_init_stream(src_t *self, istream_t *stream)
{
  init(self, src_t);
  self->kind = LEX_SRC_STREAM;
  self->loc.col = self->loc.line = 1;
  self->src.stream = stream;
}

FORCEINLINE void
src_init_str(src_t *self, char_t __const *buf)
{
  init(self, src_t);
  self->kind = LEX_SRC_STR;
  self->loc.col = self->loc.line = 1;
  self->src.str.len = strlen(buf);
  self->src.str.buf = buf;
}

FORCEINLINE void
src_init_nstr(src_t *self, char_t __const *buf, usize_t n)
{
  init(self, src_t);
  self->kind = LEX_SRC_STR;
  self->loc.col = self->loc.line = 1;
  self->src.str.len = n;
  self->src.str.buf = buf;
}

FORCEINLINE char_t
src_peek(src_t *self, usize_t n)
{
  switch (self->kind) {
    case LEX_SRC_FILE:return istream_peek(&self->src.file, n);
    case LEX_SRC_STREAM:return istream_peek(self->src.stream, n);
    case LEX_SRC_STR:
      if (self->src.str.cursor + n > self->src.str.len)
        return '\0';
      return self->src.str.buf[self->src.str.cursor + n];
  }
  return '\0';
}

FORCEINLINE usize_t
src_get(src_t *self, char_t *buf, usize_t n)
{
  switch (self->kind) {
    case LEX_SRC_FILE: return istream_get(&self->src.file, buf, n);
    case LEX_SRC_STREAM: return istream_get(self->src.stream, buf, n);
    case LEX_SRC_STR: {
      usize_t c;

      c =  self->src.str.len - self->src.str.cursor;
      if (c > 0) {
        if (c > n) c = n;
        memcpy(buf, self->src.str.buf + self->src.str.cursor, c);
      }
      return c;
    }
  }
  return 0;
}

FORCEINLINE char_t
src_next(src_t *self)
{
  char_t c;

  getc_one:
  c = '\0';
  switch (self->kind) {
    case LEX_SRC_FILE: c = istream_getc(&self->src.file);
      break;
    case LEX_SRC_STREAM: c = istream_getc(self->src.stream);
      break;
    case LEX_SRC_STR:
      if (self->src.str.cursor > self->src.str.len)
        return '\0';
      c = self->src.str.buf[self->src.str.cursor++];
      break;
  }
  ++self->loc.cursor;
  switch (c) {
    case '\0': return c;
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
  switch (self->kind) {
    case LEX_SRC_FILE: istream_close(&self->src.file);
    default: break;
  }
}
