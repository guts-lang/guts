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

#include "fs/stream.h"

DEQ_IMPL(FORCEINLINE, sbuf, char_t, size, i8cmp)

FORCEINLINE ret_t
stream_open(stream_t *self, char_t __const *filename, u32_t flags)
{
  init(self, stream_t);
  self->flags = flags;
  sbuf_ctor(&self->in);
  sbuf_ctor(&self->out);
  return fd_open(&self->fd, filename, flags);
}

FORCEINLINE ret_t
stream_close(stream_t *self)
{
  ret_t ret;

  sbuf_dtor(&self->in);
  if (self->out.tail)
    if ((ret = fd_write(&self->fd, self->out.buf, self->out.tail, nil)) > 0)
      return ret;
  sbuf_dtor(&self->out);
  return fd_close(&self->fd);
}

ret_t
stream_read(stream_t *self, char_t *buf, usize_t len, isize_t *out)
{
  usize_t b;
  isize_t r;
  ret_t ret;

  if (out) *out = 0;
  while (len) {
    if (sbuf_size(&self->in) == 0) {
      if (self->in.tail == 0 && (ret = sbuf_realloc(&self->in, 4096)) > 0)
        return ret;
      if ((ret = fd_read(&self->fd, self->in.buf, 4096, &r)) > 0) return ret;
      if (r <= 0) break;
      self->in.tail = (usize_t) r;
      self->in.head = 0;
    }
    len -= (b = sbuf_nshift(&self->in, len, &buf));
    buf += b;
    if (out) *out += b;
  }
  return len == 0  ? RET_SUCCESS : RET_FAILURE;
}

ret_t
stream_write(stream_t *self, char_t __const *buf, usize_t len,
  isize_t *out)
{
  usize_t b;
  ret_t ret;
  isize_t w;

  if (self->flags & FS_OPEN_WO) {
    if (out) *out = 0;
    while (len) {
      if (sbuf_size(&self->out) == 0) {
        if (self->out.tail == 0 && (ret = sbuf_realloc(&self->out, 4096)) > 0)
          return ret;
        if (self->out.tail) {
          if ((ret = fd_write(&self->fd, self->out.buf, self->out.tail, &w))
            > 0) return ret;
          if (w <= 0) break;
          self->out.tail = self->out.head = 0;
        }
      }
      b = self->out.cap - self->out.head;
      if (b > len) b = len;
      if ((ret = sbuf_append(&self->out, (char_t *) buf, b)) > 0) return ret;
      len -= b;
      buf += b;
      if (out) *out += b;
    }
  } else {
    if ((ret = fd_write(&self->fd, buf, len, &w)) > 0)
      return ret;
    len -= w;
  }
  return len == 0 ? RET_SUCCESS : RET_FAILURE;
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
