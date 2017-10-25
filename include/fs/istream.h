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

/*!@file fs/istream.h
 * @author uael
 */
#ifndef __FS_ISTREAM_H
# define __FS_ISTREAM_H

#include "fd.h"

typedef struct istream istream_t;

struct istream {
  char_t __const *filename;
  fd_t fd;
  bool_t opened;
  char_t *buf;
  usize_t beg, cur, end, cap, len;
  ex_t *ex;
};

extern istream_t *cin;

#define cin_read(BUF, LEN) istream_read(cin, BUF, LEN)
#define cin_readf(BUF, ...) istream_readf(cin, BUF, __VA_ARGS__)
#define cin_puts(BUF) istream_puts(cin, BUF)
#define cin_putc(C) istream_putc(cin, C)
#define cin_flush() istream_flush(cin)

__api__ bool_t
istream_open(istream_t *self, char_t __const *filename);

__api__ bool_t
istream_close(istream_t *self);

__api__ usize_t
istream_read(istream_t *self, char_t *buf, usize_t len);

__api__ usize_t
istream_readf(istream_t *self, char_t *fmt, ...);

__api__ usize_t
istream_vreadf(istream_t *self, char_t *fmt, va_list ap);

__api__ char_t
istream_getc(istream_t *self);

__api__ char_t
istream_peek(istream_t *self, usize_t n);

__api__ void
istream_flush(istream_t *self);

__api__ bool_t
istream_rewind(istream_t *self, usize_t n);

__api__ bool_t
istream_forward(istream_t *self, usize_t n);

__api__ void
istream_resume(istream_t *self);

__api__ bool_t
istream_seek(istream_t *self, usize_t off);

__api__ usize_t
istream_tell(istream_t __const *self);

#endif /* !__FS_ISTREAM_H */
