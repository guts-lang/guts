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

/*!@file guts/hir/token.h
 * @author uael
 *
 * @addtogroup guts.hir @{
 */
#ifndef __GUTS_HIR_TOKEN_H
# define __GUTS_HIR_TOKEN_H

#include "guts/hir/literal.h"

enum {
	HIR_TOK_EOF = '\0',

	/* Literal */
	HIR_TOK_LIT_NUMBER = 1,
	HIR_TOK_LIT_STRING = 2,
	HIR_TOK_LIT_CHAR = 3,
	HIR_TOK_LIT_BOOL = 4,
	HIR_TOK_LIT_NULL = 5,
	HIR_TOK_IDENT = 6,

	/* Scalar */
	HIR_TOK_STRUCT = 7, /*!< ‘struct’. */
	HIR_TOK_CHAR = 8,   /*!< ‘char’.   */
	HIR_TOK_BOOL = 9,   /*!< ‘bool’.   */
	HIR_TOK_I8 = 10,    /*!< ‘i8’.     */
	HIR_TOK_I16 = 11,   /*!< ‘i16’.    */
	HIR_TOK_I32 = 12,   /*!< ‘i32’.    */
	HIR_TOK_I64 = 13,   /*!< ‘i64’.    */
	HIR_TOK_U8 = 14,    /*!< ‘u8’.     */
	HIR_TOK_U16 = 15,   /*!< ‘u16’.    */
	HIR_TOK_U32 = 16,   /*!< ‘u32’.    */
	HIR_TOK_U64 = 17,   /*!< ‘u64’.    */
	HIR_TOK_F32 = 18,   /*!< ‘f32’.    */
	HIR_TOK_F64 = 19,   /*!< ‘f64’.    */

	/* Keyword */
	HIR_TOK_INCLUDE = 20,   /*!< ‘include’.   */
	HIR_TOK_USE = 21,       /*!< ‘use’.       */
	HIR_TOK_NAMESPACE = 22, /*!< ‘namespace’. */
	HIR_TOK_LET = 23,       /*!< ‘let’.       */
	HIR_TOK_IF = 24,        /*!< ‘if’.        */
	HIR_TOK_ELSE = 25,      /*!< ‘else’.      */
	HIR_TOK_WHILE = 26,     /*!< ‘while’.     */
	HIR_TOK_CONTINUE = 27,  /*!< ‘continue’.  */
	HIR_TOK_BREAK = 28,     /*!< ‘break’.     */
	HIR_TOK_RETURN = 29,    /*!< ‘return’.    */
	HIR_TOK_AS = 30,        /*!< ‘as’.        */
	HIR_TOK_CONST = 31,     /*!< ‘const’.     */

	/* Symbol */
	HIR_TOK_COLON = ':',     /*!< ‘:’ (58). */
	HIR_TOK_SEMICOLON = ';', /*!< ‘;’ (59). */
	HIR_TOK_COMMA = ',',     /*!< ‘,’ (44). */
	HIR_TOK_DCOLON = 69,     /*!< ‘::’.     */

	/* Delimiter */
	HIR_TOK_LPAR = '(', /*!< ‘(’ (40).  */
	HIR_TOK_RPAR = ')', /*!< ‘)’ (41).  */
	HIR_TOK_LCUR = '{', /*!< ‘{’ (123). */
	HIR_TOK_RCUR = '}', /*!< ‘}’ (124). */
	HIR_TOK_LBRA = '[', /*!< ‘[’ (91).  */
	HIR_TOK_RBRA = ']', /*!< ‘]’ (93).  */

	/* Operator */
	HIR_TOK_NOT = '!',       /*!< ‘!’ (33).   */
	HIR_TOK_NIL = '?',       /*!< ‘?’ (63).   */
	HIR_TOK_TID = '~',       /*!< ‘~’ (126).  */
	HIR_TOK_INC = 32,        /*!< ‘++’.       */
	HIR_TOK_DEC = 34,        /*!< ‘--’.       */
	HIR_TOK_EQ = 35,         /*!< ‘==’.       */
	HIR_TOK_NEQ = 36,        /*!< ‘!=’.       */
	HIR_TOK_GT = '>',        /*!< ‘>’ (62).   */
	HIR_TOK_GEQ = 39,        /*!< ‘>=’.       */
	HIR_TOK_LT = '<',        /*!< ‘<’ (60).   */
	HIR_TOK_LEQ = 46,        /*!< ‘<=’.       */
	HIR_TOK_LAND = 48,       /*!< ‘&&’.       */
	HIR_TOK_LOR = 49,        /*!< ‘||’.       */
	HIR_TOK_LSH = 50,        /*!< ‘<<’.       */
	HIR_TOK_RSH = 51,        /*!< ‘>>’.       */
	HIR_TOK_AND = '&',       /*!< ‘&’ (38).   */
	HIR_TOK_OR = '|',        /*!< ‘|’ (124).  */
	HIR_TOK_XOR = '^',       /*!< ‘^’ (94).   */
	HIR_TOK_ADD = '+',       /*!< ‘+’ (43).   */
	HIR_TOK_SUB = '-',       /*!< ‘-’ (45).   */
	HIR_TOK_MUL = '*',       /*!< ‘*’ (42).   */
	HIR_TOK_DIV = '/',       /*!< ‘/’ (47).   */
	HIR_TOK_MOD = '%',       /*!< ‘%’ (37).   */
	HIR_TOK_ASSIGN = '=',    /*!< ‘=’ (61).   */
	HIR_TOK_LSH_ASSIGN = 52, /*!< ‘<<’.       */
	HIR_TOK_RSH_ASSIGN = 53, /*!< ‘>>’.       */
	HIR_TOK_AND_ASSIGN = 54, /*!< ‘&=’.       */
	HIR_TOK_OR_ASSIGN = 55,  /*!< ‘|=’.       */
	HIR_TOK_XOR_ASSIGN = 56, /*!< ‘^=’.       */
	HIR_TOK_ADD_ASSIGN = 57, /*!< ‘+=’.       */
	HIR_TOK_SUB_ASSIGN = 64, /*!< ‘-=’.       */
	HIR_TOK_MUL_ASSIGN = 65, /*!< ‘*=’.       */
	HIR_TOK_DIV_ASSIGN = 66, /*!< ‘/=’.       */
	HIR_TOK_MOD_ASSIGN = 67, /*!< ‘%=’.       */
	HIR_TOK_NIL_ASSIGN = 68, /*!< ‘?=’.       */

};

typedef char __const *hir_ident_t;

typedef struct hir_tok {

	union {
		spanned_t spanned;

		struct {
			span_t span;
			u8_t kind;
		};
	};

	union {
		char __const *lit_number;
		hir_string_t lit_string;
		hir_char_t lit_char;
		hir_bool_t lit_bool;
		hir_ident_t ident;
		u8_t syntax;
	};

} hir_tok_t;

__api void hir_tok_init(hir_tok_t *token, loc_t start, u16_t length);
__api void hir_tok_dtor(hir_tok_t *token);
__api char __const *hir_tok_toa(u8_t kind);
__api u16_t hir_tok_diff(hir_tok_t *a, hir_tok_t *b);
__api u8_t hir_tok_matching(u8_t kind);

#endif /* !__GUTS_HIR_FE_H */
/*!@} */
