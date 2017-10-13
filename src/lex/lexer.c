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

#include "lex/lexer.h"

FORCEINLINE ret_t
lex_file(lex_lexer_t *self, char_t __const *filename) {
  ret_t ret;
  lex_src_t src;

  if ((ret = lex_src_file(&src, filename)) > 0) return ret;
  src.loc.src = self->srcs.head;
  return lex_srcs_unshift(&self->srcs, src);
}

FORCEINLINE ret_t
lex_stream(lex_lexer_t *self, stream_t *stream) {
  ret_t ret;
  lex_src_t src;

  if ((ret = lex_src_stream(&src, stream)) > 0) return ret;
  src.loc.src = self->srcs.head;
  return lex_srcs_unshift(&self->srcs, src);
}

FORCEINLINE ret_t
lex_str(lex_lexer_t *self, char_t __const *buf) {
  ret_t ret;
  lex_src_t src;

  if ((ret = lex_src_str(&src, buf)) > 0) return ret;
  src.loc.src = self->srcs.head;
  return lex_srcs_unshift(&self->srcs, src);
}

FORCEINLINE ret_t
lex_nstr(lex_lexer_t *self, char_t __const *buf, usize_t n) {
  ret_t ret;
  lex_src_t src;

  if ((ret = lex_src_nstr(&src, buf, n)) > 0) return ret;
  src.loc.src = self->srcs.head;
  return lex_srcs_unshift(&self->srcs, src);
}
