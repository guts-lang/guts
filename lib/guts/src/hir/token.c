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

#include "guts/hir/token.h"

FORCEINLINE
void hir_tok_init(hir_tok_t *token, loc_t start, u16_t length)
{
	token->span = (span_t) {
		.start = start,
		.length = length
	};
}

void hir_tok_dtor(hir_tok_t *token)
{
	if (token->kind == HIR_TOK_LIT_STRING)
		vecdtor(token->lit_string);
	bzero(token, sizeof(hir_tok_t));
}

static char __const *__token_toa[] = {
	[HIR_TOK_EOF] = "<eof>",
	[HIR_TOK_LIT_NUMBER] = "<number>",
	[HIR_TOK_LIT_STRING] = "<string>",
	[HIR_TOK_LIT_CHAR] = "<char>",
	[HIR_TOK_LIT_BOOL] = "<false/true>",
	[HIR_TOK_LIT_NULL] = "null",
	[HIR_TOK_IDENT] = "<identifier>",
	[HIR_TOK_STRUCT] = "struct",
	[HIR_TOK_CHAR] = "char",
	[HIR_TOK_BOOL] = "bool",
	[HIR_TOK_I8] = "i8",
	[HIR_TOK_I16] = "i16",
	[HIR_TOK_I32] = "i32",
	[HIR_TOK_I64] = "i64",
	[HIR_TOK_U8] = "u8",
	[HIR_TOK_U16] = "u16",
	[HIR_TOK_U32] = "u32",
	[HIR_TOK_U64] = "u64",
	[HIR_TOK_F32] = "f32",
	[HIR_TOK_F64] = "f64",
	[HIR_TOK_INCLUDE] = "include",
	[HIR_TOK_USE] = "use",
	[HIR_TOK_NAMESPACE] = "namespace",
	[HIR_TOK_LET] = "let",
	[HIR_TOK_IF] = "if",
	[HIR_TOK_ELSE] = "else",
	[HIR_TOK_WHILE] = "while",
	[HIR_TOK_CONTINUE] = "continue",
	[HIR_TOK_BREAK] = "break",
	[HIR_TOK_RETURN] = "return",
	[HIR_TOK_AS] = "as",
	[HIR_TOK_CONST] = "const",
	[HIR_TOK_COLON] = ":",
	[HIR_TOK_DCOLON] = "::",
	[HIR_TOK_SEMICOLON] = ";",
	[HIR_TOK_COMMA] = ",",
	[HIR_TOK_LPAR] = "(",
	[HIR_TOK_RPAR] = ")",
	[HIR_TOK_LCUR] = "{",
	[HIR_TOK_RCUR] = "}",
	[HIR_TOK_LBRA] = "[",
	[HIR_TOK_RBRA] = "]",
	[HIR_TOK_TID] = "~",
	[HIR_TOK_NOT] = "!",
	[HIR_TOK_NIL] = "?",
	[HIR_TOK_INC] = "++",
	[HIR_TOK_DEC] = "--",
	[HIR_TOK_EQ] = "==",
	[HIR_TOK_NEQ] = "!=",
	[HIR_TOK_GT] = ">",
	[HIR_TOK_GEQ] = ">=",
	[HIR_TOK_LEQ] = "<=",
	[HIR_TOK_LT] = "<",
	[HIR_TOK_LAND] = "&&",
	[HIR_TOK_LOR] = "||",
	[HIR_TOK_LSH] = "<<",
	[HIR_TOK_RSH] = ">>",
	[HIR_TOK_AND] = "&",
	[HIR_TOK_OR] = "|",
	[HIR_TOK_XOR] = "^",
	[HIR_TOK_ADD] = "+",
	[HIR_TOK_SUB] = "-",
	[HIR_TOK_MUL] = "*",
	[HIR_TOK_DIV] = "/",
	[HIR_TOK_MOD] = "%",
	[HIR_TOK_ASSIGN] = "=",
	[HIR_TOK_LSH_ASSIGN] = "<<",
	[HIR_TOK_RSH_ASSIGN] = ">>",
	[HIR_TOK_AND_ASSIGN] = "&=",
	[HIR_TOK_OR_ASSIGN] = "|=",
	[HIR_TOK_XOR_ASSIGN] = "^=",
	[HIR_TOK_ADD_ASSIGN] = "+=",
	[HIR_TOK_SUB_ASSIGN] = "-=",
	[HIR_TOK_MUL_ASSIGN] = "*=",
	[HIR_TOK_DIV_ASSIGN] = "/=",
	[HIR_TOK_MOD_ASSIGN] = "%=",
	[HIR_TOK_NIL_ASSIGN] = "?=",
};

FORCEINLINE PURE
char __const *hir_tok_toa(u8_t kind)
{
	return __token_toa[kind];
}

FORCEINLINE
u16_t hir_tok_diff(hir_tok_t *a, hir_tok_t *b)
{
	return (u16_t)(a->span.start.off - b->span.start.off);
}

FORCEINLINE PURE
u8_t hir_tok_matching(u8_t kind)
{
	switch (kind) {
		case HIR_TOK_LPAR: return HIR_TOK_RPAR;
		case HIR_TOK_RPAR: return HIR_TOK_LPAR;
		case HIR_TOK_LBRA: return HIR_TOK_RBRA;
		case HIR_TOK_RBRA: return HIR_TOK_LBRA;
		case HIR_TOK_LCUR: return HIR_TOK_RCUR;
		case HIR_TOK_RCUR: return HIR_TOK_LCUR;
		case HIR_TOK_LT: return HIR_TOK_GT;
		case HIR_TOK_GT: return HIR_TOK_LT;
		default:
			return HIR_TOK_EOF;
	}
}
