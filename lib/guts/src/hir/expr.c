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

#include "guts/hir/expr.h"
#include "guts/hir/entity.h"

typedef bool (rule_t)(struct hir_expr *expr, hir_parser_t *parser);

static
bool __expr(struct hir_expr *self, hir_parser_t *parser);
static
bool __cast(struct hir_expr *self, hir_parser_t *parser);
static
bool __assignation(struct hir_expr *self, hir_parser_t *parser);

static FORCEINLINE
bool __required(rule_t *rule, struct hir_expr *expr, hir_parser_t *parser)
{
	return hir_parser_required((void *) rule, expr, parser, "expression");
}

static FORCEINLINE
struct hir_expr *__consume(rule_t *rule, hir_parser_t *parser)
{
	struct hir_expr expr;

	bzero(&expr, sizeof(struct hir_expr));
	return __required(rule, &expr, parser)
		? memdup(&expr, sizeof(struct hir_expr)) : NULL;
}

static FORCEINLINE
hir_tok_t *__commas(vecof(hir_expr_t) *exprs, hir_parser_t *parser,
					bool trailing_comma, u8_t closing)
{
	hir_tok_t *tok;
	struct hir_expr expr;

	*exprs = NULL;

	do {
		bzero(&expr, sizeof(struct hir_expr));

		if (((veclen(*exprs) || !trailing_comma)
			&& !__required(__assignation, &expr, parser))
				|| !__assignation(&expr, parser))
			return hir_parser_consume(parser, closing);

		vecpush(*exprs, expr);
		tok = hir_parser_any(parser, (u8_t[]) { ',', closing, 0 });

	} while (tok->kind && tok->kind == ',');

	return tok;
}

static
bool __primary(struct hir_expr *self, hir_parser_t *parser)
{
	hir_tok_t *tok;

	switch ((tok = hir_parser_peek(parser))->kind) {
		case HIR_TOK_LIT_NUMBER: case HIR_TOK_LIT_STRING:
		case HIR_TOK_LIT_CHAR: case HIR_TOK_LIT_BOOL: case HIR_TOK_LIT_NULL: {
			hir_parser_match(&self->spanned, parser, tok, HIR_EXPR_LIT);
			hir_lit_resolve(&self->lit, tok, parser->lexer->diags);
			break;
		}
		case HIR_TOK_IDENT: {
			hir_parser_match(&self->spanned, parser, tok, HIR_EXPR_IDENT);
			self->ident = tok->ident;
			break;
		}
		case '(': {
			hir_parser_match(&self->spanned, parser, tok, HIR_EXPR_PAREN);
			self->paren.expr = __consume(__expr, parser);
			tok = hir_parser_consume(parser, ')');
			spanned_diff(&self->spanned, &tok->spanned);
			break;
		}
		case '[': {
			hir_parser_match(&self->spanned, parser, tok, HIR_EXPR_ARRAY);
			tok = __commas(&self->array.elems, parser, true, ']');
			spanned_diff(&self->spanned, &tok->spanned);
			break;
		}
		default:
			return false;
	}

	return true;
}

static
bool __postfix(struct hir_expr *self, hir_parser_t *parser)
{
	hir_tok_t *tok;

	if (!__primary(self, parser))
		return false;

	while (true)
		switch ((tok = hir_parser_peek(parser))->kind) {
			case '(': {
				self->call.callee = memdup(self, sizeof(struct hir_expr));
				hir_parser_match(&self->spanned, parser, tok, HIR_EXPR_CALL);
				tok = __commas(&self->array.elems, parser, false, ')');
				spanned_diff(&self->spanned, &tok->spanned);
				break;
			}
			case '[': {
				self->binary.lhs = memdup(self, sizeof(struct hir_expr));
				hir_parser_match(&self->spanned, parser, tok, HIR_BINARY_INDEX);
				self->binary.rhs = __consume(__expr, parser);
				tok = hir_parser_consume(parser, ']');
				spanned_diff(&self->spanned, &tok->spanned);
				break;
			}
			default:
				return true;
		}
}

static
bool __unary(struct hir_expr *self, hir_parser_t *parser)
{
	hir_tok_t *tok;

	switch ((tok = hir_parser_peek(parser))->kind) {
		case HIR_TOK_DEC: case HIR_TOK_INC:
		case '&': case '*': case '!': case '~': case '+': case '-': {
			hir_parser_match(&self->spanned, parser, tok, tok->kind);
			self->unary.operand = __consume(__cast, parser);
			tok = hir_parser_peek(parser);
			spanned_diff(&self->spanned, &tok->spanned);

			return true;
		}
		default: return __postfix(self, parser);
	}
}

static
bool __cast(struct hir_expr *self, hir_parser_t *parser)
{
	hir_tok_t *tok;

	if (!__unary(self, parser))
		return false;

	if ((tok = hir_parser_peek(parser))->kind == HIR_TOK_AS) {
		self->cast.expr = memdup(self, sizeof(struct hir_expr));
		hir_parser_match(&self->spanned, parser, tok, HIR_EXPR_CAST);
		hir_ty_consume(&self->cast.ty, parser);
		tok = hir_parser_peek(parser);
		spanned_diff(&self->spanned, &tok->spanned);
	}

	return true;
}

