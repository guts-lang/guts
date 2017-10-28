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

#include "lex/ex.h"
#include "lex/lexer.h"

i32_t lex_ex_code;

ex_t
lex_ex(loc_t __const *loc, errlvl_t lvl,
  char_t __const *fmt, ...)
{
  ex_t e;
  src_t *src;
  va_list ap;

  EX_REGISTER(lex_ex);
  src = loc->src;
  va_start(ap, fmt);
  ex_ctor(&e, lvl, lex_ex_code, fmt, ap);
  va_end(ap);
  if (src->stream.kind == ISTREAM_FILE) {
    e.file = src->stream.u.file.filename;
  }
  e.line = loc->line;
  e.col = loc->col;
  return e;
}

FORCEINLINE ex_t
lex_inval_tok_ex(struct lexer *lexer, tok_t *got, u32_t expected)
{

  return lex_ex(&got->loc, ERRLVL_ERROR,
    "Unexpected token, expected '%s' got '%s'",
    lexer->tok_str(expected), lexer->tok_str(got->id)
  );
}
