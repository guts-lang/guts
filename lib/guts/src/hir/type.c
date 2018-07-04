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
static hir_parse_t __ty_int(hir_ty_t *self, hir_parser_t *parser,
							hir_tok_t *tok, u8_t bytes, bool unsign)
{
	hir_parser_next(parser);
	self->span = tok->span;
	self->kind = HIR_TY_INT;
	self->ty_int.bytes = bytes;
	self->ty_int.unsign = unsign;
	return PARSE_OK;
}

FORCEINLINE
static hir_parse_t __ty_float(hir_ty_t *self, hir_parser_t *parser,
							  hir_tok_t *tok, u8_t bytes)
{
	hir_parser_next(parser);
	self->span = tok->span;
	self->kind = HIR_TY_FLOAT;
	self->ty_float.bytes = bytes;
	return PARSE_OK;
}

FORCEINLINE
static hir_tok_t *__ty_types(vecof(hir_ty_t *) *types,
							  hir_parser_t *parser, hir_tok_t *tok,
							  hir_tok_kind_t closing)
{
	hir_ty_t ty;

	if (tok->kind != hir_tok_matching(closing))
		return tok;
	hir_parser_next(parser);

	do {
		if (hir_ty_consume(&ty, parser) == PARSE_ERROR)
			return NULL;
		if (!(tok = hir_parser_any(parser,
			(char[]){ closing, HIR_TOK_COMMA, HIR_TOK_EOF })))
			return NULL;
		vecpush(*types, memdup(&ty, sizeof(hir_ty_t)));
	} while (tok->kind != closing);
	return tok;
}

FORCEINLINE
static hir_parse_t __ty_tuple(hir_ty_t *self, hir_parser_t *parser,
							  hir_tok_t *tok)
{
	self->span = tok->span;
	self->kind = HIR_TY_TUPLE;
	if (!(tok = __ty_types(&self->ty_tuple.elems, parser, tok, HIR_TOK_GT)))
		return PARSE_ERROR;
	self->span.length = span_diff(tok->span, self->span);
	return PARSE_OK;
}

FORCEINLINE
static hir_parse_t __ty_lambda(hir_ty_t *self, hir_parser_t *parser,
							   hir_tok_t *tok)
{
	self->span = tok->span;
	self->kind = HIR_TY_LAMBDA;
	if (!(tok = hir_parser_peekn(parser, 1)))
		return PARSE_ERROR;
	if (tok->kind == HIR_TOK_RPAR) {
		hir_parser_next(parser);
		hir_parser_next(parser);
	} else if (!__ty_types(&self->ty_lambda.inputs, parser, tok, HIR_TOK_RPAR))
		return PARSE_ERROR;
	if (!(tok = hir_parser_peek(parser)))
		return PARSE_ERROR;
	if (tok->kind == HIR_TOK_COLON) {
		hir_ty_t ty;

		hir_parser_next(parser);
		if (hir_ty_consume(&ty, parser) == PARSE_ERROR)
			return PARSE_ERROR;
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;
		self->ty_lambda.output = memdup(&ty, sizeof(hir_ty_t));
	}
	self->span.length = span_diff(tok->span, self->span);
	return PARSE_OK;
}

