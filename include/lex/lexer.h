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

/*!@file lex/lexer.h
 * @author uael
 */
#ifndef __LEX_LEXER_H
# define __LEX_LEXER_H

#include <ev.h>
#include <ds/deq.h>

#include "tok.h"

enum lexer_ev {
  LEXER_ON_TOK_PUSH
};

typedef enum lexer_ev lexer_ev_t;
typedef struct lexer lexer_t;

DEQ32_DEFINE(toks, tok_t, addrcmp)
VEC32_DEFINE(vals, val_t, addrcmp)
DEQ16_DEFINE(srcs, src_t, addrcmp)

struct lexer {
  bool_t root;
  observers_t observers;
  errs_t errs;
  srcs_t srcs;
  lexer_t *origin;
  toks_t toks;
  vals_t vals;
};

OBSERVABLE_DEFINE(lex_lexer, lexer_t, lexer_ev_t)

__api__ void
lexer_init_file(lexer_t *self, char_t __const *filename);

__api__ void
lexer_init_stream(lexer_t *self, istream_t *stream);

__api__ void
lexer_init_str(lexer_t *self, char_t __const *buf);

__api__ void
lexer_init_nstr(lexer_t *self, char_t __const *buf, usize_t n);

#endif /* !__LEX_VAL_H */
