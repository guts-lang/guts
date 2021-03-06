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

/*!@file fs/fd.h
 * @author uael
 */
#ifndef __FS_FD_H
# define __FS_FD_H

#include "defs.h"
#include "fs.h"

#if defined OS_WIN && __has_feature__(io_h)
# define FS_FD_MODEL_WIN_UCRT
#elif defined OS_WIN
# define FS_FD_MODEL_WIN_NT
#elif __has_feature__(unistd_h)
# define FS_FD_MODEL_UNIX
#else
# define FS_FD_MODEL_NONE
#endif

#define FS_FD_DFT (-1)

enum fs_open_mod {
  FS_OPEN_RO = 1 << 0,
  FS_OPEN_WO = 1 << 1,
  FS_OPEN_RW = 1 << 2,
  FS_OPEN_CREAT = 1 << 3,
  FS_OPEN_APPEND = 1 << 4,
  FS_OPEN_TRUNC = 1 << 5,
  FS_OPEN_DIRECT = 1 << 6,
  FS_OPEN_ASIO = 1 << 7
};

enum fs_seek_mod {
  FS_SEEK_BEG = 0,
  FS_SEEK_CUR,
  FS_SEEK_END
};

typedef enum fs_open_mod fs_open_mod_t;
typedef enum fs_seek_mod fs_seek_mod_t;
typedef i32_t fd_t;

__api__ void
fd_open(fd_t *__restrict self, char_t __const *filename, u32_t flags);

__api__ void
fd_close(fd_t __const *__restrict self);

__api__ usize_t
fd_read(fd_t __const *__restrict self, char_t *buf, usize_t len);

__api__ usize_t
fd_write(fd_t __const *__restrict self, char_t __const *buf, usize_t len);

__api__ usize_t
fd_seek(fd_t __const *__restrict self, isize_t off, fs_seek_mod_t whence);

__api__ usize_t
fd_offset(fd_t __const *__restrict self);

#endif /* !__FS_FD_H */
