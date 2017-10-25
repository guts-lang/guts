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

/*!@file fs/ostream.h
 * @author uael
 */
#ifndef __FS_OSTREAM_H
# define __FS_OSTREAM_H

#include "fd.h"

typedef struct ostream ostream_t;

struct ostream {
  char_t __const *filename;
  fd_t fd;
  bool_t opened;
  char_t *buf;
  usize_t beg, cur, end, len;
  ex_t *ex;
};

extern ostream_t *cout;

#define cout_write(BUF, LEN) ostream_write(cout, BUF, LEN)
#define cout_writef(BUF, ...) ostream_writef(cout, BUF, __VA_ARGS__)
#define cout_puts(BUF) ostream_puts(cout, BUF)
#define cout_putc(C) ostream_putc(cout, C)
#define cout_flush() ostream_flush(cout)

__api__ bool_t
ostream_open(ostream_t *self, char_t __const *filename);

__api__ void
ostream_close(ostream_t *self);

__api__ usize_t
ostream_write(ostream_t *self, char_t __const *buf, usize_t len);

__api__ usize_t
ostream_writef(ostream_t *self, char_t __const *fmt, ...);

__api__ usize_t
ostream_vwritef(ostream_t *self, char_t __const *fmt, va_list ap);

__api__ usize_t
ostream_puts(ostream_t *self, char_t __const *buf);

__api__ usize_t
ostream_putc(ostream_t *self, char_t c);

__api__ void
ostream_flush(ostream_t *self);

__api__ bool_t
ostream_rewind(ostream_t *self, usize_t n);

__api__ bool_t
ostream_forward(ostream_t *self, usize_t n);

__api__ void
ostream_resume(ostream_t *self);

__api__ bool_t
ostream_seek(ostream_t *self, usize_t off);

__api__ usize_t
ostream_tell(ostream_t __const *self);

#endif /* !__FS_OSTREAM_H */
