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

/*!@file fs/ofstream.h
 * @author uael
 */
#ifndef __FS_OFSTREAM_H
# define __FS_OFSTREAM_H

#include "fd.h"

typedef struct ofstream ofstream_t;

struct ofstream {
  char_t __const *filename;
  fd_t fd;
  char_t *buf;
  usize_t beg, cur, end, len;
  ex_t *ex;
};

__api__ bool_t
ofstream_open(ofstream_t *self, char_t __const *filename);

__api__ void
ofstream_close(ofstream_t *self);

__api__ usize_t
ofstream_write(ofstream_t *self, char_t __const *str, usize_t len);

__api__ usize_t
ofstream_writef(ofstream_t *self, char_t __const *fmt, ...);

__api__ usize_t
ofstream_vwritef(ofstream_t *self, char_t __const *fmt, va_list ap);

__api__ usize_t
ofstream_puts(ofstream_t *self, char_t __const *str);

__api__ usize_t
ofstream_putc(ofstream_t *self, char_t c);

__api__ void
ofstream_flush(ofstream_t *self);

__api__ bool_t
ofstream_rewind(ofstream_t *self, usize_t n);

__api__ bool_t
ofstream_forward(ofstream_t *self, usize_t n);

__api__ void
ofstream_resume(ofstream_t *self);

__api__ bool_t
ofstream_seek(ofstream_t *self, usize_t off);

__api__ usize_t
ofstream_tell(ofstream_t __const *self);

#endif /* !__FS_OFSTREAM_H */
