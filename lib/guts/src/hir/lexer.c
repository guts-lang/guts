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

#include <ctype.h>
#include <guts.h>

#include "guts/hir/lexer.h"

void hir_lexer_init(hir_lexer_t *self, source_t *src,
					vecof(diag_t) *diags)
{
	self->eof = false;
	self->src = src;
	self->lookahead = NULL;
	self->diags = diags;
}

void hir_lexer_dtor(hir_lexer_t *self)
{
	deqdtor(self->lookahead);
	self->src = NULL;
}

static bool __lookahead(hir_lexer_t *self);

hir_tok_t *hir_lexer_peek(hir_lexer_t *self)
{
	if (deqempty(self->lookahead)) {
		if (!__lookahead(self))
			return NULL;
	}
	return deqbeg(self->lookahead);
}

hir_tok_t *hir_lexer_peekn(hir_lexer_t *self, u8_t n)
{
	if (deqlen(self->lookahead) <= n) {
		while (deqlen(self->lookahead) <= n) {
			if (!__lookahead(self))
				return NULL;
		}
	}
	return deqat(self->lookahead, n);
}

hir_tok_t *hir_lexer_next(hir_lexer_t *self)
{
	if (deqempty(self->lookahead)) {
		if (!__lookahead(self))
			return NULL;
	}
	return dequsht(self->lookahead);
}

hir_tok_t *hir_lexer_consume(hir_lexer_t *self, hir_tok_kind_t kind)
{
	hir_tok_t *tok;

	if (self->eof) return NULL;
	if (deqempty(self->lookahead)) {
		if (!__lookahead(self))
			return NULL;
	}
	tok = dequsht(self->lookahead);
	if (tok->kind != kind && self->diags) {
		diag_t error;

		diag_error(&error, "unexpected token, expected ‘%s’ got ‘%s’",
			hir_tok_toa(kind), hir_tok_toa(tok->kind));
		diag_labelize(&error, true, tok->span, NULL);
		vecpush(*self->diags, error);
		tok = NULL;
	}
	return tok;
}

#define MATCH(k, len, ...) do { \
	hir_tok_init(&tok, start, (u16_t)len); \
	tok.kind = (k); \
	__VA_ARGS__ \
	goto match; \
} while (false)

#define MATCH_SKIP(k, len, ...) do { \
	u32_t __i; \
	hir_tok_init(&tok, start, (u16_t)len); \
	tok.kind = (k); \
	tok.syntax = (k); \
	__VA_ARGS__ \
	for (__i = 0; __i < len; ++__i) \
		source_next(self->src); \
	goto match; \
} while (false)

