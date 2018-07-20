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

#include "guts/hir/generic.h"
#include "guts/hir/type.h"

bool hir_generic_parse(hir_generic_t *self, hir_parser_t *parser)
{
	hir_tok_t *tok;
	hir_ty_t ty;

	bzero(self, sizeof(hir_generic_t));
	if ((tok = hir_parser_peek(parser))->kind != HIR_TOK_IDENT)
		return false;

	hir_parser_next(parser);
	self->span = tok->span;
	hir_name_init(&self->name, tok);

	if (hir_parser_peek(parser)->kind == HIR_TOK_COLON) {
		hir_parser_next(parser);
		hir_ty_consume(&ty, parser);
		self->clause = memdup(&ty, sizeof(hir_ty_t));
		self->span.length = span_diff(tok->span, self->span);
	}

	return true;
}

FORCEINLINE
bool hir_generic_consume(hir_generic_t *self, hir_parser_t *parser)
{
	return hir_parser_required((void *) hir_generic_parse,
		self, parser, "generic");
}

FORCEINLINE
void hir_generic_destroy(hir_generic_t *self)
{
	if (!self)
		return;
	hir_ty_destroy(self->clause);
}

bool hir_template_parse(hir_template_t *self, hir_parser_t *parser)
{
	hir_tok_t *tok;
	hir_generic_t generic;

	bzero(self, sizeof(hir_template_t));
	if ((tok = hir_parser_peek(parser))->kind != '<')
		return false;

	hir_parser_next(parser);
	self->span = tok->span;

	do {
		if (!hir_generic_consume(&generic, parser))
			return false;

		vecpush(self->elems, generic);
		tok = hir_parser_any(parser, (u8_t[]) { ',', '>', 0 });
	} while (tok->kind == ',');

	return true;
}

bool hir_template_consume(hir_template_t *self, hir_parser_t *parser)
{
	return hir_parser_required((void *) hir_template_parse,
		self, parser, "template");
}

FORCEINLINE
void hir_template_destroy(hir_template_t *self)
{
	u32_t i;

	if (!self)
		return;
	for (i = 0; i < veclen(self->elems); ++i)
		hir_generic_destroy(self->elems + i);
	vecdtor(self->elems);
}

bool hir_sym_parse(hir_sym_t *self, hir_parser_t *parser)
{
	hir_tok_t *tok;

	bzero(self, sizeof(hir_sym_t));
	if ((tok = hir_parser_peek(parser))->kind != HIR_TOK_IDENT)
		return false;

	hir_parser_next(parser);
	self->span = tok->span;
	hir_name_init(&self->name, tok);

	if (hir_parser_peek(parser)->kind == '<') {
		hir_ty_t ty;

		hir_parser_next(parser);

		do {
			hir_ty_consume(&ty, parser);
			vecpush(self->apply, ty);
			tok = hir_parser_any(parser, (u8_t[]) { ',', '>', 0 });
		} while (tok->kind == ',');

		self->span.length = span_diff(tok->span, self->span);
	}

	return true;
}

bool hir_sym_consume(hir_sym_t *self, hir_parser_t *parser)
{
	return hir_parser_required((void *) hir_sym_parse,
		self, parser, "symbol");
}

FORCEINLINE
void hir_sym_destroy(hir_sym_t *self)
{
	u32_t i;

	if (!self)
		return;
	for (i = 0; i < veclen(self->apply); ++i)
		hir_ty_destroy(self->apply + i);
	vecdtor(self->apply);
}

bool hir_path_parse(hir_path_t *self, hir_parser_t *parser)
{
	hir_sym_t sym;

	bzero(self, sizeof(hir_path_t));
	if (!hir_sym_parse(&sym, parser))
		return false;

	self->span = sym.span;
	vecpush(self->elems, sym);

	while (hir_parser_peek(parser)->kind == HIR_TOK_DCOLON) {
		hir_parser_next(parser);
		if (!hir_sym_consume(&sym, parser))
			return false;

		vecpush(self->elems, sym);
	}

	return true;
}

void hir_path_consume(hir_path_t *self, hir_parser_t *parser)
{
	hir_parser_required((void *) hir_path_parse,
		self, parser, "symbols path");
}

FORCEINLINE
void hir_path_destroy(hir_path_t *self)
{
	u32_t i;

	if (!self)
		return;
	for (i = 0; i < veclen(self->elems); ++i)
		hir_sym_destroy(self->elems + i);
	vecdtor(self->elems);
}
