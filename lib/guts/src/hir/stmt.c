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

#include "guts/hir/stmt.h"

#if 0
FORCEINLINE
static hir_parse_t __scope(hir_stmt_t *stmt, hir_parser_t *parser,
						   hir_tok_t *tok)
{
	hir_stmt_t elem;

	hir_parser_next(parser);
	stmt->span = tok->span;
	stmt->kind = HIR_STMT_BLOCK;
	hir_parser_scope(parser, &stmt->block.scope);

	while (true) {
		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;

		if (tok->kind == HIR_TOK_RCUR)
			break;

		if (hir_stmt_consume(&elem, parser) == PARSE_ERROR)
			return PARSE_ERROR;

		vecpush(stmt->block.stmts, &elem);
	}

	if (!(tok = hir_parser_consume(parser, HIR_TOK_RCUR)))
		return PARSE_ERROR;

	hir_parser_unscope(parser);
	stmt->span.length = span_diff(tok->span, stmt->span);

	return PARSE_OK;
}

FORCEINLINE
static hir_parse_t __let(hir_stmt_t *stmt, hir_parser_t *parser, hir_tok_t *tok)
{
	hir_parser_next(parser);
	stmt->span = tok->span;
	stmt->kind = HIR_STMT_LET;

	if (!(tok = hir_parser_consume(parser, HIR_TOK_IDENT)))
		return PARSE_ERROR;

	stmt->let.name.ident = tok->ident;
	stmt->let.name.len = tok->span.length;

	if (!hir_parser_consume(parser, HIR_TOK_COLON))
		return PARSE_ERROR;

	if (hir_ty_consume(&stmt->let.ty, parser) == PARSE_ERROR)
		return PARSE_ERROR;

	if (!(tok = hir_parser_any(parser,
		(char []){ HIR_TOK_SEMICOLON, HIR_TOK_ASSIGN, HIR_TOK_EOF })))
		return PARSE_ERROR;

	if (tok->kind == HIR_TOK_ASSIGN) {
		hir_expr_t expr;

		if (hir_expr_consume(&expr, parser) == PARSE_ERROR)
			return PARSE_ERROR;

		stmt->let.value = memdup(&expr, sizeof(hir_expr_t));

		if (!(tok = hir_parser_consume(parser, HIR_TOK_SEMICOLON)))
			return PARSE_ERROR;
	}

	stmt->span.length = span_diff(tok->span, stmt->span);

	return PARSE_OK;
}

FORCEINLINE
static hir_parse_t __if(hir_stmt_t *stmt, hir_parser_t *parser, hir_tok_t *tok)
{
	hir_stmt_t body;

	hir_parser_next(parser);
	stmt->span = tok->span;
	stmt->kind = HIR_STMT_IF;

	if (hir_expr_consume(&stmt->if_stmt.cond, parser) == PARSE_ERROR)
		return PARSE_ERROR;

	if (hir_stmt_consume(&body, parser) == PARSE_ERROR)
		return PARSE_ERROR;

	stmt->if_stmt.block = memdup(&body, sizeof(hir_stmt_t));

	if (!(tok = hir_parser_peek(parser)))
		return PARSE_ERROR;

	if (tok->kind == HIR_TOK_ELSE) {
		hir_parser_next(parser);

		if (hir_stmt_consume(&body, parser) == PARSE_ERROR)
			return PARSE_ERROR;

		stmt->if_stmt.else_part = memdup(&body, sizeof(hir_stmt_t));

		if (!(tok = hir_parser_peek(parser)))
			return PARSE_ERROR;
	}

	stmt->span.length = span_diff(tok->span, stmt->span);

	return PARSE_OK;
}

