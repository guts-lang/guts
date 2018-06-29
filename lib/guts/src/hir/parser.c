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

#include "guts/hir/parser.h"

void hir_parser_init(hir_parser_t *self, codemap_t *codemap,
					 hir_scope_t *root)
{
	u32_t i;
	source_t *source;
	hir_lexer_t lexer;

	self->codemap = codemap;
	self->lexer = NULL;
	self->lexers = NULL;
	self->root = root;
	self->current = root;

	if (root)
		hir_scope_init(root, NULL, NULL);

	for (i = 0; i < veclen(codemap->sources); ++i) {
		source = codemap->sources[i];
		hir_lexer_init(&lexer, source, &codemap->diagnostics);
		stackpush(self->lexers, lexer);
		self->lexer = stackback(self->lexers);
	}
}

void hir_parser_dtor(hir_parser_t *self)
{
	hir_lexer_t *lexer;

	while ((lexer = stackpop(self->lexers)))
		hir_lexer_dtor(lexer);
	stackdtor(self->lexers);
}

int hir_parser_include(hir_parser_t *self, char __const *str, bool virt)
{
	source_t *source;
	hir_lexer_t lexer;

	if (!(source = codemap_src_push(self->codemap, str, virt)))
		return -1;
	hir_lexer_init(&lexer, source, &self->codemap->diagnostics);
	stackpush(self->lexers, lexer);
	self->lexer = stackback(self->lexers);
	return 0;
}

hir_tok_t *hir_parser_peek(hir_parser_t *self)
{
	hir_tok_t *tok;

	if (!self->lexer)
		return NULL;
	if (!(tok = hir_lexer_peek(self->lexer)) || tok->kind == HIR_TOK_EOF) {
		stackpop(self->lexers);
		if (!stacklen(self->lexers))
			return tok;
		self->lexer = stackback(self->lexers);
		return hir_parser_peek(self);
	}
	return tok;
}

hir_tok_t *hir_parser_peekn(hir_parser_t *self, u8_t n)
{
	hir_tok_t *tok;
	u8_t count;
	hir_lexer_t *lexer;

	if (!self->lexer)
		return NULL;
	if (!(tok = hir_lexer_peekn(self->lexer, n)) || tok->kind == HIR_TOK_EOF) {
		if (!stacklen(self->lexers))
			return NULL;
		count = 0;
		lexer = self->lexer;
		do {
			count += (u8_t) (deqlen(lexer->lookahead) ? deqlen(lexer->lookahead) - 1 : 0);
			--lexer;
			tok = hir_lexer_peekn(lexer, n - count);
		} while ((!tok || tok->kind == HIR_TOK_EOF)
			&& lexer > stackbeg(self->lexers));
	}
	return tok;
}

hir_tok_t *hir_parser_next(hir_parser_t *self)
{
	hir_tok_t *tok;

	if (!self->lexer)
		return NULL;
	if (!(tok = hir_lexer_next(self->lexer)) || tok->kind == HIR_TOK_EOF) {
		stackpop(self->lexers);
		if (!stacklen(self->lexers))
			return tok;
		self->lexer = stackback(self->lexers);
		return hir_parser_next(self);
	}
	return tok;
}

hir_tok_t *hir_parser_consume(hir_parser_t *self, hir_tok_kind_t kind)
{
	hir_tok_t *tok;
	u32_t errs;

	if (!self->lexer)
		return NULL;

	errs = veclen(self->codemap->diagnostics);
	if ((tok = hir_parser_next(self)) && tok->kind != kind) {
		if (errs == veclen(self->codemap->diagnostics)) {
			diag_t error;

			diag_error(&error, "unexpected token ‘%s’ expected ‘%s’",
				hir_tok_toa(tok->kind), hir_tok_toa(kind));
			diag_labelize(&error, true, tok->span, NULL);
			vecpush(self->codemap->diagnostics, error);
		}
		tok = NULL;
	}
	return tok;
}

hir_tok_t *hir_parser_any(hir_parser_t *self, char __const *kinds)
{
	hir_tok_t *tok;
	u32_t errs;

	if (!self->lexer)
		return NULL;

	errs = veclen(self->codemap->diagnostics);
	if ((tok = hir_parser_next(self))
		&& !strchr(kinds, tok->kind)) {

		if (errs == veclen(self->codemap->diagnostics)) {
			u8_t i;
			diag_t error;
			char expected[256] = { '\0' };

			i = 0;
			while (*kinds) {
				char __const *kind;

				expected[i++] = '`';
				kind = hir_tok_toa((hir_tok_kind_t)*kinds++);
				while (*kind)
					expected[i++] = *kind++;
				expected[i++] = '\'';

				if (*kinds && *(kinds + 1)) {
					strcpy(expected + i, ", ");
					i += 2;
				} else if (*kinds) {
					strcpy(expected + i, " or ");
					i += 4;
				}
			}

			diag_error(&error, "unexpected token ‘%s’ expected %s",
				hir_tok_toa(tok->kind), expected);
			diag_labelize(&error, true, tok->span, NULL);
			vecpush(self->codemap->diagnostics, error);
		}
		tok = NULL;
	}
	return tok;
}
