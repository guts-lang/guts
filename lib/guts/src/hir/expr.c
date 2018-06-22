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

#include <il/span.h>
#include <il/loc.h>
#include "guts/hir/expr.h"

#define TRANSFER(e) if (!(e)) return NULL
#define NEW(T, ...) memcpy(malloc(sizeof(T)), &((T)__VA_ARGS__), sizeof(T))

static hir_expr_t *__assignment(hir_lexer_t *lexer);

static void __commas(hir_lexer_t *lexer, vecof(hir_expr_t *) *commas)
{
	hir_tok_t *tok;

	vecpush(*commas, __assignment(lexer));
	while (true) {
		tok = hir_lexer_peek(lexer);
		if (tok->kind != HIR_TOK_COMMA)
			break;
		hir_lexer_next(lexer);
		vecpush(*commas, __assignment(lexer));
	}
}

static hir_expr_t *__primary(hir_lexer_t *lexer)
{
	hir_tok_t *tok;
	hir_expr_t *expr;
	vecof(hir_expr_t *) commas;
	span_t start;

	TRANSFER(tok = hir_lexer_peek(lexer));
	start = tok->span;
	switch (tok->kind) {
		case HIR_TOK_IDENT:
			hir_lexer_next(lexer);
			return NEW(hir_expr_t, {
				.kind = HIR_EXPR_IDENT,
				.span = start,
				.ident = tok->ident
			});
		case HIR_TOK_LPAR:
			hir_lexer_next(lexer);
			expr = hir_expr_parse(lexer);
			if (hir_lexer_peek(lexer)->kind != HIR_TOK_COMMA) {
				tok = hir_lexer_consume(lexer, HIR_TOK_RPAR);
				start.length = (u16_t) (tok->span.start.off - start.start.off);
				return NEW(hir_expr_t, {
					.kind = HIR_EXPR_PAREN,
					.span = start,
					.paren = { expr }
				});
			}
			vecpush(commas, expr);
			__commas(lexer, &commas);
			tok = hir_lexer_consume(lexer, HIR_TOK_RPAR);
			start.length = (u16_t) (tok->span.start.off - start.start.off);
			return NEW(hir_expr_t, {
				.kind = HIR_EXPR_TUPLE,
				.span = start,
				.tuple = { commas }
			});
		case HIR_TOK_LBRA:
			hir_lexer_next(lexer);
			commas = NULL;
			__commas(lexer, &commas);
			tok = hir_lexer_consume(lexer, HIR_TOK_RBRA);
			start.length = (u16_t) (tok->span.start.off - start.start.off);
			return NEW(hir_expr_t, {
				.kind = HIR_EXPR_ARRAY,
				.span = start,
				.array = { commas }
			});
		default:
			return NULL;
	}
}

static hir_expr_t *__assignment(hir_lexer_t *lexer)
{
	return __primary(lexer);
}

hir_expr_t *hir_expr_parse(hir_lexer_t *lexer)
{
	return __primary(lexer);
}
