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

/*!@file ulex/lexer.h
 * @author uael
 */
#ifndef __ULEX_LEXER_H
# define __ULEX_LEXER_H

#include <uev.h>
#include <uds/deq.h>

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

__api__ void
lex_lexer_ctor(lex_lexer_t *self);

__api__ void
lex_lexer_dtor(lex_lexer_t *self);

__api__ ret_t
lex_lexer_push_file(lex_lexer_t *self, i8_t __const *filename);

__api__ ret_t
lex_lexer_push_buf(lex_lexer_t *self, i8_t __const *buf, u64_t len);

__api__ ret_t
lex_lexer_fork(lex_lexer_t *fork, lex_lexer_t *origin);

__api__ ret_t
lex_lexer_join(lex_lexer_t *fork);

__api__ ret_t
lex_lexer_push(lex_lexer_t *self, lex_tok_t tok);

__api__ lex_tok_t
lex_lexer_peek(lex_lexer_t *self);

__api__ lex_tok_t
lex_lexer_npeek(lex_lexer_t *self, u16_t n);

__api__ lex_tok_t
lex_lexer_next(lex_lexer_t *self);

__api__ lex_tok_t
lex_lexer_consume(lex_lexer_t *self, u32_t kind);

#endif /* !__ULEX_VAL_H */
