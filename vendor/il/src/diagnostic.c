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
#include <il/diagnostic.h>

#include "il/diagnostic.h"

static char *__severity[] = {
	[IL_SEVERITY_BUG] = "bug",
	[IL_SEVERITY_ERROR] = "error",
	[IL_SEVERITY_WARN] = "warning",
	[IL_SEVERITY_NOTE] = "note",
	[IL_SEVERITY_HELP] = "help",
};

FORCEINLINE
char *severity_toa(severity_t severity)
{
	return __severity[severity];
}

void diag_bug(diag_t *self, char __const *format, ...)
{
	va_list ap;

	self->severity = IL_SEVERITY_BUG;
	self->labels = NULL;

	if (format) {
		va_start(ap, format);
		vsnprintf(self->message, LABEL_MAX, format, ap);
		va_end(ap);
	}
}

void diag_error(diag_t *self, char __const *format, ...)
{
	va_list ap;

	self->severity = IL_SEVERITY_ERROR;
	self->labels = NULL;

	if (format) {
		va_start(ap, format);
		vsnprintf(self->message, LABEL_MAX, format, ap);
		va_end(ap);
	}
}

void diag_warn(diag_t *self, char __const *format, ...)
{
	va_list ap;

	self->severity = IL_SEVERITY_WARN;
	self->labels = NULL;

	if (format) {
		va_start(ap, format);
		vsnprintf(self->message, LABEL_MAX, format, ap);
		va_end(ap);
	}
}

void diag_note(diag_t *self, char __const *format, ...)
{
	va_list ap;

	self->severity = IL_SEVERITY_NOTE;
	self->labels = NULL;

	if (format) {
		va_start(ap, format);
		vsnprintf(self->message, LABEL_MAX, format, ap);
		va_end(ap);
	}
}

void diag_help(diag_t *self, char __const *format, ...)
{
	va_list ap;

	self->severity = IL_SEVERITY_HELP;
	self->labels = NULL;

	if (format) {
		va_start(ap, format);
		vsnprintf(self->message, LABEL_MAX, format, ap);
		va_end(ap);
	}
}

FORCEINLINE
void diag_dtor(diag_t *self)
{
	vecdtor(self->labels);
}

void diag_labelize(diag_t *self, bool primary, span_t span,
				   char __const *fmt, ...)
{
	va_list ap;
	diag_label_t label;

	label = (diag_label_t) {
		.span = span,
		.primary = primary,
		.message = { '\0' }
	};
	if (fmt) {
		va_start(ap, fmt);
		vsnprintf(label.message, LABEL_MAX, fmt, ap);
		va_end(ap);
	}
	vecpush(self->labels, label);
}

FORCEINLINE
diag_t diag(severity_t severity, char __const *message,
			u8_t nlables, diag_label_t *labels)
{
	diag_t ret;
	u8_t i;

	ret.severity = severity;
	strcpy(ret.message, message ? message : "");

	ret.labels = NULL;
	for (i = 0; i < nlables; ++i)
		vecpush(ret.labels, labels[i]);

	return ret;
}

diag_label_t diag_label(bool primary, span_t span, char __const *fmt, ...)
{
	va_list ap;
	diag_label_t label;

	label = (diag_label_t) {
		.span = span,
		.primary = primary,
		.message = { '\0' }
	};
	if (fmt) {
		va_start(ap, fmt);
		vsnprintf(label.message, LABEL_MAX, fmt, ap);
		va_end(ap);
	}
	return label;
}
