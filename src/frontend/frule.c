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

#include "frule.h"

#include "fe.h"
#include "lexer.h"
#include "program.h"

bool jl_frule_validate(jl_frule_t self, jl_fval_t *fval, jl_fe_t *fe, jl_lexer_t *lexer, jl_program_t *out) {
  jl_token_t begin;
  bool validate;

  begin = jl_lexer_peek(lexer);
  validate = self.callback(fval, fe, lexer, out);
  jl_fval_begin(fval, begin);
  if (!validate || !(fval->kind & self.expected)) {
    jl_lexer_undo(lexer, fval->begin);
    return false;
  }
  jl_fval_end(fval, jl_lexer_peek(lexer));

  return true;
}