FORCEINLINE
hir_parse_t hir_ty_parse(hir_ty_t *self, hir_parser_t *parser)
{
	hir_tok_t *tok;

	bzero(self, sizeof(hir_ty_t));
	if (!(tok = hir_parser_peek(parser)))
		return PARSE_ERROR;
	switch (tok->kind) {
		case HIR_TOK_CHAR: {
			hir_parser_next(parser);
			self->span = tok->span;
			self->kind = HIR_TY_CHAR;
			return PARSE_OK;
		}
		case HIR_TOK_BOOL: {
			hir_parser_next(parser);
			self->span = tok->span;
			self->kind = HIR_TY_BOOL;
			return PARSE_OK;
		}
		case HIR_TOK_I8: return __ty_int(self, parser, tok, 8, false);
		case HIR_TOK_I16: return __ty_int(self, parser, tok, 16, false);
		case HIR_TOK_I32: return __ty_int(self, parser, tok, 32, false);
		case HIR_TOK_I64: return __ty_int(self, parser, tok, 64, false);
		case HIR_TOK_U8: return __ty_int(self, parser, tok, 8, true);
		case HIR_TOK_U16: return __ty_int(self, parser, tok, 16, true);
		case HIR_TOK_U32: return __ty_int(self, parser, tok, 32, true);
		case HIR_TOK_U64: return __ty_int(self, parser, tok, 64, true);
		case HIR_TOK_F32: return __ty_float(self, parser, tok, 32);
		case HIR_TOK_F64: return __ty_float(self, parser, tok, 64);
		case HIR_TOK_IDENT: {
			hir_parser_next(parser);
			self->span = tok->span;
			self->kind = HIR_TY_SYM;
			self->ty_sym.ident = tok->ident;
			if (!(tok = __ty_types(&self->ty_sym.template, parser, tok,
				HIR_TOK_GT)))
				return PARSE_ERROR;
			self->span.length = span_diff(tok->span, self->span);
			return PARSE_OK;
		}
		case HIR_TOK_LT:
			return __ty_tuple(self, parser, tok);
		case HIR_TOK_LPAR:
			return __ty_lambda(self, parser, tok);
		case HIR_TOK_NIL: {
			hir_ty_t ty;

			hir_parser_next(parser);
			self->span = tok->span;
			self->kind = HIR_TY_NULLABLE;
			if (!(tok = hir_parser_peek(parser)))
				return PARSE_ERROR;
			if (tok->kind == HIR_TOK_CONST) {
				hir_parser_next(parser);
				self->ty_nullable.constant = true;
			}
			if (hir_ty_consume(&ty, parser) == PARSE_ERROR)
				return PARSE_ERROR;
			if (!(tok = hir_parser_peek(parser)))
				return PARSE_ERROR;
			self->ty_nullable.elem = memdup(&ty, sizeof(hir_ty_t));
			self->span.length = span_diff(tok->span, self->span);
			return PARSE_OK;
		}
		case HIR_TOK_MUL: {
			hir_ty_t ty;

			hir_parser_next(parser);
			self->span = tok->span;
			self->kind = HIR_TY_PTR;
			if (!(tok = hir_parser_peek(parser)))
				return PARSE_ERROR;
			if (tok->kind == HIR_TOK_CONST) {
				hir_parser_next(parser);
				self->ty_ptr.constant = true;
			}
			if (hir_ty_consume(&ty, parser) == PARSE_ERROR)
				return PARSE_ERROR;
			if (!(tok = hir_parser_peek(parser)))
				return PARSE_ERROR;
			self->ty_ptr.elem = memdup(&ty, sizeof(hir_ty_t));
			self->span.length = span_diff(tok->span, self->span);
			return PARSE_OK;
		}
		case HIR_TOK_LBRA: {
			hir_ty_t ty;

			hir_parser_next(parser);
			self->span = tok->span;
			if (hir_ty_consume(&ty, parser) == PARSE_ERROR)
				return PARSE_ERROR;
			if (!(tok = hir_parser_any(parser,
				(char[]){ HIR_TOK_RBRA, HIR_TOK_SEMICOLON, HIR_TOK_EOF })))
				return PARSE_ERROR;
			if (tok->kind == HIR_TOK_SEMICOLON)	{
				hir_expr_t expr;

				if (hir_expr_consume(&expr, parser) == PARSE_ERROR)
					return PARSE_ERROR;
				if (!(tok = hir_parser_consume(parser, HIR_TOK_RBRA)))
					return PARSE_ERROR;
				self->kind = HIR_TY_ARRAY;
				self->ty_array.elem = memdup(&ty, sizeof(hir_ty_t));
				self->ty_array.len = memdup(&expr, sizeof(hir_expr_t));
			} else {
				self->kind = HIR_TY_SLICE;
				self->ty_slice.elem = memdup(&ty, sizeof(hir_ty_t));
			}
			self->span.length = span_diff(tok->span, self->span);
			return PARSE_OK;
		}
		default:
			return PARSE_NONE;
	}
}

FORCEINLINE
hir_parse_t hir_ty_consume(hir_ty_t *self, hir_parser_t *parser)
{
	return hir_parse_required((hir_parse_rule_t *)hir_ty_parse, self, parser,
		"type");
}

void hir_ty_destroy(hir_ty_t *self)
{
	u32_t i;

	switch (self->kind) {
		case HIR_TY_SYM:
			for (i = 0; i < veclen(self->ty_sym.template); ++i) {
				hir_ty_destroy(self->ty_sym.template[i]);
				free(self->ty_sym.template[i]);
			}
			vecdtor(self->ty_sym.template);
			break;
		case HIR_TY_TUPLE:
			for (i = 0; i < veclen(self->ty_tuple.elems); ++i) {
				hir_ty_destroy(self->ty_tuple.elems[i]);
				free(self->ty_tuple.elems[i]);
			}
			vecdtor(self->ty_tuple.elems);
			break;
		case HIR_TY_LAMBDA:
			for (i = 0; i < veclen(self->ty_lambda.inputs); ++i) {
				hir_ty_destroy(self->ty_lambda.inputs[i]);
				free(self->ty_lambda.inputs[i]);
			}
			vecdtor(self->ty_lambda.inputs);
			if (self->ty_lambda.output) {
				hir_ty_destroy(self->ty_lambda.output);
				free(self->ty_lambda.output);
			}
			break;
		case HIR_TY_NULLABLE:
			hir_ty_destroy(self->ty_nullable.elem);
			free(self->ty_nullable.elem);
			break;
		case HIR_TY_PTR:
			hir_ty_destroy(self->ty_ptr.elem);
			free(self->ty_ptr.elem);
			break;
		case HIR_TY_SLICE:
			hir_ty_destroy(self->ty_slice.elem);
			free(self->ty_slice.elem);
			break;
		case HIR_TY_ARRAY:
			hir_ty_destroy(self->ty_array.elem);
			free(self->ty_array.elem);
			if (self->ty_array.len) {
				/* TODO: dtor len */
				free(self->ty_array.len);
			}
			break;
		default:
			break;
	}
}
