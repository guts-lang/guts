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

#include "il/codemap.h"

#include "test.h"

int main(void)
{
	static char __const *SRC = "\n"
		"(define test 123)\n"
		"(+ test \"\")\n"
		"()\n"
		"\n";
	source_t *src;
	codemap_t cm;
	diag_t err;
	loc_t loc, loc2;

	codemap_init(&cm, NULL);
	codemap_src_push(&cm, SRC, true);
	src = *vecat(cm.sources, 0);

	while (source_next(src));

	loc = source_locate(src, 3, 9);
	diag_error(&err, "Unexpected type in ‘+’ application");
	diag_labelize(&err, true, span(loc, 2), "Expected ‘%s’ but got ‘%s’",
		"integer", "string");
	diag_labelize(&err, false, span(loc, 2), "Expected ‘%s’ but got ‘%s’",
		"integer", "string");
	codemap_diagnostic(&cm, err);
	loc2 = source_locate(src, 3, 1);
	codemap_diagnostic(&cm, diag(
		IL_SEVERITY_WARN, "‘+’ has no effect unless its result is used",
		1, (diag_label_t[]) {
			diag_label(true, span(loc2, 11), NULL),
		}
	));
	codemap_emit(&cm, stdout);
	codemap_destroy(&cm);

	return 0;
}
