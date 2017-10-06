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

/*!@file ulex/tok.h
 * @author uael
 */
#ifndef __ULEX_TOK_H
# define __ULEX_TOK_H

#include <uds/deq.h>

#include "loc.h"
#include "src.h"
#include "val.h"

enum lex_tok_type {
  LEX_TOK_NONE = 0,
  LEX_TOK_PONCTUATION,
  LEX_TOK_OPERATOR,
  LEX_TOK_KEYWORD,
  LEX_TOK_VALUE
};

typedef enum lex_tok_type lex_tok_type_t;
typedef struct lex_tok lex_tok_t;

/*!@brief The 32 bytes token structure
 * When type is LEX_TOK_VALUE instead of kind an index to the value on the lexer
 * values cache is provided. The loc struct provide also an index to the related
 * stream.
 */
struct lex_tok {
  u8_t type;
  u16_t lws;
  union {
    u32_t kind;
    u32_t val;
  } cnt;
  lex_loc_t loc;
};

#endif /* !__ULEX_TOK_H */
