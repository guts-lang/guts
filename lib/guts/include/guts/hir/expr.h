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

typedef struct hir_expr hir_expr_t;

struct hir_expr {

	span_t span;

	hir_ty_t *type;

	enum {
		HIR_EXPR_IDENT = 0,
		HIR_EXPR_LIT,
		HIR_EXPR_PAREN,
		HIR_EXPR_TUPLE,
		HIR_EXPR_ARRAY,
	} kind;

	union {
		hir_ident_t ident;
		hir_lit_t lit;

		struct {
			hir_expr_t *expr;
		} paren;

		struct {
			vecof(hir_expr_t *) elems;
		} tuple;

		struct {
			vecof(hir_expr_t *) elems;
		} array;

		struct {
			bool indirect;
			hir_expr_t *expr;
			hir_ident_t ident;
		} member;

		struct {
			hir_expr_t *callee;
			vecof(hir_expr_t *) arguments;
		} call;

		struct {
			enum {
				HIR_UNARY_POST_DEC,
				HIR_UNARY_POST_INC,
				HIR_UNARY_PRE_DEC,
				HIR_UNARY_PRE_INC,
				HIR_UNARY_REF,
				HIR_UNARY_DEREF,
				HIR_UNARY_PLUS,
				HIR_UNARY_MINUS,
				HIR_UNARY_TILDE,
				HIR_UNARY_NOT,
			} operator;

			hir_expr_t *operand;
		} unary;

		struct {
			enum {
				HIR_BINARY_INDEX,
				HIR_BINARY_LESS,
				HIR_BINARY_GREATER,
				HIR_BINARY_LESS_EQ,
				HIR_BINARY_GREATER_EQ,
				HIR_BINARY_EQ,
				HIR_BINARY_NEQ,
				HIR_BINARY_LAND,
				HIR_BINARY_LOR,

				HIR_BINARY_MUL,
				HIR_BINARY_DIV,
				HIR_BINARY_MOD,
				HIR_BINARY_PLUS,
				HIR_BINARY_MINUS,
				HIR_BINARY_LSHIFT,
				HIR_BINARY_RSHIFT,
				HIR_BINARY_AND,
				HIR_BINARY_OR,
				HIR_BINARY_XOR,
				HIR_BINARY_ASSIGN,
				HIR_BINARY_ASSIGN_MUL,
				HIR_BINARY_ASSIGN_DIV,
				HIR_BINARY_ASSIGN_MOD,
				HIR_BINARY_ASSIGN_PLUS,
				HIR_BINARY_ASSIGN_MINUS,
				HIR_BINARY_ASSIGN_LSHIFT,
				HIR_BINARY_ASSIGN_RSHIFT,
				HIR_BINARY_ASSIGN_AND,
				HIR_BINARY_ASSIGN_OR,
				HIR_BINARY_ASSIGN_XOR,
			} operator;

			hir_expr_t *lhs;
			hir_expr_t *rhs;
		} binary;

		struct {
			hir_expr_t *cond;
			hir_expr_t *block;
			hir_expr_t *else_branch;
		} cond;
	};
};

__api hir_expr_t *hir_expr_parse(hir_lexer_t *lexer);

#endif /* !__GUTS_HIR_EXPR_H */
/*!@} */
