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

CUTEST_DATA {
  lexer_t lexer;
};

CUTEST_SETUP {
  lexer_init_str(&self->lexer,
    "int\n"
      "main(void)\n"
      "{\n"
      "  int i = 1 * 2;\n"
      "  i *= 3;\n"
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
  ostream_t s;

  ostream_memopen(&s);
  ASSERT_EQ(TOKS_COUNT, lexer_scan(&self->lexer, 50));
  for (i = 0; i < TOKS_COUNT; ++i) {
    tok_t tok;

    ASSERT(toks_shift(&self->lexer.toks, &tok));
    lexer_tok_dump(&self->lexer, &tok, &s);
  }
  ASSERT_EQ(48, s.u.mem.len);
  ASSERT_SEQ("intmain(void){inti=1*2;i*=3;returnEXIT_SUCCESS;}", s.u.mem.buf);
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