static bool __lookahead(hir_lexer_t *self)
{
	char c;
	hir_tok_t tok;
	loc_t start;

	if (self->eof)
		return false;
	while (isspace(c = source_peek(self->src)))
		source_next(self->src);
	start = source_loc(self->src);
	switch (c) {
		case '\0': MATCH_SKIP(HIR_TOK_EOF, 1, { self->eof = true; });
		case '"': goto lit_string;
		case '\'': goto lit_char;

		/* Symbol */
		case ':': MATCH_SKIP(HIR_TOK_COLON, 1);
		case ';': MATCH_SKIP(HIR_TOK_SEMICOLON, 1);
		case ',': MATCH_SKIP(HIR_TOK_COMMA, 1);

		/* Delimiter */
		case '(': MATCH_SKIP(HIR_TOK_LPAR, 1);
		case ')': MATCH_SKIP(HIR_TOK_RPAR, 1);
		case '{': MATCH_SKIP(HIR_TOK_LCUR, 1);
		case '}': MATCH_SKIP(HIR_TOK_RCUR, 1);
		case '[': MATCH_SKIP(HIR_TOK_LBRA, 1);
		case ']': MATCH_SKIP(HIR_TOK_RBRA, 1);

		/* Operator */
		case '~': MATCH_SKIP(HIR_TOK_TID, 1);
		case '!': switch (source_peekn(self->src, 1)) {
			case '=': MATCH_SKIP(HIR_TOK_NEQ, 2);
			default: MATCH_SKIP(HIR_TOK_NOT, 1);
		}
		case '?': switch (source_peekn(self->src, 1)) {
			case '=': MATCH_SKIP(HIR_TOK_NIL_ASSIGN, 2);
			default: MATCH_SKIP(HIR_TOK_NIL, 1);
		}
		case '+': switch (source_peekn(self->src, 1)) {
			case '=': MATCH_SKIP(HIR_TOK_ADD_ASSIGN, 2);
			case '+': MATCH_SKIP(HIR_TOK_INC, 2);
			default: MATCH_SKIP(HIR_TOK_ADD, 1);
		}
		case '-': switch (source_peekn(self->src, 1)) {
			case '=': MATCH_SKIP(HIR_TOK_SUB_ASSIGN, 2);
			case '-': MATCH_SKIP(HIR_TOK_DEC, 2);
			default: MATCH_SKIP(HIR_TOK_SUB, 1);
		}
		case '=': switch (source_peekn(self->src, 1)) {
			case '=': MATCH_SKIP(HIR_TOK_EQ, 2);
			default: MATCH_SKIP(HIR_TOK_ASSIGN, 1);
		}
		case '>': switch (source_peekn(self->src, 1)) {
			case '>': switch (source_peekn(self->src, 2)) {
				case '=': MATCH_SKIP(HIR_TOK_RSH_ASSIGN, 3);
				default: MATCH_SKIP(HIR_TOK_GT, 1);
			}
			case '=': MATCH_SKIP(HIR_TOK_GEQ, 2);
			default: MATCH_SKIP(HIR_TOK_GT, 1);
		}
		case '<': switch (source_peekn(self->src, 1)) {
			case '<': switch (source_peekn(self->src, 2)) {
				case '=': MATCH_SKIP(HIR_TOK_LSH_ASSIGN, 3);
				default: MATCH_SKIP(HIR_TOK_LT, 1);
			}
			case '=': MATCH_SKIP(HIR_TOK_LEQ, 2);
			default: MATCH_SKIP(HIR_TOK_LT, 1);
		}
		case '&': switch (source_peekn(self->src, 1)) {
			case '=': MATCH_SKIP(HIR_TOK_AND_ASSIGN, 2);
			case '&': MATCH_SKIP(HIR_TOK_LAND, 2);
			default: MATCH_SKIP(HIR_TOK_AND, 1);
		}
		case '|': switch (source_peekn(self->src, 1)) {
			case '=': MATCH_SKIP(HIR_TOK_OR_ASSIGN, 2);
			case '|': MATCH_SKIP(HIR_TOK_LOR, 2);
			default: MATCH_SKIP(HIR_TOK_OR, 1);
		}
		case '^': switch (source_peekn(self->src, 1)) {
			case '=': MATCH_SKIP(HIR_TOK_XOR_ASSIGN, 2);
			default: MATCH_SKIP(HIR_TOK_XOR, 1);
		}
		case '*': switch (source_peekn(self->src, 1)) {
			case '=': MATCH_SKIP(HIR_TOK_MUL_ASSIGN, 2);
			default: MATCH_SKIP(HIR_TOK_MUL, 1);
		}
		case '/': switch (source_peekn(self->src, 1)) {
			case '=': MATCH_SKIP(HIR_TOK_DIV_ASSIGN, 2);
			default: MATCH_SKIP(HIR_TOK_DIV, 1);
		}
		case '%': switch (source_peekn(self->src, 1)) {
			case '=': MATCH_SKIP(HIR_TOK_MOD_ASSIGN, 2);
			default: MATCH_SKIP(HIR_TOK_MOD, 1);
		}

		default: {
			if (isalpha(c) || c == '_')
				goto ident;
			if (isdigit(c) || (c == '.' && isdigit(source_peekn(self->src, 1))))
				goto number;
			goto unexpected;
		}

		unexpected: {
			if (self->diags) {
				diag_t error;

				diag_error(&error, "unexpected character ‘%c’", c);
				diag_labelize(&error, true, span(start, 1), NULL);
				vecpush(*self->diags, error);
			}
			self->eof = true;
			return false;
		}

		match: {
			deqpush(self->lookahead, tok);
			return true;
		}

		ident: {
			u16_t len;

			tok.ident = source_str(self->src);

			source_next(self->src);
			while (isalnum(c = source_peek(self->src)) || c == '_')
				source_next(self->src);

			//TODO(uael): check OF
			len = (u16_t) (source_loc(self->src).off - start.off);
			switch (len) {
				case 2:
					if (!memcmp("i8", tok.ident, 2))
						MATCH(HIR_TOK_I8, len);
					else if (!memcmp("u8", tok.ident, 2))
						MATCH(HIR_TOK_U8, len);
					else if (!memcmp("if", tok.ident, 2))
						MATCH(HIR_TOK_IF, len);
					else if (!memcmp("as", tok.ident, 2))
						MATCH(HIR_TOK_AS, len);
					else break;
				case 3:
					if (!memcmp("i16", tok.ident, 3))
						MATCH(HIR_TOK_I16, len);
					else if (!memcmp("i32", tok.ident, 3))
						MATCH(HIR_TOK_I32, len);
					else if (!memcmp("i64", tok.ident, 3))
						MATCH(HIR_TOK_I64, len);
					else if (!memcmp("u16", tok.ident, 3))
						MATCH(HIR_TOK_U16, len);
					else if (!memcmp("u32", tok.ident, 3))
						MATCH(HIR_TOK_U32, len);
					else if (!memcmp("u64", tok.ident, 3))
						MATCH(HIR_TOK_U64, len);
					else if (!memcmp("f32", tok.ident, 3))
						MATCH(HIR_TOK_F32, len);
					else if (!memcmp("f34", tok.ident, 3))
						MATCH(HIR_TOK_F64, len);
					else if (!memcmp("use", tok.ident, 3))
						MATCH(HIR_TOK_USE, len);
					else if (!memcmp("let", tok.ident, 3))
						MATCH(HIR_TOK_LET, len);
					else break;
				case 4:
					if (!memcmp("char", tok.ident, 4))
						MATCH(HIR_TOK_CHAR, len);
					else if (!memcmp("bool", tok.ident, 4))
						MATCH(HIR_TOK_BOOL, len);
					else if (!memcmp("else", tok.ident, 4))
						MATCH(HIR_TOK_ELSE, len);
					else if (!memcmp("true", tok.ident, 4))
						MATCH(HIR_TOK_LIT_BOOL, len, { tok.lit_bool = true; });
					else if (!memcmp("null", tok.ident, 4))
						MATCH(HIR_TOK_LIT_NULL, len);
					else break;
				case 5:
					if (!memcmp("false", tok.ident, 5))
						MATCH(HIR_TOK_LIT_BOOL, len, { tok.lit_bool = false; });
					else if (!memcmp("while", tok.ident, 5))
						MATCH(HIR_TOK_WHILE, len);
					else if (!memcmp("break", tok.ident, 5))
						MATCH(HIR_TOK_BREAK, len);
					else if (!memcmp("const", tok.ident, 5))
						MATCH(HIR_TOK_CONST, len);
					else break;
				case 6:
					if (!memcmp("struct", tok.ident, 6))
						MATCH(HIR_TOK_STRUCT, len);
					else if (!memcmp("return", tok.ident, 6))
						MATCH(HIR_TOK_RETURN, len);
					else break;
				case 7:
					if (!memcmp("include", tok.ident, 7))
						MATCH(HIR_TOK_INCLUDE, len);
					else break;
				case 8:
					if (!memcmp("continue", tok.ident, 8))
						MATCH(HIR_TOK_CONTINUE, len);
					else break;
				case 9:
					if (!memcmp("namespace", tok.ident, 9))
						MATCH(HIR_TOK_NAMESPACE, len);
					else break;
				default: break;
			}
			MATCH(HIR_TOK_IDENT, len);
		}

		number: {
			u16_t len;
			char c2;

			tok.lit_number = source_str(self->src);

			if (source_peek(self->src) == '.')
				source_next(self->src);

			while (true) {
				if (isdigit(c = source_peek(self->src)) || c == '.'
					|| c == '_') source_next(self->src);
				else if (isalpha(c)) {
					if (tolower(c) == 'e' &&
						((c2 = source_peekn(self->src, 1)) == '+'
						|| c2 == '-')) {
						source_next(self->src);
					}
					source_next(self->src);
				} else {
					break;
				}
			}
			//TODO(uael): check OF
			len = (u16_t) (source_loc(self->src).off - start.off);
			MATCH(HIR_TOK_LIT_NUMBER, len);
		}

		lit_string: {
			tok.lit_string = NULL;

			//TODO(uael): escape + check
			source_next(self->src);
			while ((c = source_next(self->src)) != '"' && c)
				vecpush(tok.lit_string, c);
			if (c != '"')
				goto unexpected;
			MATCH(HIR_TOK_LIT_STRING, veclen(tok.lit_string) + 2);
		}

		lit_char: {
			source_next(self->src);

			//TODO(uael): escape + check
			if (!isascii(c = source_next(self->src)))
				goto unexpected;
			tok.lit_char = c;
			if (source_next(self->src) != '\'')
				goto unexpected;
			MATCH(HIR_TOK_LIT_CHAR, 3);
		}
	}
}
