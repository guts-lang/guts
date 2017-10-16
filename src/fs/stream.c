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

FORCEINLINE void
stream_open(stream_t *self, char_t __const *filename, u32_t flags)
{
  init(self, stream_t);
  self->flags = flags;
  sinbuf_ctor(&self->in);
  soutbuf_ctor(&self->out);
  fd_open(&self->fd, filename, flags);
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

FORCEINLINE void
stream_close(stream_t *self)
{
  stream_flush(self);
  fd_close(&self->fd);
  sinbuf_dtor(&self->in);
  soutbuf_dtor(&self->out);
}

static usize_t
stream_bufferize(stream_t *self, usize_t len)
{
  usize_t r, b;

  if (self->in.tail % 4096 != 0) {
    b = sinbuf_size(&self->in);
  } else {
    b = 0;
    while (sinbuf_size(&self->in) < len) {
      char_t buf[FS_PAGE_SIZE];

      if ((r = fd_read(&self->fd, buf, FS_PAGE_SIZE)) == 0) break;
      sinbuf_append(&self->in, buf, (usize_t __const) r);
      b += r;
    }
  }
  if (b > len) return len;
  return b;
}

usize_t
stream_read(stream_t *self, char_t *buf, usize_t len)
{
  return sinbuf_nshift(&self->in, stream_bufferize(self, len), &buf);
}

FORCEINLINE char_t
stream_getc(stream_t *self)
{
  char_t c;

  if (stream_read(self, &c, 1) == 1)
    return c;
  return '\0';
}

FORCEINLINE char_t
stream_peek(stream_t *self, usize_t n)
{
  if (n > stream_bufferize(self, n))
    return '\0';
  return sinbuf_at(&self->in, n);
}

usize_t
stream_write(stream_t *self, char_t __const *buf, usize_t len)
{
  usize_t b, tw;
  bool_t ret;

  tw = 0;
  if (self->flags & FS_OPEN_WO) {
    while (len) {
      b = FS_PAGE_SIZE - self->out.len;
      if (b > len) b = len;
      if ((ret = soutbuf_append(&self->out, (char_t *) buf, b)) > 0) return ret;
      len -= b;
      buf += b;
      tw += b;
      if (self->out.len == FS_PAGE_SIZE) {
        if (fd_write(&self->fd, self->out.buf, self->out.len) == 0)
          break;
        self->out.len = 0;
      }
    }
    return tw;
  }
  return fd_write(&self->fd, buf, len);
}

FORCEINLINE void
stream_flush(stream_t *self)
{
  if (self->out.len) {
    fd_write(&self->fd, self->out.buf, self->out.len);
    self->out.len = 0;
  }
}

bool_t
stream_seek(stream_t __const *self, isize_t off, fs_seek_mod_t whence)
{
  (void) self;
  (void) off;
  (void) whence;
  return 0;
}

usize_t
stream_offset(stream_t __const *self)
{
  (void) self;
  return 0;
}
