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

#include <stdio.h>

#include "fe.h"

#include "compiler.h"
#include "lexer.h"
#include "symbol.h"
#include "program.h"

#include "c/c_fe.h"

void jl_fe_init(jl_fe_t *self, enum jl_fe_n kind, jl_compiler_t *compiler) {
  *self = (jl_fe_t) {
    .compiler = compiler,
    .kind = kind,
    .scope = xmalloc(sizeof(jl_scope_t))
  };
  *self->scope = (jl_scope_t) {0};

  switch (self->kind) {
    case JL_FRONTEND_C:
      self->parse = c_fe_parse;
      break;
    case JL_FRONTEND_JAY:
      break;
    default:
      break;
  }
}

void jl_fe_dtor(jl_fe_t *self) {
  const char *src;

  adt_vector_foreach(self->sources, src) {
    free((void *) src);
    src = NULL;
  }
  adt_deque_dtor(self->sources);
  while (self->scope->parent) {
    self->scope = self->scope->parent;
  }
  jl_scope_dtor(self->scope);
  free(self->scope);
  self->scope = NULL;
  if (self->lexer) {
    jl_lexer_dtor(self->lexer, true);
    self->lexer = NULL;
  }
}

void jl_fe_parse(struct jl_fe_t *self, struct jl_lexer_t *lexer, struct jl_program_t *out) {
  jl_lexer_t l = (jl_lexer_t) {0};

  if (!lexer) {
    jl_lexer_init_f(&l, self);
    self->lexer = &l;
  }
  jl_program_init(out);
  self->parse(self, out);
  if (!lexer) {
    jl_lexer_dtor(&l, true);
    self->lexer = NULL;
  }
}

void jl_fe_push_src(jl_fe_t *self, const char *src) {
  const char *file;

#ifdef _MSC_VER
  // todo
  file = xstrdup(src);
#else
  file = xmalloc(FILENAME_MAX + 1);
  realpath(src, (char *) file);
#endif
  adt_deque_push(self->sources, file);
}

void jl_fe_scope(jl_fe_t *self) {
  jl_scope_t *parent;

  parent = self->scope;
  self->scope = xmalloc(sizeof(jl_scope_t));
  *self->scope = (jl_scope_t) {0};
  self->scope->parent = parent;
  adt_vector_push(parent->childs, self->scope);
}

void jl_fe_unscope(jl_fe_t *self) {
  self->scope = self->scope->parent;
}
