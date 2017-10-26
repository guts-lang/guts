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

/*!@file fs/imstream.h
 * @author uael
 */
#ifndef __FS_IMSTREAM_H
# define __FS_IMSTREAM_H

#include "fd.h"

typedef struct imstream imstream_t;

struct imstream {
  char_t __const* buf;
  usize_t cur, len;
  ex_t *ex;
};

__api__ bool_t
imstream_open(imstream_t *self, char_t __const *str);

__api__ bool_t
imstream_nopen(imstream_t *self, char_t __const *str, usize_t n);

__api__ bool_t
imstream_close(imstream_t *self);

__api__ usize_t
imstream_read(imstream_t *self, char_t *buf, usize_t len);

__api__ usize_t
imstream_readf(imstream_t *self, char_t *fmt, ...);

__api__ usize_t
imstream_vreadf(imstream_t *self, char_t *fmt, va_list ap);

__api__ char_t
imstream_getc(imstream_t *self);

__api__ usize_t
imstream_get(imstream_t *self, char_t *buf, usize_t len);

__api__ char_t
imstream_peek(imstream_t *self, usize_t n);

__api__ void
imstream_flush(imstream_t *self);

__api__ bool_t
imstream_rewind(imstream_t *self, usize_t n);

__api__ bool_t
imstream_forward(imstream_t *self, usize_t n);

__api__ void
imstream_resume(imstream_t *self);

__api__ bool_t
imstream_seek(imstream_t *self, usize_t off);

__api__ usize_t
imstream_tell(imstream_t __const *self);

#endif /* !__FS_IMSTREAM_H */
