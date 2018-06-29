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
#include "guts/hir/parser.h"

typedef parse_st_t (rule_t)(hir_expr_t *expr, hir_parser_t *parser);

static parse_st_t __expr(hir_expr_t *expr, hir_parser_t *parser);
static parse_st_t __cast(hir_expr_t *expr, hir_parser_t *parser);

/*!@brief
 *
 * @code
 * __literal
 *   : HIR_TOK_LIT_NUMBER
 *   | HIR_TOK_LIT_STRING
 *   | HIR_TOK_LIT_CHAR
 *   | HIR_TOK_LIT_BOOL
 *   | HIR_TOK_LIT_NULL
 *   ;
 * @endcode
 *
 * @param expr
 * @param parser
 * @return
 */
static parse_st_t __literal(hir_expr_t *expr, hir_parser_t *parser)
{
	hir_tok_t *tok;

	if (!(tok = hir_parser_peek(parser)))
		return PARSE_ERROR;

	switch (tok->kind) {
		case HIR_TOK_LIT_NUMBER:
		case HIR_TOK_LIT_STRING:
		case HIR_TOK_LIT_CHAR:
		case HIR_TOK_LIT_BOOL:
		case HIR_TOK_LIT_NULL: {
			expr->span = tok->span;
			expr->kind = HIR_EXPR_LIT;
			hir_lit_resolve(&expr->lit, tok, parser->lexer->diags);
			hir_parser_next(parser);
			return PARSE_OK;
		}
		default:
			return PARSE_NONE;
	}
}

static parse_st_t __required(rule_t *rule, hir_expr_t *expr,
							 hir_parser_t *parser)
{
	parse_st_t st;
	hir_tok_t *tok;

	if ((st = rule(expr, parser)) == PARSE_ERROR)
		return st;
	else if (st == PARSE_NONE) {
		diag_t error;

		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		diag_error(&error,
			"expected expression before ‘%s’ token",
			hir_tok_toa(tok->kind));
		diag_labelize(&error, true, tok->span, NULL);
		codemap_diagnostic(parser->codemap, error);

		return PARSE_ERROR;
	}

	return PARSE_OK;
}

static hir_tok_t *__commas(vecof(hir_expr_t *) *exprs, hir_parser_t *parser,
						   bool trailing_comma, bool empty,
						   hir_tok_kind_t closing)
{
	parse_st_t st;
	hir_tok_t *tok;
	hir_expr_t r;

	while (true) {
		bzero(&r, sizeof(hir_expr_t));

		if ((st = (!trailing_comma && (veclen(*exprs) || !empty)
			? __required(__expr, &r, parser) : __expr(&r, parser)))
				== PARSE_ERROR)
			return NULL;
		if (st == PARSE_NONE)
			return hir_parser_consume(parser, closing);

		vecpush(*exprs, memdup(&r, sizeof(hir_expr_t)));

		tok = hir_parser_any(parser, (char __const[]){
			closing, HIR_TOK_COMMA, HIR_TOK_EOF
		});
		if (!tok || tok->kind == closing)
			return tok;
	}
}

/*!@brief
 *
 * @code
 * __primary
 *   : __literal
 *   | HIR_TOK_IDENT
 *   | '(' __required_expr ')'
 *   | '(' __required_expr, __commas ')'
 *   | '[' __commas ']'
 *   | '[' __commas ',' ']'
 *   ;
 * @endcode
 *
 * @param expr
 * @param parser
 * @return
 */
