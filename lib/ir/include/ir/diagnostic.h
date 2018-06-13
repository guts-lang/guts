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

/*!@file ir/diagnostic.h
 * @author uael
 *
 * @addtogroup ir.diagnostic @{
 */
#ifndef __IR_DIAGNOSTIC_H
# define __IR_DIAGNOSTIC_H

#include "ir/filemap.h"

typedef enum {
	IR_LABEL_PRIMARY,
	IR_LABEL_SECONDARY
} ir_label_style_t;

typedef struct {
	ir_label_style_t style;
	ir_span_t span;
	char __const *message;
} ir_label_t;

__api ir_label_t ir_label_primary(ir_span_t span, char __const *message);
__api ir_label_t ir_label_secondary(ir_span_t span, char __const *message);

typedef enum {
	IR_SEVERITY_BUG,
	IR_SEVERITY_ERROR,
	IR_SEVERITY_WARN,
	IR_SEVERITY_NOTE,
	IR_SEVERITY_HELP
} ir_severity_t;

typedef struct {
	ir_severity_t severity;
	char __const *message;
	vecof(ir_label_t) labels;
} ir_diag_t;

__api void ir_diag_bug(ir_diag_t *self, char __const *message);
__api void ir_diag_error(ir_diag_t *self, char __const *message);
__api void ir_diag_warn(ir_diag_t *self, char __const *message);
__api void ir_diag_note(ir_diag_t *self, char __const *message);
__api void ir_diag_help(ir_diag_t *self, char __const *message);
__api void ir_diag_dtor(ir_diag_t *self);
__api void ir_diag_with_label(ir_diag_t *self, ir_label_t label);
__api void ir_diag_with_labels(ir_diag_t *self, ir_label_t *labels, usize_t n);

#endif /* !__IR_DIAGNOSTIC_H */
/*!@} */
