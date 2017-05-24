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

#include "symbol.h"

void jl_sym_dtor(jl_sym_t *self) {}

void jl_scope_dtor(jl_scope_t *self) {
  jl_sym_t sym;
  jl_scope_t *scope;
  size_t i;

  kh_foreach_value(&self->symtab, sym, {
    jl_sym_dtor(&sym);
  });
  jl_symtab_dtor(&self->symtab);
  foreach(self->childs, i) {
    scope = ds_at(self->childs, i);
    jl_scope_dtor(scope);
    xfree(scope);
    ds_at(self->childs, i) = NULL;
  }
  vec_dtor(self->childs);
}

bool jl_sym_has_flag(jl_sym_t *self, unsigned flag) {
  return (bool) (self->flags & flag);
}

jl_sym_t *jl_sym_put(jl_scope_t *scope, const char *id, int *r) {
  jl_sym_t *sym;
  unsigned it;

  it = kh_put(jl_symtab, &scope->symtab, id, r);
  if (r == 0) {
    return NULL;
  }
  sym = &kh_value(&scope->symtab, it);
  sym->id = id;
  return sym;
}

jl_sym_t *jl_sym_get(jl_scope_t *scope, const char *id, int *r) {
  unsigned it;

  it = kh_get(jl_symtab, &scope->symtab, id);
  if (it == kh_end(&scope->symtab)) {
    *r = 1;
    return NULL;
  }
  *r = 0;
  return &kh_value(&scope->symtab, it);
}

KHASH_MAP_IMPL_STR(jl_symtab, jl_sym_t)