static parse_st_t __primary(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_tok_t *tok;

	if ((st = __literal(expr, parser)) != PARSE_NONE)
		return st;
	if (!(tok = hir_parser_peek(parser)))
		return PARSE_ERROR;

	switch (tok->kind) {
		case HIR_TOK_IDENT: {
			expr->span = tok->span;
			expr->kind = HIR_EXPR_IDENT;
			expr->ident = tok->ident;
			hir_parser_next(parser);

			return PARSE_OK;
		}
		case HIR_TOK_LPAR: {
			hir_expr_t *r;

			expr->span = tok->span;
			hir_parser_next(parser);

			if (!(tok = __commas(&expr->tuple.elems, parser, false, false,
				HIR_TOK_RPAR)))
				return PARSE_ERROR;
			expr->span.length = span_diff(tok->span, expr->span);

			if (veclen(expr->tuple.elems) != 1)
				expr->kind = HIR_EXPR_TUPLE;
			else {
				r = expr->tuple.elems[0];
				vecdtor(expr->tuple.elems);
				expr->kind = HIR_EXPR_PAREN;
				expr->paren.expr = r;
			}

			return PARSE_OK;
		}
		case HIR_TOK_LBRA: {
			expr->kind = HIR_EXPR_ARRAY;
			expr->span = tok->span;
			hir_parser_next(parser);

			if (!(tok = __commas(&expr->array.elems, parser, true, true,
				HIR_TOK_RBRA)))
				return PARSE_ERROR;

			expr->span.length = span_diff(tok->span, expr->span);

			return PARSE_OK;
		}
		default:
			return PARSE_NONE;
	}
}

/*!@brief
 *
 * @code
 * __postfix
 *   : __primary
 *   | __postfix '(' __commas ')'
 *   | __postfix '[' __required_expr ']'
 *   | __postfix '.' HIR_TOK_IDENT #TODO
 *   | __postfix '->' HIR_TOK_IDENT #TODO
 *   | __postfix HIR_UNARY_POST_DEC #TODO
 *   | __postfix HIR_UNARY_POST_INC #TODO
 *   ;
 * @endcode
 *
 * @param expr
 * @param parser
 * @return
 */
static parse_st_t __postfix(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_expr_t r1;
	hir_tok_t *tok;

	bzero(&r1, sizeof(hir_expr_t));
	if ((st = __primary(&r1, parser)) != PARSE_OK)
		return st;

	while (true) {
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		switch (tok->kind) {
			case HIR_TOK_LPAR: {
				expr->span = tok->span;
				expr->kind = HIR_EXPR_CALL;
				expr->call.callee = memdup(&r1, sizeof(hir_expr_t));
				hir_parser_next(parser);

				if (!(tok = __commas(&expr->call.arguments, parser, false, true,
					HIR_TOK_RPAR)))
					return PARSE_ERROR;

				expr->span.length = span_diff(tok->span, expr->span);

				return PARSE_OK;
			}
			case HIR_TOK_LBRA: {
				expr->span = tok->span;
				expr->kind = HIR_EXPR_BINARY;
				expr->binary.lhs = memdup(&r1, sizeof(hir_expr_t));
				hir_parser_next(parser);

				bzero(&r1, sizeof(hir_expr_t));
				if ((st = __required(__expr, &r1, parser)) == PARSE_ERROR)
					return st;

				if (!(tok = hir_parser_consume(parser, HIR_TOK_RBRA)))
					return PARSE_ERROR;

				expr->span.length = span_diff(tok->span, expr->span);
				expr->binary.rhs = memdup(&r1, sizeof(hir_expr_t));
				expr->binary.operator = HIR_BINARY_INDEX;

				return PARSE_OK;
			}
			default: {
				*expr = r1;
				return PARSE_OK;
			}
		}
	}
}

static parse_st_t __unary(hir_expr_t *expr, hir_parser_t *parser)
{
	hir_tok_t *tok;

	if (!(tok = hir_parser_peek(parser)))
		return PARSE_ERROR;

	switch (tok->kind) {
		case HIR_TOK_AND:
		case HIR_TOK_MUL:
		case HIR_TOK_NOT:
		case HIR_TOK_TID:
		case HIR_TOK_ADD:
		case HIR_TOK_SUB:
		case HIR_TOK_DEC:
		case HIR_TOK_INC: {
			hir_expr_t operand;

			expr->span = tok->span;
			hir_parser_next(parser);
			bzero(&operand, sizeof(hir_expr_t));

			if (__required(__cast, &operand, parser) == PARSE_ERROR)
				return PARSE_ERROR;

			expr->kind = HIR_EXPR_UNARY;
			expr->unary.operator = (hir_unary_kind_t)tok->kind;
			expr->unary.operand = memdup(&operand, sizeof(hir_expr_t));

			return PARSE_OK;
		}
		default:
			return __postfix(expr, parser);
	}
}