static FORCEINLINE
bool __binary(struct hir_expr *self, hir_parser_t *parser, rule_t *rule,
			  u8_t const *ops)
{
	hir_tok_t *tok;

	if (!rule(self, parser))
		return false;

	while ((tok = hir_parser_peek(parser))->kind
		   && strchr((const char *) ops, tok->kind)) {
		self->binary.lhs = memdup(self, sizeof(struct hir_expr));
		hir_parser_match(&self->spanned, parser, tok, tok->kind);
		self->binary.rhs = __consume(rule, parser);
		tok = hir_parser_peek(parser);
		spanned_diff(&self->spanned, &tok->spanned);
	}

	return true;
}

static
bool __multiplicative(struct hir_expr *self, hir_parser_t *parser)
{
	return __binary(self, parser, __cast, (u8_t[]) { '*', '/', '%', 0 });
}

static
bool __additive(struct hir_expr *self, hir_parser_t *parser)
{
	return __binary(self, parser, __multiplicative, (u8_t[]) { '+', '-', 0 });
}

static
bool __shift(struct hir_expr *self, hir_parser_t *parser)
{
	return __binary(self, parser, __additive,
		(u8_t[]) { HIR_TOK_LSH, HIR_TOK_RSH, 0 });
}

static
bool __relational(struct hir_expr *self, hir_parser_t *parser)
{
	return __binary(self, parser, __shift,
		(u8_t[]) { '>', HIR_TOK_GEQ, '<', HIR_TOK_LEQ, 0 });
}

static
bool __equality(struct hir_expr *self, hir_parser_t *parser)
{
	return __binary(self, parser, __relational,
		(u8_t[]) { HIR_TOK_EQ, HIR_TOK_NEQ, 0 });
}

static
bool __add(struct hir_expr *self, hir_parser_t *parser)
{
	return __binary(self, parser, __equality, (u8_t[]) { '&', 0 });
}

static
bool __xor(struct hir_expr *self, hir_parser_t *parser)
{
	return __binary(self, parser, __add, (u8_t[]) { '^', 0 });
}

static
bool __or(struct hir_expr *self, hir_parser_t *parser)
{
	return __binary(self, parser, __xor, (u8_t[]) { '|', 0 });
}

static
bool __land(struct hir_expr *self, hir_parser_t *parser)
{
	return __binary(self, parser, __or, (u8_t[]) { HIR_TOK_LAND, 0 });
}

static
bool __lor(struct hir_expr *self, hir_parser_t *parser)
{
	return __binary(self, parser, __land, (u8_t[]) { HIR_TOK_LOR, 0 });
}

static
bool __assignation(struct hir_expr *self, hir_parser_t *parser)
{
	hir_tok_t *tok;

	if (!__lor(self, parser))
		return false;

	switch ((tok = hir_parser_peek(parser))->kind) {
		case '=':
		case HIR_TOK_LSH_ASSIGN: case HIR_TOK_RSH_ASSIGN:
		case HIR_TOK_AND_ASSIGN: case HIR_TOK_OR_ASSIGN:
		case HIR_TOK_XOR_ASSIGN: case HIR_TOK_ADD_ASSIGN:
		case HIR_TOK_SUB_ASSIGN: case HIR_TOK_MUL_ASSIGN:
		case HIR_TOK_DIV_ASSIGN: case HIR_TOK_MOD_ASSIGN: {
			self->binary.lhs = memdup(self, sizeof(struct hir_expr));
			hir_parser_match(&self->spanned, parser, tok, tok->kind);
			self->binary.rhs = __consume(__lor, parser);
			tok = hir_parser_peek(parser);
			spanned_diff(&self->spanned, &tok->spanned);
			break;
		}
		default:
			break;
	}

	return true;
}

static
bool __expr(struct hir_expr *self, hir_parser_t *parser)
{
	hir_tok_t *tok;
	struct hir_expr lhs;

	if (!__assignation(self, parser))
		return false;
	if (hir_parser_peek(parser)->kind != ',')
		return true;

	lhs = *self;
	self->kind = HIR_EXPR_TUPLE;
	self->tuple.elems = NULL;
	vecpush(self->tuple.elems, lhs);

	do {
		hir_parser_next(parser);
		bzero(&lhs, sizeof(struct hir_expr));

		if (__required(__assignation, &lhs, parser))
			vecpush(self->tuple.elems, lhs);

	} while (hir_parser_peek(parser)->kind == ',');

	tok = hir_parser_peek(parser);
	spanned_diff(&self->spanned, &tok->spanned);

	return true;
}

FORCEINLINE
bool hir_expr_parse(struct hir_expr *expr, hir_parser_t *parser)
{
	bzero(expr, sizeof(struct hir_expr));
	return __expr(expr, parser);
}

FORCEINLINE
void hir_expr_consume(struct hir_expr *expr, hir_parser_t *parser)
{
	bzero(expr, sizeof(struct hir_expr));
	__required(__expr, expr, parser);
}

FORCEINLINE
void hir_expr_destroy(struct hir_expr *self)
{
	(void)self;
	/* TODO */
}
