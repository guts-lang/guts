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

#include "fe/c/pp.h"

FORCEINLINE c_pp_t *
c_pp_new(void)
{
  c_pp_t *pp;

  if ((pp = mem_malloc(sizeof(c_pp_t))) == nil) {
    THROW(ex_errno(ERRLVL_FATAL, errno, nil));
  }
  init(pp, c_pp_t);
  c_macros_ctor(&pp->macros);
  pp->update = c_pp_update;
  pp->dtor = c_pp_dtor;
  return pp;
}

FORCEINLINE void
c_pp_dtor(c_pp_t *self)
{
  c_macros_dtor(&self->macros, nil, c_macro_dtor);
  mem_free(self);
}

FORCEINLINE bool_t
c_pp_update(c_pp_t *self, lexer_t *lexer, lexer_ev_t code, void *arg)
{
  (void) self;
  (void) lexer;
  (void) code;
  (void) arg;
  return true;
}
