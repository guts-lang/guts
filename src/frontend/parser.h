/*
 * MIT License
 *
 * Copyright (c) 2016-2017 uael <www.github.com/uael>
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

#ifndef   JL_PARSER_H__
# define  JL_PARSER_H__

#include <u/deque.h>

#include "lexer.h"

struct jl_scope_t;
struct jl_program_t;

typedef struct jl_parser_t jl_parser_t;

enum jl_parser_n {
  JL_PARSER_UNDEFINED = 0,
  JL_PARSER_C,
  JL_PARSER_JAY
};

struct jl_parser_t {
  enum jl_parser_n kind;
  struct jl_compiler_t *compiler;
  struct jl_scope_t *scope;
  jl_lexer_t *lexer;
  adt_deque_of(const char *) sources;

  void (*parse)(jl_parser_t *self, struct jl_program_t *out);
};

void jl_fe_init(jl_parser_t *self, enum jl_parser_n kind, struct jl_compiler_t *compiler);
void jl_fe_dtor(jl_parser_t *self);
void jl_fe_parse(jl_parser_t *self, jl_lexer_t *lexer, struct jl_program_t *out);
void jl_fe_push_src(jl_parser_t *self, const char *src);
void jl_fe_scope(jl_parser_t *self);
void jl_fe_unscope(jl_parser_t *self);

#endif /* JL_PARSER_H__ */
