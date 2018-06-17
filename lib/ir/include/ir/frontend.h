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

/*!@file ir/frontend.h
 * @author uael
 *
 * @addtogroup ir.frontend @{
 */
#ifndef __IR_FRONTEND_H
# define __IR_FRONTEND_H

#include <stdio.h>

#include "ir/source.h"
#include "ir/diagnostic.h"

struct ir_fe;

typedef int (ir_emitter_t)(FILE *stream, struct ir_fe *fe, ir_diag_t *diag);

typedef struct ir_fe {
	vecof(ir_src_t) sources;
	vecof(ir_diag_t) diagnostics;
	ir_emitter_t *emitter;
} ir_fe_t;

__api ir_emitter_t *IR_DFT_EMITTER;

__api void ir_fe_init(ir_fe_t *self, ir_emitter_t *emitter);
__api void ir_fe_dtor(ir_fe_t *self);
__api int ir_fe_srcpush(ir_fe_t *self, char __const *str, bool virtual);
__api ir_src_t *ir_fe_srcfind(ir_fe_t *self, ir_loc_t *loc);
__api void ir_fe_diagpush(ir_fe_t *self, ir_diag_t diag);
__api bool ir_fe_hasdiag(ir_fe_t *self);
__api int ir_fe_emit(ir_fe_t *self, FILE *stream);

#endif /* !__IR_FRONTEND_H */
/*!@} */
