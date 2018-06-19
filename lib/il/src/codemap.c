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

#include <termcolor.h>

#include "il/codemap.h"

FORCEINLINE
void codemap_init(codemap_t *self, emitter_t *emitter)
{
	self->sources = NULL;
	self->diagnostics = NULL;
	self->emitter = emitter ? emitter : IL_DFT_EMITTER;
}

FORCEINLINE
void codemap_dtor(codemap_t *self)
{
	usize_t i;

	for (i = 0; i < veclen(self->sources); ++i)
		source_dtor(vecat(self->sources, i));
	vecdtor(self->sources);
	for (i = 0; i < veclen(self->diagnostics); ++i)
		diag_dtor(vecat(self->diagnostics, i));
	vecdtor(self->diagnostics);
}

FORCEINLINE
source_t *codemap_src_push(codemap_t *self, char __const *str, bool virtual)
{
	source_t src;

	if (source_init(&src, str, virtual))
		return NULL;

	src.loc.src = (u32_t)veclen(self->sources);
	vecpush(self->sources, src);
	return vecback(self->sources);
}

FORCEINLINE
source_t *codemap_src_find(codemap_t *self, loc_t *loc)
{
	if (!loc || loc->src >= veclen(self->sources)) {
		errno = EINVAL;
		return NULL;
	}

	return vecat(self->sources, loc->src);
}

FORCEINLINE
void codemap_diagnostic(codemap_t *self, diag_t diag)
{
	vecpush(self->diagnostics, diag);
}

FORCEINLINE
bool codemap_has_errors(codemap_t *self)
{
	return veclen(self->diagnostics) > 0;
}

FORCEINLINE
int codemap_emit(codemap_t *self, FILE *stream)
{
	usize_t i;
	int st;

	for (i = 0; i < veclen(self->diagnostics); ++i) {
		if ((st = self->emitter(stream, self, vecat(self->diagnostics, i))))
			return st;

		if (i < veclen(self->diagnostics))
			fputc('\n', stream);
	}

	return 0;
}

static int __emit(FILE *s, codemap_t *fe, diag_t *diag);
emitter_t *IL_DFT_EMITTER = __emit;

static int __emit(FILE *s, codemap_t *fe, diag_t *diag)
{
	usize_t i, j;
	diag_label_t *label;
	source_t *src;
	color_t sev_clr;

	switch (diag->severity) {
		case IL_SEVERITY_WARN:
			sev_clr = color(TERMCOLOR_YELLOW);
			break;
		case IL_SEVERITY_NOTE:
			sev_clr = color(TERMCOLOR_CYAN);
			break;
		case IL_SEVERITY_HELP:
			sev_clr = color(TERMCOLOR_GREEN);
			break;
		case IL_SEVERITY_BUG:
		case IL_SEVERITY_ERROR:
		default:
			sev_clr = color(TERMCOLOR_RED);
	}

	termcolor_setfg(s, sev_clr);
	termcolor_setfl(s, TERMCOLOR_BOLD);
	fprintf(s, "%s: ", severity_toa(diag->severity));
	termcolor_reset(s);
	termcolor_setfl(s, TERMCOLOR_BOLD);
	fprintf(s, "%s\n", diag->message);
	termcolor_reset(s);

	for (i = 0; i < veclen(diag->labels); ++i) {
		label = vecat(diag->labels, i);

		if (!(src = codemap_src_find(fe, &label->span.start)))
			fprintf(s, "- %s\n", label->message);
		else {
			u32_t line, column, off, line_off;
			int mark;
			char *linea, *eol, *markb, *marke;
			line = label->span.start.raw;
			column = label->span.start.col;
			color_t mark_color;

			if ((linea = source_getln(src, line))) {
				eol = strchr(linea, '\n');
				if (!eol) eol = strchr(linea, '\r');
				if (!eol) eol = strchr(linea, '\0');

				line_off = source_getoff(src, line);
				markb = linea + (label->span.start.off - line_off);
				marke = markb + label->span.length;

				if (label->primary) {
					mark = '^';
					mark_color = sev_clr;
				} else {
					mark = '-';
					mark_color = color(TERMCOLOR_CYAN);
				}

				fprintf(s, "- %s:%u:%u\n", src->filename, line, column);
				fprintf(s, "%3u | %.*s", line, (int)(markb - linea), linea);
				termcolor_setfg(s, mark_color);
				fprintf(s, "%.*s", (int)(marke - markb), markb);
				termcolor_reset(s);
				fprintf(s, "%.*s\n", (int)(eol - marke), marke);

				fprintf(s, "    | ");
				termcolor_setfg(s, mark_color);

				for (off = line_off; off < label->span.start.off; ++off)
					fputc(' ', s);

				for (j = 0; j < label->span.length; ++j)
					fputc(mark, s);

				if (*label->message)
					fprintf(s, " %s", label->message);
				termcolor_reset(s);

				fputc('\n', s);
			}
		}
	}

	return 0;
}
