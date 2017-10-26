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

/*!@file fs/ifstream.h
 * @author uael
 */
#ifndef __FS_IFSTREAM_H
# define __FS_IFSTREAM_H

#include "fd.h"

typedef struct ifstream ifstream_t;

struct ifstream {
  char_t __const *filename;
  fd_t fd;
  char_t *buf;
  usize_t beg, cur, end, cap, len;
  ex_t *ex;
};

__api__ bool_t
ifstream_open(ifstream_t *self, char_t __const *filename);

__api__ bool_t
ifstream_close(ifstream_t *self);

__api__ usize_t
ifstream_read(ifstream_t *self, char_t *buf, usize_t len);

__api__ usize_t
ifstream_readf(ifstream_t *self, char_t *fmt, ...);

__api__ usize_t
ifstream_vreadf(ifstream_t *self, char_t *fmt, va_list ap);

__api__ char_t
ifstream_getc(ifstream_t *self);

__api__ usize_t
ifstream_get(ifstream_t *self, char_t *buf, usize_t len);

__api__ char_t
ifstream_peek(ifstream_t *self, usize_t n);

__api__ void
ifstream_flush(ifstream_t *self);

__api__ bool_t
ifstream_rewind(ifstream_t *self, usize_t n);

__api__ bool_t
ifstream_forward(ifstream_t *self, usize_t n);

__api__ void
ifstream_resume(ifstream_t *self);

__api__ bool_t
ifstream_seek(ifstream_t *self, usize_t off);

__api__ usize_t
ifstream_tell(ifstream_t __const *self);

#endif /* !__FS_IFSTREAM_H */
