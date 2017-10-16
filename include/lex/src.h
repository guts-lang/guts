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

/*!@file lex/src.h
 * @author uael
 */
#ifndef __LEX_SRC_H
# define __LEX_SRC_H

#include <fs/stream.h>

#include "loc.h"

enum src_kind {
  LEX_SRC_FILE = 0,
  LEX_SRC_STREAM,
  LEX_SRC_STR
};

typedef enum src_kind src_kind_t;
typedef struct src src_t;

struct src {
  src_kind_t kind;
  union {
    stream_t file;
    stream_t *stream;
    struct {
      usize_t cursor, len;
      char_t __const *buf;
    } str;
  } src;
  loc_t loc;
};

__api__ void
src_init_file(src_t *self, char_t __const *filename);

__api__ void
src_init_stream(src_t *self, stream_t *stream);

__api__ void
src_init_str(src_t *self, char_t __const *buf);

__api__ void
src_init_nstr(src_t *self, char_t __const *buf, usize_t n);

__api__ char_t
src_peek(src_t *self, usize_t n);

__api__ char_t
src_next(src_t *self);

__api__ void
src_dtor(src_t *self);

#endif /* !__LEX_SRC_H */
