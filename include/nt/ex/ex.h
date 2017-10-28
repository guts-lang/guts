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

/*!@file nt/ex/ex.h
 * @author uael
 */
#ifndef __NT_EX_EX_H
# define __NT_EX_EX_H

#include "../tys.h"
#include "lvl.h"

#if defined __ELASTERROR
# define EX_USR_BEGIN __ELASTERROR
#else
# define EX_USR_BEGIN 2000
#endif

#define EX_REGISTER(E) do { \
    if (E##_code == 0) E##_code = ex_register(); \
  } while (false)

__api__ i32_t
ex_register(void);

typedef struct ex ex_t;

struct ex {
  errlvl_t lvl;
  char_t __const *fn, *file;
  u32_t line, col;
  i32_t code;
  char_t msg[U8_MAX];
  ex_t *prev;
  void (*dump)(ex_t *self, void *stream);
};

__api__ void
ex_ctor(ex_t *self, errlvl_t lvl, i32_t code, char_t __const *msg,
  va_list args);

__api__ void
ex_dump(ex_t *self, FILE *stream);

__api__ ex_t
ex_usr(errlvl_t lvl, i32_t no, char_t __const *msg, ...);

__api__ ex_t
ex(errlvl_t lvl, char_t __const *msg, ...);

__api__ ex_t
ex_errno(errlvl_t lvl, i32_t err_no, char_t __const *msg, ...);

__api__ ex_t
ex_notice(char_t __const *msg, ...);

__api__ ex_t
ex_warn(char_t __const *msg, ...);

__api__ ex_t
ex_error(char_t __const *msg, ...);

__api__ ex_t
ex_fatal(char_t __const *msg, ...);

#endif /* !__NT_EX_EX_H */
