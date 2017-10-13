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

#include <fs.h>
#include "fs/stream.h"

#if defined PAGE_SIZE && PAGE_SIZE <= 4096
# define FS_PAGE_SIZE PAGE_SIZE
#elif defined PAGESIZE && PAGESIZE <= 4096
# define FS_PAGE_SIZE PAGESIZE
#else
# define FS_PAGE_SIZE 4096
#endif

DEQ_IMPL(FORCEINLINE, sinbuf, char_t, size, i8cmp)
VEC_IMPL(FORCEINLINE, soutbuf, char_t, size, i8cmp)

FORCEINLINE ret_t
stream_open(stream_t *self, char_t __const *filename, u32_t flags)
{
  init(self, stream_t);
  self->flags = flags;
  sinbuf_ctor(&self->in);
  soutbuf_ctor(&self->out);
  return fd_open(&self->fd, filename, flags);
}

FORCEINLINE void
stream_in(stream_t *self)
{
  init(self, stream_t);
  self->fd = 0;
  self->flags |= FS_OPEN_RO;
}

FORCEINLINE void
stream_out(stream_t *self)
{
  init(self, stream_t);
  self->fd = 1;
  self->flags |= FS_OPEN_WO;
}

FORCEINLINE ret_t
stream_close(stream_t *self)
{
  ret_t ret, ret2;

  ret = stream_flush(self);
  if (self->fd > 1) {
    ret2 = fd_close(&self->fd);
    if (ret2 > ret) ret = ret2;
  }
  sinbuf_dtor(&self->in);
  soutbuf_dtor(&self->out);
  return ret;
}

static ret_t
stream_bufferize(stream_t *self, usize_t len, usize_t *out)
{
  usize_t r;
  ret_t ret;

  ret = RET_SUCCESS;
  if (self->in.tail % 4096 != 0) {
    if (out) {
      *out = sinbuf_size(&self->in);
      if (*out > len) *out = len;
    }
  } else {
    if (out) *out = len;
    while (sinbuf_size(&self->in) < len) {
      char_t buf[FS_PAGE_SIZE];

      if ((ret = fd_read(&self->fd, buf, FS_PAGE_SIZE, &r)) > 0) break;
      if ((ret = sinbuf_append(&self->in, buf, (usize_t __const) r)) > 0) break;
      if (*out && r < *out) *out = r;
    }
  }
  return ret;
}

ret_t
stream_read(stream_t *self, char_t *buf, usize_t len, usize_t *out)
{
  ret_t ret;
  usize_t r;

  if ((ret = stream_bufferize(self, len, &r)) > 0) return ret;
  if ((r = sinbuf_nshift(&self->in, r, &buf)) == 0) return RET_FAILURE;
  if (out) *out = r;
  return RET_SUCCESS;
}

FORCEINLINE ret_t
stream_getc(stream_t *self, char_t *out) {
  return stream_read(self, out, 1, nil);
}

FORCEINLINE ret_t
stream_peek(stream_t *self, usize_t n, char_t *out) {
  ret_t ret;
  usize_t r;

  if ((ret = stream_bufferize(self, n, &r)) > 0) return ret;
  if (r > n) return RET_FAILURE;
  *out = sinbuf_at(&self->in, n);
  return ret;
}

ret_t
stream_write(stream_t *self, char_t __const *buf, usize_t len, usize_t *out)
{
  usize_t b;
  ret_t ret;
  isize_t w;

  if (self->flags & FS_OPEN_WO) {
    if (out) *out = 0;
    while (len) {
      b = FS_PAGE_SIZE - self->out.len;
      if (b > len) b = len;
      if ((ret = soutbuf_append(&self->out, (char_t *) buf, b)) > 0) return ret;
      len -= b;
      buf += b;
      if (out) *out += b;
      if (self->out.len == FS_PAGE_SIZE) {
        if ((ret = fd_write(&self->fd, self->out.buf, self->out.len, &w)) > 0)
          return ret;
        if (w <= 0) break;
        self->out.len = 0;
      }
    }
  } else {
    if ((ret = fd_write(&self->fd, buf, len, &w)) > 0)
      return ret;
    len -= w;
  }
  return len == 0 ? RET_SUCCESS : RET_FAILURE;
}

FORCEINLINE ret_t
stream_flush(stream_t *self)
{
  ret_t ret;

  if (self->out.len) {
    if ((ret = fd_write(&self->fd, self->out.buf, self->out.len, nil)) > 0)
      return ret;
    self->out.len = 0;
  }
  return RET_SUCCESS;
}

ret_t
stream_seek(stream_t __const *self, isize_t off, fs_seek_mod_t whence,
  isize_t *out)
{
  (void) self;
  (void) off;
  (void) whence;
  (void) out;
  return RET_NOT_IMPL;
}

isize_t
stream_offset(stream_t __const *self)
{
  (void) self;
  return RET_NOT_IMPL;
}