static parse_st_t __cast(hir_expr_t *expr, hir_parser_t *parser)
{
	return __unary(expr, parser);
}

static parse_st_t __multiplicative(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_expr_t lhs;
	hir_tok_t *tok;

	bzero(&lhs, sizeof(hir_expr_t));
	if ((st = __cast(&lhs, parser)) != PARSE_OK)
		return st;

	while (true) {
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		switch (tok->kind) {
			case HIR_TOK_MUL:
			case HIR_TOK_DIV:
			case HIR_TOK_MOD: {
				hir_expr_t rhs;

				expr->span = tok->span;
				hir_parser_next(parser);
				bzero(&rhs, sizeof(hir_expr_t));

				if (__required(__cast, &rhs, parser) == PARSE_ERROR)
					return PARSE_ERROR;

				expr->kind = HIR_EXPR_BINARY;
				expr->binary.operator = (hir_binary_kind_t)tok->kind;
				expr->binary.lhs = memdup(&lhs, sizeof(hir_expr_t));
				expr->binary.rhs = memdup(&rhs, sizeof(hir_expr_t));

				lhs = *expr;
				break;
			}
			default:
				*expr = lhs;
				return PARSE_OK;
		}
	}
}

static parse_st_t __additive(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_expr_t lhs;
	hir_tok_t *tok;

	bzero(&lhs, sizeof(hir_expr_t));
	if ((st = __multiplicative(&lhs, parser)) != PARSE_OK)
		return st;

	while (true) {
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		switch (tok->kind) {
			case HIR_TOK_ADD:
			case HIR_TOK_SUB: {
				hir_expr_t rhs;

				expr->span = tok->span;
				hir_parser_next(parser);
				bzero(&rhs, sizeof(hir_expr_t));

				if (__required(__multiplicative, &rhs, parser) == PARSE_ERROR)
					return PARSE_ERROR;

				expr->kind = HIR_EXPR_BINARY;
				expr->binary.operator = (hir_binary_kind_t)tok->kind;
				expr->binary.lhs = memdup(&lhs, sizeof(hir_expr_t));
				expr->binary.rhs = memdup(&rhs, sizeof(hir_expr_t));

				lhs = *expr;
				break;
			}
			default:
				*expr = lhs;
				return PARSE_OK;
		}
	}
}

static parse_st_t __shift(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_expr_t lhs;
	hir_tok_t *tok;

	bzero(&lhs, sizeof(hir_expr_t));
	if ((st = __additive(&lhs, parser)) != PARSE_OK)
		return st;

	while (true) {
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		switch (tok->kind) {
			case HIR_TOK_LSH:
			case HIR_TOK_RSH: {
				hir_expr_t rhs;

				expr->span = tok->span;
				hir_parser_next(parser);
				bzero(&rhs, sizeof(hir_expr_t));

				if (__required(__additive, &rhs, parser) == PARSE_ERROR)
					return PARSE_ERROR;

				expr->kind = HIR_EXPR_BINARY;
				expr->binary.operator = (hir_binary_kind_t)tok->kind;
				expr->binary.lhs = memdup(&lhs, sizeof(hir_expr_t));
				expr->binary.rhs = memdup(&rhs, sizeof(hir_expr_t));

				lhs = *expr;
				break;
			}
			default:
				*expr = lhs;
				return PARSE_OK;
		}
	}
}

static parse_st_t __relational(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_expr_t lhs;
	hir_tok_t *tok;

	bzero(&lhs, sizeof(hir_expr_t));
	if ((st = __shift(&lhs, parser)) != PARSE_OK)
		return st;

	while (true) {
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		switch (tok->kind) {
			case HIR_TOK_GT:
			case HIR_TOK_GEQ:
			case HIR_TOK_LT:
			case HIR_TOK_LEQ: {
				hir_expr_t rhs;

				expr->span = tok->span;
				hir_parser_next(parser);
				bzero(&rhs, sizeof(hir_expr_t));

				if (__required(__shift, &rhs, parser) == PARSE_ERROR)
					return PARSE_ERROR;

				expr->kind = HIR_EXPR_BINARY;
				expr->binary.operator = (hir_binary_kind_t)tok->kind;
				expr->binary.lhs = memdup(&lhs, sizeof(hir_expr_t));
				expr->binary.rhs = memdup(&rhs, sizeof(hir_expr_t));

				lhs = *expr;
				break;
			}
			default:
				*expr = lhs;
				return PARSE_OK;
		}
	}
}

