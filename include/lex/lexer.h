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
#include <ds/vec.h>
#include <fs/ostream.h>

#include "tok.h"

#define MCAR(str, off, at, c) \
  ((str)[(at) + (off)] == (c))
#define MSTR1(str, off, a) \
  MCAR(str, off, 0, a)
#define MSTR2(str, off, a, b) \
  (MSTR1(str, off, a) && MCAR(str, off, 1, b))
#define MSTR3(str, off, a, b, c) \
  (MSTR2(str, off, a, b) && MCAR(str, off, 2, c))
#define MSTR4(str, off, a, b, c, d) \
  (MSTR3(str, off, a, b, c) && MCAR(str, off, 3, d))
#define MSTR5(str, off, a, b, c, d, e) \
  (MSTR4(str, off, a, b, c, d) && MCAR(str, off, 4, e))
#define MSTR6(str, off, a, b, c, d, e, f) \
  (MSTR5(str, off, a, b, c, d, e) && MCAR(str, off, 5, f))
#define MSTR7(str, off, a, b, c, d, e, f, g) \
  (MSTR6(str, off, a, b, c, d, e, f) && MCAR(str, off, 6, g))
#define MSTR8(str, off, a, b, c, d, e, f, g, h) \
  (MSTR7(str, off, a, b, c, d, e, f, g) && MCAR(str, off, 7, h))

#define MSTR(str, off, ...) \
  PP_MCALL(PP_JOIN(MSTR, PP_VA_NARGS(__VA_ARGS__)), str, off, __VA_ARGS__)

#define MSTRN1(str, len, off, a) \
  ((len) == (1+(off)) && MSTR1(str, off, a))
#define MSTRN2(str, len, off, a, b) \
  ((len) == (2+(off)) && MSTR2(str, off, a, b))
#define MSTRN3(str, len, off, a, b, c) \
  ((len) == (3+(off)) && MSTR3(str, off, a, b, c))
#define MSTRN4(str, len, off, a, b, c, d) \
  ((len) == (4+(off)) && MSTR4(str, off, a, b, c, d))
#define MSTRN5(str, len, off, a, b, c, d, e) \
  ((len) == (5+(off)) && MSTR5(str, off, a, b, c, d, e))
#define MSTRN6(str, len, off, a, b, c, d, e, f) \
  ((len) == (6+(off)) && MSTR6(str, off, a, b, c, d, e, f))
#define MSTRN7(str, len, off, a, b, c, d, e, f, g) \
  ((len) == (7+(off)) && MSTR7(str, off, a, b, c, d, e, f, g))
#define MSTRN8(str, len, off, a, b, c, d, e, f, g, h) \
  ((len) == (8+(off)) && MSTR8(str, off, a, b, c, d, e, f, g, h))

#define MSTRN(str, len, off, ...) \
  PP_MCALL(PP_JOIN(MSTRN, PP_VA_NARGS(__VA_ARGS__)), str, len, off, __VA_ARGS__)

enum lexer_ev {
  LEXER_ON_TOK_PUSH
};

typedef enum lexer_ev lexer_ev_t;
typedef bool_t (*lrule_t)(tok_t *, char_t, val_t *, src_t *);
typedef struct lexer lexer_t;

VEC8_DEFINE(lrules, lrule_t, addrcmp)

struct lexer {
  bool_t root;
  observers_t observers;
  errs_t errs;
  srcs_t srcs;
  lexer_t *origin;
  toks_t toks;
  vals_t vals;
  lrules_t rules;
  char_t __const *(*tok_str)(tok_t *);
};

OBSERVABLE_DEFINE(lexer, lexer_t, lexer_ev_t)

__api__ void
lexer_ctor(lexer_t *self);

__api__ void
lexer_dtor(lexer_t *self);

__api__ void
lexer_init_file(lexer_t *self, char_t __const *filename);

__api__ void
lexer_init_stream(lexer_t *self, istream_t *stream);

__api__ void
lexer_init_str(lexer_t *self, char_t __const *buf);

__api__ void
lexer_init_nstr(lexer_t *self, char_t __const *buf, usize_t n);

__api__ usize_t
lexer_scan(lexer_t *self, usize_t n);

__api__ void
lexer_tok_dump(lexer_t *self, tok_t *tok, ostream_t *stream);

#endif /* !__LEX_VAL_H */
