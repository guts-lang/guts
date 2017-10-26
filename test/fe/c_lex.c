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

#include "fe/c/lex.h"

#define TOKS_COUNT 21

static struct { val_kind_t k; char_t *v; } vals[] = {
  { VAL_IDENT, "main" },
  { VAL_IDENT, "i" },
  { VAL_I64, (char_t *) 1 },
  { VAL_I64, (char_t *) 2 },
  { VAL_IDENT, "i" },
  { VAL_I64, (char_t *) 3 },
  { VAL_IDENT, "EXIT_SUCCESS" },
};

static tok_t toks[TOKS_COUNT] = {
  { TOK_KEYWORD, 0, C_TOK_INT, { 0, 3, 1, 1, 0 } },
  { TOK_VALUE, 0, 0, { 0, 4, 2, 1, 4 } },
  { TOK_SYNTAX, 0, '(', { 0, 1, 2, 5, 8 } },
  { TOK_KEYWORD, 0, C_TOK_VOID, { 0, 4, 2, 6, 9 } },
  { TOK_SYNTAX, 0, ')', { 0, 1, 2, 10, 13 } },
  { TOK_SYNTAX, 0, '{', { 0, 1, 3, 1, 15 } },
  { TOK_KEYWORD, 2, C_TOK_INT, { 0, 3, 4, 3, 19 } },
  { TOK_VALUE, 1, 1, { 0, 1, 4, 7, 23 } },
  { TOK_SYNTAX, 1, '=', { 0, 1, 4, 9, 25 } },
  { TOK_VALUE, 1, 2, { 0, 1, 4, 11, 27 } },
  { TOK_SYNTAX, 1, '*', { 0, 1, 4, 13, 29 } },
  { TOK_VALUE, 1, 3, { 0, 1, 4, 15, 31 } },
  { TOK_SYNTAX, 0, ';', { 0, 1, 4, 16, 32 } },
  { TOK_VALUE, 2, 4, { 0, 1, 4, 19, 35 } },
  { TOK_PONCT, 1, C_TOK_MUL_ASSIGN, { 0, 1, 4, 21, 37 } },
  { TOK_VALUE, 1, 5, { 0, 1, 4, 24, 40 } },
  { TOK_SYNTAX, 0, ';', { 0, 1, 4, 25, 41 } },
  { TOK_KEYWORD, 2, C_TOK_RETURN, { 0, 6, 4, 28, 44 } },
  { TOK_VALUE, 1, 6, { 0, 12, 4, 35, 51 } },
  { TOK_SYNTAX, 0, ';', { 0, 1, 4, 47, 63 } },
  { TOK_SYNTAX, 0, '}', { 0, 1, 5, 1, 65 } },
};

CUTEST_DATA {
  lexer_t lexer;
};

CUTEST_SETUP {
  lexer_init_str(&self->lexer,
    "int\n"
      "main(void)\n"
      "{\n"
      "  int i = 1 * 2;"
      "  i *= 3;"
      "  return EXIT_SUCCESS;\n"
      "}"
  );
  lexer_lex_c(&self->lexer);
}
CUTEST_TEARDOWN {
  lexer_dtor(&self->lexer);
  cout_flush();
}

CUTEST(fe_c, toks) {
  u8_t i;

  ASSERT_EQ(TOKS_COUNT, lexer_scan(&self->lexer, 50));
  for (i = 0; i < 4; ++i) {
    tok_t tok;

    ASSERT(toks_shift(&self->lexer.toks, &tok));
    ASSERT_EQ(toks[i].kind, tok.kind);
    ASSERT_EQ(toks[i].lws, tok.lws);
    ASSERT_EQ(toks[i].id, tok.id);
    ASSERT_EQ(toks[i].loc.src, tok.loc.src);
    ASSERT_EQ(toks[i].loc.len, tok.loc.len);
    ASSERT_EQ(toks[i].loc.line, tok.loc.line);
    ASSERT_EQ(toks[i].loc.col, tok.loc.col);
    ASSERT_EQ(toks[i].loc.cursor, tok.loc.cursor);
    if (tok.kind == TOK_VALUE) {
      ASSERT_EQ(vals[tok.id].k, self->lexer.vals.buf[tok.id].kind);
      switch (vals[tok.id].k) {
        case VAL_IDENT:
          ASSERT_SEQ(
            vals[tok.id].v, self->lexer.vals.buf[tok.id].val.ident.buf
          );
          break;
        case VAL_I64:
          ASSERT_EQ(
            (i64_t) vals[tok.id].v, self->lexer.vals.buf[tok.id].val.i64
          );
          break;
        default: break;
      }
    }
  }
  return CUTE_SUCCESS;
}

i32_t
main(void)
{
  CUTEST_DATA test;

  init(&test, CUTEST_DATA);
  CUTEST_PASS(fe_c, toks);
  return EXIT_SUCCESS;
}
