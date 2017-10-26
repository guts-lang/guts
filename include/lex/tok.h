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

/*!@file lex/tok.h
 * @author uael
 */
#ifndef __LEX_TOK_H
# define __LEX_TOK_H

#include "loc.h"
#include "src.h"
#include "val.h"

enum tok_kind {
  TOK_SYNTAX = 0,
  TOK_PONCT,
  TOK_KEYWORD,
  TOK_VALUE
};

typedef enum tok_kind tok_kind_t;
typedef struct tok tok_t;

/*!@brief The 32 bytes token structure
 * When kind is TOK_VALUE, instead of the type, an index to the value on the
 * lexer values cache is provided in the 'id' field. The loc struct provide also
 * an index to the related stream by the 'src' field.
 */
struct tok {
  tok_kind_t kind: 4;
  u16_t lws;
  u32_t id;
  loc_t loc;
};

#endif /* !__LEX_TOK_H */
