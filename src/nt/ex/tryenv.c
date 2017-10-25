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

#include "nt/ex/tryenv.h"

static __thread_local tryenv_t tryenvs[TRYENV_SIZE] = { 0 };
static __thread_local u16_t tryenv_idx = 0;

FORCEINLINE tryenv_t *
tryenv_push(void)
{
  return tryenvs + tryenv_idx++;
}

FORCEINLINE ex_t *
tryenv_pop(void)
{
  tryenv_t *env;

  if (tryenv_idx) {
    env = tryenvs + --tryenv_idx;
    if (env->e.code) {
      return &env->e;
    }
  }
  return nil;
}

FORCEINLINE NORETURN void
tryenv_throw(ex_t e, char_t __const *fn, char_t __const *file, u32_t line)
{
  e.fn = fn;
  e.file = file;
  e.line = line;
  if (tryenv_idx > 0) {
    tryenvs[tryenv_idx - 1].e = e;
    longjmp(tryenvs[tryenv_idx - 1].jmp, 1);
  } else {
    ex_dump(&e, stdout);
    exit(e.code);
  }
}

FORCEINLINE NORETURN void
tryenv_rethrow(char_t __const *fn, char_t __const *file, u32_t line)
{
  ex_t e;

  e = tryenvs[tryenv_idx].e;
  e.prev = &tryenvs[tryenv_idx].e;
  tryenv_throw(e, fn, file, line);
}
