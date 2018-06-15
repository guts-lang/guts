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

#include "ir/diagnostic.h"

FORCEINLINE
ir_label_t ir_label_primary(ir_span_t span, char __const *message)
{
    return (ir_label_t) {
        .span = span,
        .message = message,
        .style = IR_LABEL_PRIMARY
    };
}

FORCEINLINE
ir_label_t ir_label_secondary(ir_span_t span, char __const *message)
{
    return (ir_label_t) {
        .span = span,
        .message = message,
        .style = IR_LABEL_SECONDARY
    };
}

static char *__severity[] = {
    [IR_SEVERITY_BUG] = CLR_BOLD CLR_RED"bug"CLR_RESET,
    [IR_SEVERITY_ERROR] = CLR_BOLD CLR_RED"error"CLR_RESET,
    [IR_SEVERITY_WARN] = CLR_BOLD CLR_YELLOW"warning"CLR_RESET,
    [IR_SEVERITY_NOTE] = CLR_BOLD CLR_CYAN"note"CLR_RESET,
    [IR_SEVERITY_HELP] = CLR_BOLD CLR_GREEN"help"CLR_RESET,
};

FORCEINLINE
char *ir_severity_toa(ir_severity_t severity)
{
    return __severity[severity];
}

FORCEINLINE
void ir_diag_bug(ir_diag_t *self, char __const *message)
{
    self->severity = IR_SEVERITY_BUG;
    self->message = message;
    self->labels = NULL;
}

FORCEINLINE
void ir_diag_error(ir_diag_t *self, char __const *message)
{
    self->severity = IR_SEVERITY_ERROR;
    self->message = message;
    self->labels = NULL;
}

FORCEINLINE
void ir_diag_warn(ir_diag_t *self, char __const *message)
{
    self->severity = IR_SEVERITY_WARN;
    self->message = message;
    self->labels = NULL;
}

FORCEINLINE
void ir_diag_note(ir_diag_t *self, char __const *message)
{
    self->severity = IR_SEVERITY_NOTE;
    self->message = message;
    self->labels = NULL;
}

FORCEINLINE
void ir_diag_help(ir_diag_t *self, char __const *message)
{
    self->severity = IR_SEVERITY_HELP;
    self->message = message;
    self->labels = NULL;
}

FORCEINLINE
void ir_diag_dtor(ir_diag_t *self)
{
    vecdtor(self->labels);
}

FORCEINLINE
void ir_diag_labelpush(ir_diag_t *self, ir_label_t label)
{
    vecpush(self->labels, label);
}

FORCEINLINE
void ir_diag_labelnpush(ir_diag_t *self, ir_label_t *labels, usize_t n)
{
    vecnpush(self, labels, n);
}
