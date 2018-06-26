/*
 * MIT License
 *
 * Copyright (c) 2016-2018 Abel Lucas <www.github.com/uael>
 * Copyright (c) 2016-2018 Guts <www.github.com/guts-lang>
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

/*!@file guts/hir/lexer.h
 * @author uael
 *
 * @addtogroup guts.hir @{
 */
#ifndef __GUTS_HIR_LEXER_H
# define __GUTS_HIR_LEXER_H

#include <il/source.h>
#include <ds/deque.h>

#include "token.h"

typedef struct {
	bool eof;
	source_t *src;
	deqof(hir_tok_t) lookahead;
	vecof(diag_t) *diags;
} hir_lexer_t;

__api void hir_lexer_init(hir_lexer_t *self, source_t *src,
						  vecof(diag_t) *diags);
__api void hir_lexer_dtor(hir_lexer_t *self);
__api hir_tok_t *hir_lexer_peek(hir_lexer_t *self);
__api hir_tok_t *hir_lexer_peekn(hir_lexer_t *self, u8_t n);
__api hir_tok_t *hir_lexer_next(hir_lexer_t *self);
__api hir_tok_t *hir_lexer_consume(hir_lexer_t *self, hir_tok_kind_t kind);

#endif /* !__GUTS_HIR_LEXER_H */
/*!@} */
