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

#include <ir/filemap.h>
#include "ir/filemap.h"

#include "test.h"

int main(void)
{
	static char __const *SRC = "Hello world !\nHello world !\nHello world !\n";
	ir_filemap_t filemap;
	char c, *line;
	u32_t off, raw, col;

	ASSERT_EQ(0, ir_filemap_virtual(&filemap, SRC));

	off = 0;
	raw = 1;
	col = 0;
	while ((c = ir_filemap_next(&filemap))) {
		ASSERT_EQ(SRC[off++], c);

		if (c != '\n') ++col;
		else {
			++raw;
			col = 0;
		}

		ASSERT_EQ(raw, filemap.loc.raw);
		ASSERT_EQ(col, filemap.loc.col);
		ASSERT_EQ(off, filemap.loc.off);
	}

	ASSERT_EQ(4, veclen(filemap.lines));

	line = ir_filemap_readline(&filemap, 1);
	ASSERT(line);

	ir_filemap_dtor(&filemap);
	return 0;
}
