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

#include "ulex/lexer.h"

void
lex_lexer_ctor(lex_lexer_t *self) {
  *self = init (lex_lexer_t, true);
}

void
lex_lexer_dtor(lex_lexer_t *self) {
  lex_lexer_detach(self);
  err_stack_dtor(&self->errs);
  lex_toks_dtor(&self->toks);
  lex_vals_dtor(&self->vals);
}

ret_t
lex_lexer_push_file(lex_lexer_t *self, i8_t __const *filename) {
  (void) self;
  (void) filename;
  return RET_SUCCESS;
}

ret_t
lex_lexer_push_buf(lex_lexer_t *self, i8_t __const *buf, u64_t len) {
  (void) self;
  (void) buf;
  (void) len;
  return RET_SUCCESS;
}

ret_t
lex_lexer_fork(lex_lexer_t *fork, lex_lexer_t *origin) {
  fork->origin = origin;

  return RET_SUCCESS;
}

ret_t
lex_lexer_join(lex_lexer_t *fork) {
  (void) fork;
  return RET_SUCCESS;
}

ret_t
lex_lexer_push(lex_lexer_t *self, lex_tok_t tok) {
  (void) self;
  (void) tok;
  return RET_SUCCESS;
}

lex_tok_t
lex_lexer_peek(lex_lexer_t *self) {
  (void) self;
  return init (lex_tok_t, 0);
}

lex_tok_t
lex_lexer_npeek(lex_lexer_t *self, u16_t n) {
  (void) self;
  (void) n;
  return init (lex_tok_t, 0);
}

lex_tok_t
lex_lexer_next(lex_lexer_t *self) {
  (void) self;
  return init (lex_tok_t, 0);
}

lex_tok_t
lex_lexer_consume(lex_lexer_t *self, u32_t kind) {
  (void) self;
  (void) kind;
  return init (lex_tok_t, 0);
}
