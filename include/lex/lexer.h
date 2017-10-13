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

enum lex_lexer_ev {
  LEX_LEXER_ON_TOK_PUSH
};

typedef enum lex_lexer_ev lex_lexer_ev_t;
typedef struct lex_lexer lex_lexer_t;

DEQ32_DEFINE(lex_toks, lex_tok_t, addrcmp)
VEC32_DEFINE(lex_vals, lex_val_t, addrcmp)
DEQ16_DEFINE(lex_srcs, lex_src_t, addrcmp)

struct lex_lexer {
  bool_t root;
  observers_t observers;
  err_stack_t errs;
  lex_srcs_t srcs;
  lex_lexer_t *origin;
  lex_toks_t toks;
  lex_vals_t vals;
};

OBSERVABLE_DEFINE(lex_lexer, lex_lexer_t, lex_lexer_ev_t)

__api__ ret_t
lex_file(lex_lexer_t *self, char_t __const *filename);

__api__ ret_t
lex_stream(lex_lexer_t *self, stream_t *stream);

__api__ ret_t
lex_str(lex_lexer_t *self, char_t __const *buf);

__api__ ret_t
lex_nstr(lex_lexer_t *self, char_t __const *buf, usize_t n);

#endif /* !__LEX_VAL_H */
