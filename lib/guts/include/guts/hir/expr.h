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

/*!@file guts/hir/expr.h
 * @author uael
 *
 * @addtogroup guts.hir @{
 */
#ifndef __GUTS_HIR_EXPR_H
# define __GUTS_HIR_EXPR_H

#include "type.h"

struct hir_stmt;

typedef enum {
	HIR_EXPR_LIT = 0,
	HIR_EXPR_IDENT,
	HIR_EXPR_PAREN,
	HIR_EXPR_TUPLE,
	HIR_EXPR_ARRAY,
	HIR_EXPR_CALL,
	HIR_EXPR_CAST,
	HIR_EXPR_UNARY,
	HIR_EXPR_BINARY,
} hir_expr_kind_t;

typedef enum {
	HIR_UNARY_REF = HIR_TOK_AND,
	HIR_UNARY_DEREF = HIR_TOK_MUL,
	HIR_UNARY_NOT = HIR_TOK_NOT,
	HIR_UNARY_TID = HIR_TOK_TID,
	HIR_UNARY_PLUS = HIR_TOK_ADD,
	HIR_UNARY_MINUS = HIR_TOK_SUB,
	HIR_UNARY_PRE_DEC = HIR_TOK_DEC,
	HIR_UNARY_PRE_INC = HIR_TOK_INC,
	HIR_UNARY_POST_DEC = HIR_UNARY_PRE_INC + 1,
	HIR_UNARY_POST_INC,
} hir_unary_kind_t;

typedef enum {
	HIR_BINARY_INDEX = 0,
	HIR_BINARY_LT = HIR_TOK_LT,
	HIR_BINARY_GT = HIR_TOK_GT,
	HIR_BINARY_LEQ = HIR_TOK_LEQ,
	HIR_BINARY_GEQ = HIR_TOK_GEQ,
	HIR_BINARY_EQ = HIR_TOK_EQ,
	HIR_BINARY_NEQ = HIR_TOK_NEQ,
	HIR_BINARY_LAND = HIR_TOK_LAND,
	HIR_BINARY_LOR = HIR_TOK_LOR,
	HIR_BINARY_MUL = HIR_TOK_MUL,
	HIR_BINARY_DIV = HIR_TOK_DIV,
	HIR_BINARY_MOD = HIR_TOK_MOD,
	HIR_BINARY_ADD = HIR_TOK_ADD,
	HIR_BINARY_SUB = HIR_TOK_SUB,
	HIR_BINARY_LSH = HIR_TOK_LSH,
	HIR_BINARY_RSH = HIR_TOK_RSH,
	HIR_BINARY_AND = HIR_TOK_AND,
	HIR_BINARY_OR = HIR_TOK_OR,
	HIR_BINARY_XOR = HIR_TOK_XOR,
	HIR_BINARY_ASSIGN = HIR_TOK_ASSIGN,
	HIR_BINARY_MUL_ASSIGN = HIR_TOK_MUL_ASSIGN,
	HIR_BINARY_DIV_ASSIGN = HIR_TOK_DIV_ASSIGN,
	HIR_BINARY_MOD_ASSIGN = HIR_TOK_MOD_ASSIGN,
	HIR_BINARY_PLUS_ASSIGN = HIR_TOK_ADD_ASSIGN,
	HIR_BINARY_MINUS_ASSIGN = HIR_TOK_SUB_ASSIGN,
	HIR_BINARY_LSHIFT_ASSIGN = HIR_TOK_LSH_ASSIGN,
	HIR_BINARY_RSHIFT_ASSIGN = HIR_TOK_RSH_ASSIGN,
	HIR_BINARY_AND_ASSIGN = HIR_TOK_AND_ASSIGN,
	HIR_BINARY_OR_ASSIGN = HIR_TOK_OR_ASSIGN,
	HIR_BINARY_XOR_ASSIGN = HIR_TOK_XOR_ASSIGN,
} hir_binary_kind_t;

typedef struct hir_expr {

	hir_expr_kind_t kind: 8;

	span_t span;

	union {
		hir_lit_t lit;
		hir_ident_t ident;

		struct {
			struct hir_expr *expr;
		} paren;

		struct {
			vecof(struct hir_expr *) elems;
		} tuple;

		struct {
			vecof(struct hir_expr *) elems;
		} array;

		struct {
			bool indirect;
			struct hir_expr *expr;
			hir_ident_t ident;
		} member;

		struct {
			struct hir_expr *callee;
			vecof(struct hir_expr *) arguments;
		} call;

		struct {
			hir_ty_t ty;
			struct hir_expr *expr;
		} cast;

		struct {
			hir_unary_kind_t operator: 8;
			struct hir_expr *operand;
		} unary;

		struct {
			hir_binary_kind_t operator: 8;
			struct hir_expr *lhs;
			struct hir_expr *rhs;
		} binary;
	};

} hir_expr_t;

__api hir_parse_t hir_expr_parse(hir_expr_t *expr, hir_parser_t *parser);
__api hir_parse_t hir_expr_consume(hir_expr_t *expr, hir_parser_t *parser);

#endif /* !__GUTS_HIR_EXPR_H */
/*!@} */
