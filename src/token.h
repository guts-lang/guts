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

#ifndef   JL_TOKEN_H__
# define  JL_TOKEN_H__

#include <u/deque.h>
#include <adt/xmalloc.h>

#include "api.h"

struct jl_lexer_t;

typedef struct jl_loc jl_loc_t;
typedef struct jl_token jl_token_t;
typedef struct jl_rtoken jl_rtoken_t;
typedef struct jl_lloc jl_lloc_t;

typedef adt_deque_of(jl_token_t) jl_token_r;

enum {
  JL_TOKEN_UNDEFINED = 0,
  JL_TOKEN_KEYWORD,
  JL_TOKEN_SYNTAX,
  JL_TOKEN_NUMBER,
  JL_TOKEN_IDENTIFIER,
  JL_TOKEN_STRING_LITERAL
};

struct jl_loc {
  uint32_t lineno;
  uint32_t colno;
  uint32_t position;
  uint32_t file_id;
};

struct jl_token {
  unsigned int kind : 4;
  char type;
  const char *name, *value;
  size_t cursor;
  jl_loc_t loc;
  uint32_t length, leading_ws;
};

struct jl_rtoken {
  jl_token_t begin, end;
};

struct jl_lloc {
  struct jl_lexer_t *lexer;
  size_t begin, end;
};

#define jl_lloc(b, e) ((jl_lloc_t) {b.lexer, b.begin, e.end})
#define jl_no_lloc() ((jl_lloc_t) {NULL, 0, 0})

void jl_token_dtor(jl_token_t *self);

jl_lloc_t jl_lloc_begin(struct jl_lexer_t *lexer);
jl_lloc_t jl_lloc_end(jl_lloc_t self);
jl_rtoken_t jl_llocate(jl_lloc_t lloc);

#endif /* JL_TOKEN_H__ */
