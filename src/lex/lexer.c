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

#include <lex.h>
#include "lex/lexer.h"

FORCEINLINE void
lexer_ctor(lexer_t *self)
{
  init(self, lexer_t);
  toks_ctor(&self->toks);
  tokvals_ctor(&self->tokvals);
  srcs_ctor(&self->srcs);
  lrules_ctor(&self->rules);
}

FORCEINLINE void
lexer_dtor(lexer_t *self)
{
  toks_dtor(&self->toks, nil);
  tokvals_dtor(&self->tokvals, tokval_dtor);
  srcs_dtor(&self->srcs, src_dtor);
  lrules_dtor(&self->rules, nil);
}

FORCEINLINE void
lexer_init_file(lexer_t *self, char_t __const *filename)
{
  src_t src;

  lexer_ctor(self);

  src_init_file(&src, filename);
  srcs_unshift(&self->srcs, src);
  src.loc.src = self->srcs.buf + self->srcs.head;
}

FORCEINLINE void
lexer_init_str(lexer_t *self, char_t __const *buf)
{
  src_t src;

  lexer_ctor(self);
  src_init_str(&src, buf);
  srcs_unshift(&self->srcs, src);
  src.loc.src = self->srcs.buf + self->srcs.head;
}

FORCEINLINE void
lexer_init_nstr(lexer_t *self, char_t __const *buf, usize_t n)
{
  src_t src;

  lexer_ctor(self);
  src_init_nstr(&src, buf, n);
  srcs_unshift(&self->srcs, src);
  src.loc.src = self->srcs.buf + self->srcs.head;
}

FORCEINLINE usize_t
lexer_scan(lexer_t *self, usize_t n)
{
  usize_t c;

  for (c = 0; c < n;) {
    tok_t tok;
    tokval_t *val;
    src_t *src;
    char_t peek;
    lrule_t *rule;
    u8_t i;

    if (srcs_size(&self->srcs) == 0) {
      break;
    }
    src = srcs_offset(&self->srcs, 0);
    if ((peek = src_peek(src, 0)) == '\0') {
      srcs_shift(&self->srcs, nil);
      continue;
    }
    init(&tok, tok_t);
    while (peek == ' ' || peek == '\t') {
      ++tok.lws;
      peek = src_next(src);
    }
    tok.loc = src->loc;
    tokvals_grow(&self->tokvals, 1);
    val = self->tokvals.buf + self->tokvals.len;
    init(val, tokval_t);
    tok.val = val;
    foreach (i, rule, invec(self->rules)) {
      if ((*rule)(&tok, peek, src)) {
        tok.loc.len = (u8_t) (src->loc.cursor - tok.loc.cursor);
        if (lexer_notify(self, LEXER_ON_TOK_PUSH, &tok)) {
          if (!tok.is_id) ++self->tokvals.len;
          toks_push(&self->toks, tok);
          ++c;
          break;
        }
      }
    }
  }
  return c;
}

__api__ char_t __const *
lexer_tok_tostr(lexer_t *self, tok_t *tok)
{
  if (tok->is_id)
    return self->tok_str(tok->id);
  else {
    switch (tok->val->kind) {
      case TOKVAL_STR: return "String";
      case TOKVAL_IDENT: return "Identifier";
      case TOKVAL_I8: case TOKVAL_I16: case TOKVAL_I32: case TOKVAL_I64:
      case TOKVAL_U8: case TOKVAL_U16: case TOKVAL_U32: case TOKVAL_U64:
      case TOKVAL_F32: case TOKVAL_F64:
        return "Number";
      default:
        return "Unknown";
    }
  }
}

FORCEINLINE void
lexer_tok_dump(lexer_t *self, tok_t *tok, ostream_t *stream)
{
  if (tok->is_id) {
    char_t __const *str;

    if ((str = self->tok_str(tok->id)) != nil)
      ostream_puts(stream, str);
    else
      ostream_putc(stream, (char_t) tok->id);
  } else {
    tokval_dump(tok->val, stream);
  }
}
