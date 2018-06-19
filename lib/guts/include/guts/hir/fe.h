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

/*!@file guts/hir/fe.h
 * @author uael
 *
 * @addtogroup guts.hir @{
 */
#ifndef __GUTS_HIR_FE_H
# define __GUTS_HIR_FE_H

#include <ds/deque.h>
#include <il/codemap.h>

#include "guts/conf.h"

typedef char __const *hir_ident_t;

typedef enum {
	HIR_INTEGER_UNRESOLVED = 0,
	HIR_INTEGER_DECIMAL,
	HIR_INTEGER_OCTAL,
	HIR_INTEGER_HEXADECIMAL,
} hit_integer_base_t;

typedef enum {
	HIR_INTEGER_INT,
	HIR_INTEGER_LONG,
	HIR_INTEGER_LLONG,
} hit_integer_size_t;

typedef struct {
	hit_integer_base_t base;
	hit_integer_size_t size;
	bool unsign;
	char __const * number;
} hir_integer_t;

typedef enum {
	HIR_FLOAT_UNRESOLVED = 0,
	HIR_FLOAT_DECIMAL,
	HIR_FLOAT_HEXADECIMAL,
} hit_float_base_t;

typedef enum {
	HIR_FLOAT_FLOAT,
	HIR_FLOAT_DOUBLE,
	HIR_FLOAT_LDOUBLE
} hit_float_size_t;

typedef struct {
	hit_float_base_t base;
	hit_float_size_t size;
	char __const * number;
} hir_float_t;

typedef enum {
	HIR_TOK_EOF = 0,

	/* Literal */
	HIR_TOK_LIT_CHAR,
	HIR_TOK_LIT_INTEGER,
	HIR_TOK_LIT_FLOAT,
	HIR_TOK_LIT_STRING,
	HIR_TOK_IDENT,

	/* Scalar */
	HIR_TOK_STRUCT, /* struct */
	HIR_TOK_CHAR,   /* char */
	HIR_TOK_I8,     /* i8 */
	HIR_TOK_I16,    /* i16 */
	HIR_TOK_I32,    /* i32 */
	HIR_TOK_I64,    /* i64 */
	HIR_TOK_U8,     /* u8 */
	HIR_TOK_U16,    /* u16 */
	HIR_TOK_U32,    /* u32 */
	HIR_TOK_U64,    /* u64 */
	HIR_TOK_F32,    /* f32 */
	HIR_TOK_F64,    /* f64 */

	/* Keyword */
	HIR_TOK_USE,       /* use */
	HIR_TOK_NAMESPACE, /* namespace */
	HIR_TOK_IF,        /* if */
	HIR_TOK_ELSE,      /* else */
	HIR_TOK_RETURN,    /* return */

	/* Symbol */
	HIR_TOK_COLON,     /* : */
	HIR_TOK_SEMICOLON, /* ; */
	HIR_TOK_COMMA,     /* , */

	/* Delimiter */
	HIR_TOK_LPAR, /* ( */
	HIR_TOK_RPAR, /* ) */
	HIR_TOK_LCUR, /* { */
	HIR_TOK_RCUR, /* } */
	HIR_TOK_LBRA, /* [ */
	HIR_TOK_RBRA, /* ] */

	/* Operator */
	HIR_TOK_TID,        /* ~  */
	HIR_TOK_NOT,        /* !  */
	HIR_TOK_INC,        /* ++ */
	HIR_TOK_DEC,        /* -- */
	HIR_TOK_EQ,         /* == */
	HIR_TOK_NEQ,        /* != */
	HIR_TOK_GT,         /* >  */
	HIR_TOK_GEQ,        /* >= */
	HIR_TOK_LEQ,        /* <= */
	HIR_TOK_LT,         /* <  */
	HIR_TOK_LAND,       /* && */
	HIR_TOK_LOR,        /* || */
	HIR_TOK_LSH,        /* << */
	HIR_TOK_RSH,        /* >> */
	HIR_TOK_AND,        /* &  */
	HIR_TOK_OR,         /* |  */
	HIR_TOK_ADD,        /* +  */
	HIR_TOK_SUB,        /* -  */
	HIR_TOK_MUL,        /* *  */
	HIR_TOK_DIV,        /* /  */
	HIR_TOK_MOD,        /* %  */
	HIR_TOK_ASSIGN,     /* =  */
	HIR_TOK_LSH_ASSIGN, /* << */
	HIR_TOK_RSH_ASSIGN, /* >> */
	HIR_TOK_AND_ASSIGN, /* &= */
	HIR_TOK_OR_ASSIGN,  /* |= */
	HIR_TOK_ADD_ASSIGN, /* += */
	HIR_TOK_SUB_ASSIGN, /* -= */
	HIR_TOK_MUL_ASSIGN, /* *= */
	HIR_TOK_DIV_ASSIGN, /* /= */
	HIR_TOK_MOD_ASSIGN, /* %= */

} tok_kind_t;

typedef struct {
	tok_kind_t kind: 8;
	span_t span;

	union {
		char lit_char;
		hir_integer_t lit_integer;
		hir_float_t lit_float;
		vecof(char) lit_string;
		hir_ident_t ident;
		tok_kind_t syntax;
	};
} hir_tok_t;

__api void hir_tok_init(hir_tok_t *token, loc_t start, u16_t length);
__api void hir_tok_dtor(hir_tok_t *token);

typedef struct {
	bool eof;
	source_t *src;
	deqof(hir_tok_t) lookahead;
	vecof(diag_t) *diags;
} hir_lexer_t;

__api void hir_lexer_init(hir_lexer_t *self, source_t *src,
						  vecof(diag_t) *diags);
__api void hir_lexer_dtor(hir_lexer_t *self);
__api hir_tok_t *hir_lexer_peek(hir_lexer_t *self);
__api hir_tok_t *hir_lexer_peekn(hir_lexer_t *self, u8_t n);
__api hir_tok_t *hir_lexer_next(hir_lexer_t *self);

#endif /* !__GUTS_HIR_FE_H */
/*!@} */