FORCEINLINE
static hir_parse_t __while(hir_stmt_t *stmt, hir_parser_t *parser,
						   hir_tok_t *tok)
{
	hir_stmt_t body;

	hir_parser_next(parser);
	stmt->span = tok->span;
	stmt->kind = HIR_STMT_WHILE;

	if (hir_expr_consume(&stmt->while_stmt.cond, parser) == PARSE_ERROR)
		return PARSE_ERROR;

	if (hir_stmt_consume(&body, parser) == PARSE_ERROR)
		return PARSE_ERROR;

	stmt->while_stmt.block = memdup(&body, sizeof(hir_stmt_t));

	if (!(tok = hir_parser_peek(parser)))
		return PARSE_ERROR;

	stmt->span.length = span_diff(tok->span, stmt->span);

	return PARSE_OK;
}

FORCEINLINE
static hir_parse_t __continue(hir_stmt_t *stmt, hir_parser_t *parser,
							  hir_tok_t *tok)
{
	hir_parser_next(parser);
	stmt->span = tok->span;
	stmt->kind = HIR_STMT_CONTINUE;

	if (!(tok = hir_parser_consume(parser, HIR_TOK_SEMICOLON)))
		return PARSE_ERROR;

	stmt->span.length = span_diff(tok->span, stmt->span);

	return PARSE_OK;
}

FORCEINLINE
static hir_parse_t __break(hir_stmt_t *stmt, hir_parser_t *parser,
						   hir_tok_t *tok)
{
	hir_parser_next(parser);
	stmt->span = tok->span;
	stmt->kind = HIR_STMT_BREAK;

	if (!(tok = hir_parser_consume(parser, HIR_TOK_SEMICOLON)))
		return PARSE_ERROR;

	stmt->span.length = span_diff(tok->span, stmt->span);

	return PARSE_OK;
}

FORCEINLINE
static hir_parse_t __return(hir_stmt_t *stmt, hir_parser_t *parser,
							hir_tok_t *tok)
{
	hir_parser_next(parser);
	stmt->span = tok->span;
	stmt->kind = HIR_STMT_RETURN;

	if (hir_expr_parse(&stmt->return_expr, parser) == PARSE_ERROR)
		return PARSE_ERROR;

	if (!(tok = hir_parser_consume(parser, HIR_TOK_SEMICOLON)))
		return PARSE_ERROR;

	stmt->span.length = span_diff(tok->span, stmt->span);

	return PARSE_OK;
}

FORCEINLINE
static hir_parse_t __expr(hir_stmt_t *stmt, hir_parser_t *parser,
						  hir_tok_t *tok)
{
	stmt->span = tok->span;
	stmt->kind = HIR_STMT_EXPR;

	if (hir_expr_parse(&stmt->expr, parser) == PARSE_ERROR)
		return PARSE_ERROR;

	if (!(tok = hir_parser_consume(parser, HIR_TOK_SEMICOLON)))
		return PARSE_ERROR;

	stmt->span.length = span_diff(tok->span, stmt->span);

	return PARSE_OK;
}

hir_parse_t hir_stmt_parse(hir_stmt_t *stmt, hir_parser_t *parser)
{
	hir_tok_t *tok;

	bzero(stmt, sizeof(hir_stmt_t));

	if (!(tok = hir_parser_peek(parser)))
		return PARSE_ERROR;

	switch (tok->kind) {
		case HIR_TOK_LCUR:
			return __scope(stmt, parser, tok);
		case HIR_TOK_LET:
			return __let(stmt, parser, tok);
		case HIR_TOK_IF:
			return __if(stmt, parser, tok);
		case HIR_TOK_WHILE:
			return __while(stmt, parser, tok);
		case HIR_TOK_CONTINUE:
			return __continue(stmt, parser, tok);
		case HIR_TOK_BREAK:
			return __break(stmt, parser, tok);
		case HIR_TOK_RETURN:
			return __return(stmt, parser, tok);
		default:
			return __expr(stmt, parser, tok);
	}
}

hir_parse_t hir_stmt_consume(hir_stmt_t *stmt, hir_parser_t *parser)
{
	return hir_parse_required((hir_parse_rule_t *)hir_stmt_parse, stmt, parser,
		"statement");
}
#endif
