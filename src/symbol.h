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

#ifndef   JL_SYMBOL_H__
# define  JL_SYMBOL_H__

#include <adt/hash.h>
#include <u/vector.h>

#include "entity.h"

typedef struct jl_sym_t jl_sym_t;
typedef struct jl_scope_t jl_scope_t;
typedef vec_of(jl_scope_t *) jl_scope_r;

KHASH_DECLARE(jl_symtab, const char *, jl_sym_t)

struct jl_sym_t {
  const char *id;
  unsigned flags;
  jl_scope_t *scope;
  jl_entity_t entity;
};

struct jl_scope_t {
  jl_scope_t *parent;
  jl_sym_t *sym;
  jl_symtab_t symtab;
  jl_scope_r childs;
};

void jl_sym_dtor(jl_sym_t *self);
void jl_scope_dtor(jl_scope_t *self);
bool jl_sym_has_flag(jl_sym_t *self, unsigned flag);
jl_sym_t *jl_sym_put(jl_scope_t *scope, const char *id, int *r);
jl_sym_t *jl_sym_get(jl_scope_t *scope, const char *id, int *r);

#endif /* JL_SYMBOL_H__ */
