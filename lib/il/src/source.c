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

#include <stdio.h>
#include <il/source.h>
#include <il/loc.h>

#include "il/source.h"

static FORCEINLINE
int __src_virtual(source_t *self, char __const *src)
{
	if (!src) {
		errno = EINVAL;
		return -1;
	}

	self->virtual = true;
	self->filename = "<test>";
	self->lines = NULL;
	self->src = src;
	self->srclen = strlen(src);
	il_loc_init(&self->loc);
	vecpush(self->lines, 0);
	return 0;
}

static FORCEINLINE
int __src_real(source_t *self, char __const *filename)
{
	FILE *fp;
	char *src;
	long sz;

	if (!filename) {
		errno = EINVAL;
		return -1;
	}

	if (!(fp = fopen(filename, "r")) || fseek(fp, 0, SEEK_END) ||
		(sz = ftell(fp)) < 0)
		return -1;

	rewind(fp);
	src = (char *)malloc((size_t)(sz + 1));
	fread(src, 1, (usize_t)sz, fp);
	fclose(fp);

	if (ferror(fp))
		return -1;

	self->virtual = true;
	self->filename = filename;
	self->lines = NULL;
	self->src = src;
	self->srclen = (size_t)sz;
	il_loc_init(&self->loc);
	vecpush(self->lines, 0);
	return 0;
}

int source_init(source_t *self, char __const *str, bool virtual)
{
	if (virtual)
		return __src_virtual(self, str);

	return __src_real(self, str);
}

FORCEINLINE
void source_dtor(source_t *self)
{
	if (!self->virtual)
		free((void *)self->src);

	vecdtor(self->lines);
}

FORCEINLINE
char source_peek(source_t *self)
{
	if (self->loc.off >= self->srclen)
		return '\0';

	return self->src[self->loc.off];
}

FORCEINLINE
char source_peekn(source_t *self, u8_t n)
{
	if (self->loc.off + n >= self->srclen)
		return '\0';

	return self->src[self->loc.off + n];
}

FORCEINLINE
char source_next(source_t *self)
{
	char c;

	if (self->loc.off >= self->srclen)
		return '\0';

	c = self->src[self->loc.off];
	il_loc_shift(&self->loc, c, &self->lines);
	return c;
}

FORCEINLINE
char *source_str(source_t *self)
{
	return (char *)(self->src + self->loc.off);
}

FORCEINLINE
u32_t source_getoff(source_t *self, u32_t line)
{
	if (line < 1 || line > veclen(self->lines)) {
		errno = EINVAL;
		return 0;
	}

	return *vecat(self->lines, line - 1);
}

FORCEINLINE
char *source_getln(source_t *self, u32_t line)
{
	u32_t off;

	if (line < 1 || line > veclen(self->lines)) {
		errno = EINVAL;
		return NULL;
	}

	off = *vecat(self->lines, line - 1);
	return (char *)(self->src + off);
}

loc_t source_locate(source_t *self, u32_t line, u32_t col)
{
	u32_t off;

	if (!line) line = 1;
	if (!col) col = 1;
	off = source_getoff(self, line);
	return (loc_t) {
		.off = off + col - 1,
		.src = self->loc.src,
		.raw = line,
		.col = col,
	};
}

FORCEINLINE
loc_t source_loc(source_t *self)
{
	return self->loc;
}
