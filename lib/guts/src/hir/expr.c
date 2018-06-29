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

#include <guts/hir/expr.h>
#include <il/span.h>
#include <il/loc.h>
#include <guts/hir/parser.h>
#include "guts/hir/expr.h"
#include "guts/hir/entity.h"
#include "guts/hir/parser.h"

static parse_st_t __expr(hir_expr_t *expr, hir_parser_t *parser);

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

static parse_st_t __required_expr(hir_expr_t *expr, hir_parser_t *parser)
{
	parse_st_t st;
	hir_tok_t *tok;

	if ((st = __expr(expr, parser)) == PARSE_ERROR)
		return st;
	else if (st == PARSE_NONE) {
		diag_t error;

		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		diag_error(&error,
			"unexpected token, expected `<expression>' got `%s'",
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
			? __required_expr : __expr)(&r, parser)) == PARSE_ERROR)
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
 *   | __postfix '--' #TODO
 *   | __postfix '++' #TODO
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
				if ((st = __required_expr(&r1, parser)) == PARSE_ERROR)
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

/*!@brief
 *
 * @code
 * __expr
 *   : __postfix
 *   ;
 * @endcode
 *
 * @param expr
 * @param parser
 * @return
 */
static parse_st_t __expr(hir_expr_t *expr, hir_parser_t *parser)
{
	return __postfix(expr, parser);
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
