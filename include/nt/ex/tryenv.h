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

/*!@file nt/ex/tryenv.h
 * @author uael
 */
#ifndef __NT_EX_TRYENV_H
# define __NT_EX_TRYENV_H

#include "ex.h"

#define TRYENV_SIZE (U8_MAX*4)

typedef struct tryenv tryenv_t;

struct tryenv {
  jmp_buf jmp;
  ex_t e;
};

#define TRY if (setjmp(tryenv_push()->jmp) == 0)
#define CATCH(e) else if (((e) = tryenv_pop()) != nil)
#if defined DEBUG
# define THROW(E) tryenv_throw(E, __pretty_func__, __file__, __line__)
# define RETHROW() tryenv_rethrow(__pretty_func__, __file__, __line__)
#else
# define THROW(E) tryenv_throw(E, __pretty_func__, nil, 0)
# define RETHROW() tryenv_rethrow(__pretty_func__, nil, 0)
#endif

__api__ tryenv_t *
tryenv_push(void);

__api__ ex_t *
tryenv_pop(void);

__api__ NORETURN void
tryenv_throw(ex_t e, char_t __const *fn, char_t __const *file, u32_t line);

__api__ NORETURN void
tryenv_rethrow(char_t __const *fn, char_t __const *file, u32_t line);

#endif /* !__NT_EX_TRYENV_H */