static parse_st_t __equality(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_expr_t lhs;
	hir_tok_t *tok;

	bzero(&lhs, sizeof(hir_expr_t));
	if ((st = __relational(&lhs, parser)) != PARSE_OK)
		return st;

	while (true) {
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		switch (tok->kind) {
			case HIR_TOK_EQ:
			case HIR_TOK_NEQ: {
				hir_expr_t rhs;

				expr->span = tok->span;
				hir_parser_next(parser);
				bzero(&rhs, sizeof(hir_expr_t));

				if (__required(__relational, &rhs, parser) == PARSE_ERROR)
					return PARSE_ERROR;

				expr->kind = HIR_EXPR_BINARY;
				expr->binary.operator = (hir_binary_kind_t)tok->kind;
				expr->binary.lhs = memdup(&lhs, sizeof(hir_expr_t));
				expr->binary.rhs = memdup(&rhs, sizeof(hir_expr_t));

				lhs = *expr;
				break;
			}
			default:
				*expr = lhs;
				return PARSE_OK;
		}
	}
}

static parse_st_t __add(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_expr_t lhs;
	hir_tok_t *tok;

	bzero(&lhs, sizeof(hir_expr_t));
	if ((st = __equality(&lhs, parser)) != PARSE_OK)
		return st;

	while (true) {
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		switch (tok->kind) {
			case HIR_TOK_AND: {
				hir_expr_t rhs;

				expr->span = tok->span;
				hir_parser_next(parser);
				bzero(&rhs, sizeof(hir_expr_t));

				if (__required(__equality, &rhs, parser) == PARSE_ERROR)
					return PARSE_ERROR;

				expr->kind = HIR_EXPR_BINARY;
				expr->binary.operator = (hir_binary_kind_t)tok->kind;
				expr->binary.lhs = memdup(&lhs, sizeof(hir_expr_t));
				expr->binary.rhs = memdup(&rhs, sizeof(hir_expr_t));

				lhs = *expr;
				break;
			}
			default:
				*expr = lhs;
				return PARSE_OK;
		}
	}
}

static parse_st_t __xor(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_expr_t lhs;
	hir_tok_t *tok;

	bzero(&lhs, sizeof(hir_expr_t));
	if ((st = __add(&lhs, parser)) != PARSE_OK)
		return st;

	while (true) {
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		switch (tok->kind) {
			case HIR_TOK_XOR: {
				hir_expr_t rhs;

				expr->span = tok->span;
				hir_parser_next(parser);
				bzero(&rhs, sizeof(hir_expr_t));

				if (__required(__add, &rhs, parser) == PARSE_ERROR)
					return PARSE_ERROR;

				expr->kind = HIR_EXPR_BINARY;
				expr->binary.operator = (hir_binary_kind_t)tok->kind;
				expr->binary.lhs = memdup(&lhs, sizeof(hir_expr_t));
				expr->binary.rhs = memdup(&rhs, sizeof(hir_expr_t));

				lhs = *expr;
				break;
			}
			default:
				*expr = lhs;
				return PARSE_OK;
		}
	}
}

static parse_st_t __or(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_expr_t lhs;
	hir_tok_t *tok;

	bzero(&lhs, sizeof(hir_expr_t));
	if ((st = __xor(&lhs, parser)) != PARSE_OK)
		return st;

	while (true) {
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		switch (tok->kind) {
			case HIR_TOK_OR: {
				hir_expr_t rhs;

				expr->span = tok->span;
				hir_parser_next(parser);
				bzero(&rhs, sizeof(hir_expr_t));

				if (__required(__xor, &rhs, parser) == PARSE_ERROR)
					return PARSE_ERROR;

				expr->kind = HIR_EXPR_BINARY;
				expr->binary.operator = (hir_binary_kind_t)tok->kind;
				expr->binary.lhs = memdup(&lhs, sizeof(hir_expr_t));
				expr->binary.rhs = memdup(&rhs, sizeof(hir_expr_t));

				lhs = *expr;
				break;
			}
			default:
				*expr = lhs;
				return PARSE_OK;
		}
	}
}

