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

/*!@file fs/omstream.h
 * @author uael
 */
#ifndef __FS_OMSTREAM_H
# define __FS_OMSTREAM_H

#include "fd.h"

typedef struct omstream omstream_t;

struct omstream {
  char_t *buf;
  usize_t cap, cur, len;
};

__api__ bool_t
omstream_open(omstream_t *self);

__api__ void
omstream_close(omstream_t *self);

__api__ usize_t
omstream_write(omstream_t *self, char_t __const *str, usize_t len);

__api__ usize_t
omstream_writef(omstream_t *self, char_t __const *fmt, ...);

__api__ usize_t
omstream_vwritef(omstream_t *self, char_t __const *fmt, va_list ap);

__api__ usize_t
omstream_puts(omstream_t *self, char_t __const *str);

__api__ usize_t
omstream_putc(omstream_t *self, char_t c);

__api__ void
omstream_flush(omstream_t *self);

__api__ bool_t
omstream_rewind(omstream_t *self, usize_t n);

__api__ bool_t
omstream_forward(omstream_t *self, usize_t n);

__api__ void
omstream_resume(omstream_t *self);

__api__ bool_t
omstream_seek(omstream_t *self, usize_t off);

__api__ usize_t
omstream_tell(omstream_t __const *self);

#endif /* !__FS_OMSTREAM_H */
