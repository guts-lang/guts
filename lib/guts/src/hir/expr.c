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

	if (!(tok = hir_parser_peek(parser))) return PARSE_ERROR;

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

/*!@brief
 *
 * @code
 * __primary
 *   : __literal
 *   | HIR_TOK_IDENT
 *   | '(' __expr ')'
 *   | '('__commas ')' #TODO
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

	if ((st = __literal(expr, parser)) != PARSE_NONE) return st;
	if (!(tok = hir_parser_peek(parser))) return PARSE_ERROR;

	switch (tok->kind) {
		case HIR_TOK_IDENT: {
			hir_entity_t *ent;

			if (!(ent = hir_scope_locate(parser->current, tok->ident,
				tok->span))) {
				diag_t e;

				diag_error(&e, "‘%.*s’ undeclared (first use in this function)",
					(int) tok->span.length, tok->ident);
				diag_labelize(&e, true, tok->span, NULL);
				codemap_diagnostic(parser->codemap, e);
				return PARSE_ERROR;
			}
			expr->span = tok->span;
			expr->kind = HIR_EXPR_IDENT;
			expr->ident = tok->ident;
			hir_parser_next(parser);
			return PARSE_OK;
		}
		case HIR_TOK_LPAR: {
			hir_expr_t r;

			expr->span = tok->span;
			hir_parser_next(parser);

			if ((st = __expr(&r, parser)) != PARSE_OK)
				return st;

			expr->kind = HIR_EXPR_PAREN;
			expr->paren.expr = memdup(&r, sizeof(hir_expr_t));
			hir_parser_consume(parser, HIR_TOK_RPAR);
			return PARSE_OK;
		}
		default:
			return PARSE_NONE;
	}
}

/*!@brief
 *
 * @code
 * @endcode
 *
 * @param expr
 * @param parser
 * @return
 */
static parse_st_t __expr(hir_expr_t *expr, hir_parser_t *parser)
{
	return __primary(expr, parser);
}

/*!@brief
 *
 * literal_expr
 *   : HIR_TOK_LIT_NUMBER
 *   | HIR_TOK_LIT_STRING
 *   | HIR_TOK_LIT_CHAR
 *   | HIR_TOK_LIT_BOOL
 *   | HIR_TOK_LIT_NULL
 *   ;
 *
 * @param expr
 * @param parser
 * @return
 */
parse_st_t hir_expr_parse(hir_expr_t *expr, hir_parser_t *parser)
{
	return __expr(expr, parser);
}
