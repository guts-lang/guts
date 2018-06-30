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

/*!@file guts/hir/stmt.h
 * @author uael
 *
 * @addtogroup guts.hir @{
 */
#ifndef __GUTS_HIR_STMT_H
# define __GUTS_HIR_STMT_H

#include "expr.h"
#include "scope.h"

typedef enum {
	HIR_STMT_EXPR,
	HIR_STMT_BLOCK,
	HIR_STMT_LET,
	HIR_STMT_IF,
	HIR_STMT_WHILE,
	HIR_STMT_CONTINUE,
	HIR_STMT_BREAK,
	HIR_STMT_RETURN,
} hir_stmt_kind_t;

typedef struct hir_stmt {

	hir_stmt_kind_t kind: 8;

	span_t span;

	union {
		hir_expr_t expr, return_expr;

		struct {
			vecof(struct hir_stmt *) stmts;
			hir_expr_t expr;
			hir_scope_t scope;
		} block;

		struct {
			hir_name_t name;
			hir_ty_t ty;
			hir_expr_t *value;
		} let;

		struct {
			hir_expr_t cond;
			struct hir_stmt *block, *else_part;
		} if_stmt;

		struct {
			hir_expr_t cond;
			struct hir_stmt *block;
		} while_stmt;
	};

} hir_stmt_t;

__api hir_parse_t hir_stmt_parse(hir_stmt_t *stmt, hir_parser_t *parser);
__api hir_parse_t hir_stmt_consume(hir_stmt_t *stmt, hir_parser_t *parser);

#endif /* !__GUTS_HIR_STMT_H */
/*!@} */
