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

FORCEINLINE ret_t
lex_src_file(lex_src_t *self, char_t __const *filename) {
  init(self, lex_src_t);
  self->kind = LEX_SRC_FILE;
  self->loc.col = self->loc.line = 1;
  return stream_open(&self->src.file, filename, FS_OPEN_RO);
}

FORCEINLINE ret_t
lex_src_stream(lex_src_t *self, stream_t *stream) {
  init(self, lex_src_t);
  self->kind = LEX_SRC_STREAM;
  self->loc.col = self->loc.line = 1;
  self->src.stream = stream;
  return RET_SUCCESS;
}

FORCEINLINE ret_t
lex_src_str(lex_src_t *self, char_t __const *buf) {
  init(self, lex_src_t);
  self->kind = LEX_SRC_STR;
  self->loc.col = self->loc.line = 1;
  self->src.str.len = strlen(buf);
  self->src.str.buf = buf;
  return RET_SUCCESS;
}

FORCEINLINE ret_t
lex_src_nstr(lex_src_t *self, char_t __const *buf, usize_t n) {
  init(self, lex_src_t);
  self->kind = LEX_SRC_STR;
  self->loc.col = self->loc.line = 1;
  self->src.str.len = n;
  self->src.str.buf = buf;
  return RET_SUCCESS;
}

FORCEINLINE ret_t
lex_src_peek(lex_src_t *self, usize_t n, char_t *out) {
  ret_t ret;

  switch (self->kind) {
    case LEX_SRC_FILE:
      if ((ret = stream_peek(&self->src.file, n, out)) > 0) return ret;
      break;
    case LEX_SRC_STREAM:
      if ((ret = stream_peek(self->src.stream, n, out)) > 0) return ret;
      break;
    case LEX_SRC_STR:
      if (self->src.str.cursor + n > self->src.str.len) return RET_FAILURE;
      *out = self->src.str.buf[self->src.str.cursor + n];
      break;
  }
  return *out ? RET_SUCCESS : RET_FAILURE;
}

FORCEINLINE ret_t
lex_src_next(lex_src_t *self, char_t *out) {
  ret_t ret;
  char_t c;

  getc_one:
  c = '\0';
  switch (self->kind) {
    case LEX_SRC_FILE:
      if ((ret = stream_getc(&self->src.file, &c)) > 0) return ret;
      break;
    case LEX_SRC_STREAM:
      if ((ret = stream_getc(self->src.stream, &c)) > 0) return ret;
      break;
    case LEX_SRC_STR:
      if (self->src.str.cursor > self->src.str.len) return RET_FAILURE;
      c = self->src.str.buf[self->src.str.cursor++];
      break;
  }
  ++self->loc.cursor;
  switch (c) {
    case '\0': return RET_FAILURE;
    case '\r':
      goto getc_one;
    case '\v':
    case '\f':
    case '\n':
      ++self->loc.line;
      self->loc.col = 0;
      break;
    default: ++self->loc.col;
      break;
  }
  return lex_src_peek(self, 0, out);
}
