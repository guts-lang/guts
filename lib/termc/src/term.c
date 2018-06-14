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

#include "termc/term.h"

#define ANSI_INTENSE_FG(N) "\x1B[38;5;" STRINGIFY(N) "m"
#define ANSI_INTENSE_BG(N) "\x1B[48;5;" STRINGIFY(N) "m"
#define ANSI_INTENSE(N) ANSI_INTENSE_FG(N), ANSI_INTENSE_BG(N)
#define ANSI_NORMAL_FG(N) "\x1B[3" STRINGIFY(N) "m"
#define ANSI_NORMAL_BG(N) "\x1B[4" STRINGIFY(N) "m"
#define ANSI_NORMAL(N) ANSI_NORMAL_FG(N), ANSI_NORMAL_BG(N)
#define ANSI_ESCAPE(I, N) { ANSI_INTENSE(I) }, { ANSI_NORMAL(N) }

static char __const *__ansi[][2][2] = {
	[TERMC_BLACK] = { ANSI_ESCAPE(8, 0) },
	[TERMC_BLUE] = { ANSI_ESCAPE(12, 4) },
	[TERMC_GREEN] = { ANSI_ESCAPE(10, 2) },
	[TERMC_RED] = { ANSI_ESCAPE(9, 1) },
	[TERMC_CYAN] = { ANSI_ESCAPE(14, 6) },
	[TERMC_MAGENTA] = { ANSI_ESCAPE(13, 5) },
	[TERMC_YELLOW] = { ANSI_ESCAPE(11, 3) },
	[TERMC_WHITE] = { ANSI_ESCAPE(15, 7) },
};

FORCEINLINE
static void __wransi(FILE *stream, color_t c, bool bg)
{
	(void)c;
	if (bg)
		fprintf(stream, "TODO");
	else
		fprintf(stream, "TODO2");
}

FORCEINLINE
static void __wrrgb(FILE *stream, color_t c, bool bg)
{
	(void)c;
	if (bg)
		fprintf(stream, "TODO");
	else
		fprintf(stream, "TODO2");
}

FORCEINLINE
static void __wrcolor(FILE *stream, color_t c, bool intense, bool bg)
{
	(void)stream;
	if (c.kind == TERMC_ANSI256)
		return __wransi(stream, c, bg);
	if (c.kind == TERMC_RGB)
		return __wrrgb(stream, c, bg);
	fprintf(stream, __ansi[c.kind][intense][bg]);
}

FORCEINLINE
FILE *termc_set(FILE *stream, color_spec_t spec)
{
	termc_reset(stream);
	if (spec.flags & TERMC_BOLD)
		fprintf(stream, "\x1B[1m");
	if (spec.flags & TERMC_UNDERLINE)
		fprintf(stream, "\x1B[4m");
	if (spec.fg.kind)
		__wrcolor(stream, spec.fg, (bool)(spec.flags & TERMC_INTENSE), 0);
	if (spec.bg.kind)
		__wrcolor(stream, spec.bg, (bool)(spec.flags & TERMC_INTENSE), 1);
	return stream;
}

FORCEINLINE
FILE *termc_reset(FILE *stream)
{
	fprintf(stream, "\x1B[0m");
	return stream;
}