static parse_st_t __land(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_expr_t lhs;
	hir_tok_t *tok;

	bzero(&lhs, sizeof(hir_expr_t));
	if ((st = __or(&lhs, parser)) != PARSE_OK)
		return st;

	while (true) {
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		switch (tok->kind) {
			case HIR_TOK_LAND: {
				hir_expr_t rhs;

				expr->span = tok->span;
				hir_parser_next(parser);
				bzero(&rhs, sizeof(hir_expr_t));

				if (__required(__or, &rhs, parser) == PARSE_ERROR)
					return PARSE_ERROR;

				expr->kind = HIR_EXPR_BINARY;
				expr->binary.operator = (hir_binary_kind_t)tok->kind;
				expr->binary.lhs = memdup(&lhs, sizeof(hir_expr_t));
				expr->binary.rhs = memdup(&rhs, sizeof(hir_expr_t));

				lhs = *expr;
				break;
			}
			default:
				*expr = lhs;
				return PARSE_OK;
		}
	}
}

static parse_st_t __lor(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_expr_t lhs;
	hir_tok_t *tok;

	bzero(&lhs, sizeof(hir_expr_t));
	if ((st = __land(&lhs, parser)) != PARSE_OK)
		return st;

	while (true) {
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		switch (tok->kind) {
			case HIR_TOK_LAND: {
				hir_expr_t rhs;

				expr->span = tok->span;
				hir_parser_next(parser);
				bzero(&rhs, sizeof(hir_expr_t));

				if (__required(__land, &rhs, parser) == PARSE_ERROR)
					return PARSE_ERROR;

				expr->kind = HIR_EXPR_BINARY;
				expr->binary.operator = (hir_binary_kind_t)tok->kind;
				expr->binary.lhs = memdup(&lhs, sizeof(hir_expr_t));
				expr->binary.rhs = memdup(&rhs, sizeof(hir_expr_t));

				lhs = *expr;
				break;
			}
			default:
				*expr = lhs;
				return PARSE_OK;
		}
	}
}

static parse_st_t __expr(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_expr_t lhs;
	hir_tok_t *tok;

	bzero(&lhs, sizeof(hir_expr_t));
	if ((st = __lor(&lhs, parser)) != PARSE_OK)
		return st;

	if (!(tok = hir_parser_peek(parser)))
		return PARSE_ERROR;

	switch (tok->kind) {
		case HIR_TOK_ASSIGN:
		case HIR_TOK_LSH_ASSIGN:
		case HIR_TOK_RSH_ASSIGN:
		case HIR_TOK_AND_ASSIGN:
		case HIR_TOK_OR_ASSIGN:
		case HIR_TOK_XOR_ASSIGN:
		case HIR_TOK_ADD_ASSIGN:
		case HIR_TOK_SUB_ASSIGN:
		case HIR_TOK_MUL_ASSIGN:
		case HIR_TOK_DIV_ASSIGN:
		case HIR_TOK_MOD_ASSIGN: {
			hir_expr_t rhs;

			expr->span = tok->span;
			hir_parser_next(parser);
			bzero(&rhs, sizeof(hir_expr_t));

			if (__required(__lor, &rhs, parser) == PARSE_ERROR)
				return PARSE_ERROR;

			expr->kind = HIR_EXPR_BINARY;
			expr->binary.operator = (hir_binary_kind_t)tok->kind;
			expr->binary.lhs = memdup(&lhs, sizeof(hir_expr_t));
			expr->binary.rhs = memdup(&rhs, sizeof(hir_expr_t));

			return PARSE_OK;
		}
		default:
			*expr = lhs;
			return PARSE_OK;
	}
}

/*!@brief
 *
 * @param expr
 * @param parser
 * @return
 */
parse_st_t hir_expr_parse(hir_expr_t *expr, hir_parser_t *parser)
{
	bzero(expr, sizeof(hir_expr_t));
	return __expr(expr, parser);
}
