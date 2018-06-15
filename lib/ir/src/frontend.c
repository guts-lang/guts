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

#include <ir/diagnostic.h>
#include <ir/span.h>
#include <ir/source.h>
#include <ir/loc.h>
#include "ir/frontend.h"

FORCEINLINE
void ir_fe_init(ir_fe_t *self, ir_emitter_t *emitter)
{
	self->sources = NULL;
	self->diagnostics = NULL;
	self->emitter = emitter ? emitter : IR_DFT_EMITTER;
}

FORCEINLINE
int ir_fe_srcpush(ir_fe_t *self, char __const *str, bool virtual)
{
	int st;
	ir_src_t src;

	if ((st = ir_src_init(&src, str, virtual)))
		return st;
	src.loc.src = (u32_t)veclen(self->sources);
	vecpush(self->sources, src);
	return 0;
}

FORCEINLINE
ir_src_t *ir_fe_srcfind(ir_fe_t *self, ir_loc_t *loc)
{
	if (!loc || loc->src >= veclen(self->sources)) {
		errno = EINVAL;
		return NULL;
	}
	return vecat(self->sources, loc->src);
}

FORCEINLINE
void ir_fe_diagpush(ir_fe_t *self, ir_diag_t diag)
{
	vecpush(self->diagnostics, diag);
}

FORCEINLINE
bool ir_fe_hasdiag(ir_fe_t *self)
{
	return veclen(self->diagnostics) > 0;
}

FORCEINLINE
int ir_fe_emit(ir_fe_t *self, FILE *stream)
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

static int __emit(FILE *s, ir_fe_t *fe, ir_diag_t *diag);
ir_emitter_t *IR_DFT_EMITTER = __emit;

static int __emit(FILE *s, ir_fe_t *fe, ir_diag_t *diag)
{
	usize_t i, j;
	ir_label_t *label;
	ir_src_t *src;

	fprintf(s, "%s: %s\n", ir_severity_toa(diag->severity), diag->message);

	for (i = 0; i < veclen(diag->labels); ++i) {
		label = vecat(diag->labels, i);
		if (!(src = ir_fe_srcfind(fe, &label->span.start))) {
			fprintf(s, "- "CLR_BOLD"%s"CLR_RESET"\n", label->message);
		} else {
			u32_t line, column, off;
			int mark;
			char *linea, *eol;

			line = label->span.start.raw;
			column = label->span.start.col;
			if ((linea = ir_src_getl(src, line))) {
				eol = strchr(linea, '\n');
				if (!eol) eol = strchr(linea, '\r');
				if (!eol) eol = strchr(linea, '\0');

				fprintf(s, "- %s:%u:%u\n", src->filename, line, column);
				fprintf(s, "%3u | %.*s\n", line, (int)(eol - linea), linea);

				mark = label->style == IR_LABEL_PRIMARY ? '^' : '-';

				fprintf(s, "    | ");
				for (off = ir_src_getoff(src, line);
					off < label->span.start.off; ++off)
					fputc(' ', s);
				for (j = 0; j < label->span.length; ++j)
					fputc(mark, s);
				if (label->message)
					fprintf(s, " %s", label->message);
				fputc('\n', s);
			}
		}
	}
	return 0;
}