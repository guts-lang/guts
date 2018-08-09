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
#include "guts/hir/entity.h"

static FORCEINLINE
bool __int(struct hir_ty *self, hir_parser_t *parser, hir_tok_t *tok,
		   u8_t bytes, bool unsign)
{
	hir_parser_match(&self->spanned, parser, tok, HIR_TY_INT);
	self->ty_int.bytes = bytes;
	self->ty_int.unsign = unsign;
	return true;
}

static FORCEINLINE
bool __float(struct hir_ty *self, hir_parser_t *parser, hir_tok_t *tok,
			 u8_t bytes)
{
	hir_parser_match(&self->spanned, parser, tok, HIR_TY_FLOAT);
	self->ty_float.bytes = bytes;
	return true;
}

static FORCEINLINE
bool __lambda(struct hir_ty *self, hir_parser_t *parser, hir_tok_t *tok)
{
	struct hir_ty ty;

	hir_parser_match(&self->spanned, parser, tok, HIR_TY_LAMBDA);

	while ((tok = hir_parser_peek(parser))->kind && tok->kind != ')') {
		hir_ty_consume(&ty, parser);
		vecpush(self->ty_lambda.inputs, ty);
		if (hir_parser_any(parser, (u8_t[]) { ')', ',', 0 })->kind == ')')
			break;
	}

	if ((tok = hir_parser_peek(parser))->kind == ':') {
		hir_parser_next(parser);
		hir_ty_consume(&ty, parser);
		self->ty_lambda.output = memdup(&ty, sizeof(struct hir_ty));
		tok = hir_parser_peek(parser);
	}

	spanned_diff(&self->spanned, &tok->spanned);

	return true;
}

static FORCEINLINE
bool __array_or_tuple(struct hir_ty *self, hir_parser_t *parser, hir_tok_t *tok)
{
	struct hir_ty ty;

	hir_parser_match(&self->spanned, parser, tok, HIR_TY_ARRAY);
	hir_ty_consume(&ty, parser);
	tok = hir_parser_any(parser, (u8_t[]) { ';', ']', ',', 0 });

	if (tok->kind == ']')
		self->ty_array.elem = memdup(&ty, sizeof(struct hir_ty));
	else if (tok->kind == ';') {
		hir_expr_t expr;

		self->ty_array.elem = memdup(&ty, sizeof(struct hir_ty));
		hir_expr_consume(&expr, parser);
		self->ty_array.len = memdup(&expr, sizeof(hir_expr_t));
		tok = hir_parser_consume(parser, ']');
	} else if (tok->kind == ',') {
		self->kind = HIR_TY_TUPLE;
		vecpush(self->ty_tuple.elems, ty);

		while ((tok = hir_parser_peek(parser))->kind && tok->kind != ']') {
			hir_ty_consume(&ty, parser);
			vecpush(self->ty_tuple.elems, ty);
			if (hir_parser_any(parser, (u8_t[]) { ']', ',', 0 })->kind == ']')
				break;
		}

		tok = hir_parser_peek(parser);
	}

	spanned_diff(&self->spanned, &tok->spanned);

	return true;
}

bool hir_ty_parse(struct hir_ty *self, hir_parser_t *parser)
{
	hir_tok_t *tok;

	bzero(self, sizeof(struct hir_ty));
	switch (((tok = hir_parser_peek(parser)))->kind) {
		case HIR_TOK_I8: return __int(self, parser, tok, 8, false);
		case HIR_TOK_I16: return __int(self, parser, tok, 16, false);
		case HIR_TOK_I32: return __int(self, parser, tok, 32, false);
		case HIR_TOK_I64: return __int(self, parser, tok, 64, false);
		case HIR_TOK_U8: return __int(self, parser, tok, 8, true);
		case HIR_TOK_U16: return __int(self, parser, tok, 16, true);
		case HIR_TOK_U32: return __int(self, parser, tok, 32, true);
		case HIR_TOK_U64: return __int(self, parser, tok, 64, true);
		case HIR_TOK_F32: return __float(self, parser, tok, 32);
		case HIR_TOK_F64: return __float(self, parser, tok, 64);
		case '(': return __lambda(self, parser, tok);
		case '[': return __array_or_tuple(self, parser, tok);
		case '?': case '*': {
			struct hir_ty ty;

			if (tok->kind == '?')
				self->ty_ptr.nullable = true;
			hir_parser_match(&self->spanned, parser, tok, HIR_TY_PTR);
			if (hir_parser_peek(parser)->kind == HIR_TOK_CONST) {
				hir_parser_next(parser);
				self->ty_ptr.constant = true;
			}
			hir_ty_consume(&ty, parser);
			self->ty_ptr.elem = memdup(&ty, sizeof(struct hir_ty));
			tok = hir_parser_peek(parser);
			spanned_diff(&self->spanned, &tok->spanned);
			return true;
		}
		case HIR_TOK_CHAR: {
			hir_parser_match(&self->spanned, parser, tok, HIR_TY_CHAR);
			return true;
		}
		case HIR_TOK_BOOL: {
			hir_parser_match(&self->spanned, parser, tok, HIR_TY_BOOL);
			return true;
		}
		case HIR_TOK_IDENT: {
			hir_path_consume(&self->ty_sym.path, parser);
			self->span = self->ty_sym.path.span;
			self->kind = HIR_TY_SYM;
			return true;
		}
		default:
			return false;
	}
}

FORCEINLINE
bool hir_ty_consume(struct hir_ty *self, hir_parser_t *parser)
{
	return hir_parser_required((hir_parse_rule_t *) hir_ty_parse,
		self, parser, "type");
}

void hir_ty_destroy(struct hir_ty *self)
{
	/* self is nullable here. */
	if (!self)
		return;

	switch (self->kind) {
		case HIR_TY_SYM:
			hir_path_destroy(&self->ty_sym.path);
			break;
		case HIR_TY_TUPLE:
			vecapply(self->ty_tuple.elems, hir_ty_destroy);
			vecdtor(self->ty_tuple.elems);
			break;
		case HIR_TY_LAMBDA:
			vecapply(self->ty_lambda.inputs, hir_ty_destroy);
			vecdtor(self->ty_lambda.inputs);
			if (self->ty_lambda.output) {
				hir_ty_destroy(self->ty_lambda.output);
				free(self->ty_lambda.output);
			}
			break;
		case HIR_TY_PTR:
			hir_ty_destroy(self->ty_ptr.elem);
			free(self->ty_ptr.elem);
			break;
		case HIR_TY_ARRAY:
			hir_ty_destroy(self->ty_array.elem);
			free(self->ty_array.elem);
			if (self->ty_array.len) {
				hir_expr_destroy(self->ty_array.len);
				free(self->ty_array.len);
			}
			break;
		case HIR_TY_STRUCT:
			vecapply(self->ty_struct.fields, hir_ent_destroy);
			vecdtor(self->ty_struct.fields);
			break;
		case HIR_TY_ENUM:
			vecapply(self->ty_enum.fields, hir_ent_destroy);
			vecdtor(self->ty_enum.fields);
			break;
		default:
			break;
	}
}
