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

#include "ir/frontend.h"

#include "test.h"

int main(void)
{
	static char __const *SRC = "\n"
		"(define test 123)\n"
		"(+ test \"\")\n"
		"()\n"
		"\n";
	ir_src_t *src;
	ir_fe_t fe;
	ir_diag_t err, warn;
	ir_loc_t loc;

	ir_fe_init(&fe, NULL);
	ir_fe_srcpush(&fe, SRC, true);
	src = vecat(fe.sources, 0);

	while (ir_src_next(src));

	loc = ir_src_locate(src, 3, 9);
	ir_diag_error(&err, "Unexpected type in `+' application");
	ir_diag_labelpush(&err,
		ir_label_primary(ir_span(loc, 2), "Expected `%s' but got `%s'",
			"integer", "string"));
	ir_diag_labelpush(&err,
		ir_label_secondary(ir_span(loc, 2), "Expected `%s' but got `%s'",
			"integer", "string"));
	ir_fe_diagpush(&fe, err);
	loc = ir_src_locate(src, 3, 1);
	ir_diag_warn(&warn, "`+` function has no effect unless its result is used");
	ir_diag_labelpush(&warn, ir_label_primary(ir_span(loc, 11), NULL));
	ir_fe_diagpush(&fe, warn);
	ir_fe_emit(&fe, stdout);
	ir_fe_dtor(&fe);

	return 0;
}
