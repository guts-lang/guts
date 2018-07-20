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

/*!@file guts/hir/parser.h
 * @author uael
 *
 * @addtogroup guts.hir @{
 */
#ifndef __GUTS_HIR_PARSER_H
# define __GUTS_HIR_PARSER_H

#include <ds/stack.h>
#include <il/codemap.h>

#include "lexer.h"
#include "scope.h"

struct hir_entity;

typedef struct {
	codemap_t *codemap;
	hir_scope_t *root, *scope;
	struct hir_entity *entity;
	stackof(hir_lexer_t)lexers;
	hir_lexer_t *lexer;
	hir_tok_t *eof;
} hir_parser_t;

typedef enum {
	PARSE_OK = 0,
	PARSE_NONE,
	PARSE_ERROR,
} hir_parse_t;

__api void hir_parser_init(hir_parser_t *self, codemap_t *codemap,
						   hir_scope_t *root);
__api void hir_parser_dtor(hir_parser_t *self);
__api int hir_parser_include(hir_parser_t *self, char __const *str, bool virt);
__api hir_tok_t *hir_parser_peek(hir_parser_t *self);
__api hir_tok_t *hir_parser_peekn(hir_parser_t *self, u8_t n);
__api hir_tok_t *hir_parser_next(hir_parser_t *self);
__api hir_tok_t *hir_parser_consume(hir_parser_t *self, u8_t kind);
__api hir_tok_t *hir_parser_any(hir_parser_t *self, u8_t __const *kinds);
__api void hir_parser_match(spanned_t *self, hir_parser_t *parser,
							hir_tok_t *tok, u16_t kind);
__api void hir_parser_scope(hir_parser_t *self, hir_scope_t *scope);
__api void hir_parser_unscope(hir_parser_t *self);

typedef bool (hir_parse_rule_t)(void *self, hir_parser_t *parser);

__api bool hir_parser_required(hir_parse_rule_t *rule, void *self,
							   hir_parser_t *parser, char __const *name);

#endif /* !__GUTS_HIR_PARSER_H */
/*!@} */
