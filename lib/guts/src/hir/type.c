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

#include "guts/hir/type.h"
#include "guts/hir/expr.h"

FORCEINLINE
static hir_parse_t __slice(hir_ty_t *ty, hir_parser_t *parser, hir_tok_t *tok)
{
	hir_ty_t elem;

	hir_parser_next(parser);
	ty->span = tok->span;
	bzero(&elem, sizeof(hir_ty_t));

	if (hir_ty_consume(&elem, parser) == PARSE_ERROR)
		return PARSE_ERROR;

	tok = hir_parser_any(parser, (char __const[]){
		HIR_TOK_RBRA, HIR_TOK_SEMICOLON, HIR_TOK_EOF
	});

	if (!tok)
		return PARSE_ERROR;

	if (tok->kind == HIR_TOK_RBRA) {
		ty->kind = HIR_TY_SLICE;
		ty->ty_slice.elem = memdup(&elem, sizeof(hir_ty_t));
	} else {
		hir_expr_t expr;

		ty->kind = HIR_TY_ARRAY;

		if (hir_expr_consume(&expr, parser) == PARSE_ERROR)
			return PARSE_ERROR;
		if (!(tok = hir_parser_consume(parser, HIR_TOK_RBRA)))
			return PARSE_ERROR;

		ty->ty_array.elem = memdup(&elem, sizeof(hir_ty_t));
		ty->ty_array.len = memdup(&expr, sizeof(hir_expr_t));
	}

	ty->span.length = span_diff(tok->span, ty->span);

	return PARSE_OK;
}

FORCEINLINE
static hir_parse_t __ptr(hir_ty_t *ty, hir_parser_t *parser, hir_tok_t *tok)
{
	hir_ty_t elem;

	hir_parser_next(parser);
	ty->span = tok->span;
	bzero(&elem, sizeof(hir_ty_t));

	if (hir_ty_consume(&elem, parser) == PARSE_ERROR)
		return PARSE_ERROR;
	if (!(tok = hir_parser_peek(parser)))
		return PARSE_ERROR;

	ty->kind = HIR_TY_PTR;
	ty->ty_ptr.elem = memdup(&elem, sizeof(hir_ty_t));
	ty->span.length = span_diff(tok->span, ty->span);

	return PARSE_OK;
}

FORCEINLINE
static hir_parse_t __tuple(hir_ty_t *ty, hir_parser_t *parser, hir_tok_t *tok)
{
	vecof(hir_ty_t *) types;
	hir_ty_t elem;

	types = NULL;
	hir_parser_next(parser);
	ty->span = tok->span;

	while (true) {
		bzero(&elem, sizeof(hir_expr_t));

		if (hir_ty_consume(&elem, parser) == PARSE_ERROR)
			return PARSE_ERROR;

		vecpush(types, memdup(&elem, sizeof(hir_expr_t)));

		tok = hir_parser_any(parser, (char __const[]){
			HIR_TOK_RPAR, HIR_TOK_COMMA, HIR_TOK_EOF
		});

		if (!tok)
			return PARSE_ERROR;
		if (tok->kind == HIR_TOK_RPAR)
			break;
	}

	ty->kind = HIR_TY_TUPLE;
	ty->ty_tuple.elems = types;
	ty->span.length = span_diff(tok->span, ty->span);

	/* TODO: fn */

	return PARSE_OK;
}

FORCEINLINE
static hir_parse_t __integer(hir_ty_t *ty, hir_parser_t *parser, hir_tok_t *tok,
							 u8_t bytes, bool unsign)
{
	hir_parser_next(parser);
	ty->span = tok->span;
	ty->kind = HIR_TY_INT;
	ty->ty_int.bytes = bytes;
	ty->ty_int.unsign = unsign;
	return PARSE_OK;
}

static hir_parse_t __ty(hir_ty_t *ty, hir_parser_t *parser)
{
	hir_tok_t *tok;

	if (!(tok = hir_parser_peek(parser)))
		return PARSE_ERROR;

	switch (tok->kind) {
		case HIR_TOK_LBRA:
			return __slice(ty, parser, tok);
		case HIR_TOK_MUL:
			return __ptr(ty, parser, tok);
		case HIR_TOK_LPAR:
			return __tuple(ty, parser, tok);
		case HIR_TOK_I8:
			return __integer(ty, parser, tok, 8, false);
		case HIR_TOK_I16:
			return __integer(ty, parser, tok, 16, false);
		case HIR_TOK_I32:
			return __integer(ty, parser, tok, 32, false);
		case HIR_TOK_I64:
			return __integer(ty, parser, tok, 64, false);
		case HIR_TOK_U8:
			return __integer(ty, parser, tok, 8, true);
		case HIR_TOK_U16:
			return __integer(ty, parser, tok, 16, true);
		case HIR_TOK_U32:
			return __integer(ty, parser, tok, 32, true);
		case HIR_TOK_U64:
			return __integer(ty, parser, tok, 64, true);
		case HIR_TOK_CHAR:
			hir_parser_next(parser);
			ty->span = tok->span;
			ty->kind = HIR_TY_CHAR;
			return PARSE_OK;
		case HIR_TOK_BOOL:
			hir_parser_next(parser);
			ty->span = tok->span;
			ty->kind = HIR_TY_BOOL;
			return PARSE_OK;
		case HIR_TOK_IDENT:
			hir_parser_next(parser);
			ty->span = tok->span;
			ty->kind = HIR_TY_IDENT;
			ty->ident = tok->ident;
			return PARSE_OK;
		default:
			return PARSE_NONE;
	}
}

FORCEINLINE
hir_parse_t hir_ty_parse(hir_ty_t *self, hir_parser_t *parser)
{
	bzero(self, sizeof(hir_ty_t));
	return __ty(self, parser);
}

FORCEINLINE
hir_parse_t hir_ty_consume(hir_ty_t *self, hir_parser_t *parser)
{
	bzero(self, sizeof(hir_ty_t));
	return hir_parse_required((hir_parse_rule_t *)__ty, self, parser, "type");
}
