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

#include <ir/source.h>
#include "ir/source.h"

#include "test.h"

int main(void)
{
	static char __const *SRC = "Hello world !\nHello world !\nHello world !\n";
	ir_src_t source;
	char c, *line;
	u32_t off, raw, col;

	ASSERT_EQ(0, ir_src_init(&source, SRC, true));
	off = 0;
	raw = 1;
	col = 1;

	while ((c = ir_src_next(&source))) {
		ASSERT_EQ(SRC[off++], c);

		if (c != '\n') ++col;
		else {
			++raw;
			col = 1;
		}

		ASSERT_EQ(raw, source.loc.raw);
		ASSERT_EQ(col, source.loc.col);
		ASSERT_EQ(off, source.loc.off);
	}

	ASSERT_EQ(4, veclen(source.lines));
	line = ir_src_getln(&source, 1);
	ASSERT(line);
	ir_src_dtor(&source);
	return 0;
}
