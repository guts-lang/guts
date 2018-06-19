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

/*!@file il/diagnostic.h
 * @author uael
 *
 * @addtogroup il.diagnostic @{
 */
#ifndef __IL_DIAGNOSTIC_H
# define __IL_DIAGNOSTIC_H

#include "il/source.h"

#define LABEL_MAX 256

typedef struct {
	bool primary;
	span_t span;
	char message[LABEL_MAX];
} diag_label_t;

typedef enum {
	IL_SEVERITY_BUG,
	IL_SEVERITY_ERROR,
	IL_SEVERITY_WARN,
	IL_SEVERITY_NOTE,
	IL_SEVERITY_HELP
} severity_t;

__api char *severity_toa(severity_t severity);

typedef struct {
	severity_t severity;
	char message[LABEL_MAX];
	vecof(diag_label_t) labels;
} diag_t;

__api void diag_bug(diag_t *self, char __const *fmt, ...);
__api void diag_error(diag_t *self, char __const *fmt, ...);
__api void diag_warn(diag_t *self, char __const *fmt, ...);
__api void diag_note(diag_t *self, char __const *fmt, ...);
__api void diag_help(diag_t *self, char __const *fmt, ...);
__api void diag_dtor(diag_t *self);
__api void diag_labelize(diag_t *self, bool primary, span_t span,
	char __const *fmt, ...);
__api diag_t diag(severity_t severity, char __const *message,
				  u8_t nlables, diag_label_t *labels);
__api diag_label_t diag_label(bool primary, span_t span,
							  char __const *fmt, ...);

#endif /* !__IL_DIAGNOSTIC_H */
/*!@} */
