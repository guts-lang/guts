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

/*!@file fs/stream.h
 * @author uael
 */
#ifndef __FS_STREAM_H
# define __FS_STREAM_H

#include "fd.h"

typedef struct stream stream_t;
typedef deqof(char_t, size) sinbuf_t;
typedef vecof(char_t, size) soutbuf_t;

struct stream {
  fd_t fd;
  u32_t flags;
  sinbuf_t in;
  soutbuf_t out;
  errs_t errs;
};

__api__ bool_t
stream_open(stream_t *self, char_t __const *filename, u32_t flags);

__api__ void
stream_in(stream_t *self);

__api__ void
stream_out(stream_t *self);

__api__ void
stream_close(stream_t *self);

__api__ usize_t
stream_read(stream_t *self, char_t *buf, usize_t len);

__api__ char_t
stream_getc(stream_t *self);

__api__ char_t
stream_peek(stream_t *self, usize_t n);

__api__ usize_t
stream_write(stream_t *self, char_t __const *buf, usize_t len);

__api__ void
stream_flush(stream_t *self);

__api__ bool_t
stream_seek(stream_t __const *self, isize_t off, fs_seek_mod_t whence);

__api__ usize_t
stream_offset(stream_t __const *self);

#endif /* !__FS_STREAM_H */
