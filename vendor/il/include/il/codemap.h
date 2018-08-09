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

/*!@file il/codemap.h
 * @author uael
 *
 * @addtogroup il.codemap @{
 */
#ifndef __IL_CODEMAP_H
# define __IL_CODEMAP_H

#include <stdio.h>

#include "il/source.h"
#include "il/diagnostic.h"

struct codemap;

typedef int (emitter_t)(FILE *stream, struct codemap *fe, diag_t *diag);

typedef struct codemap {
	vecof(source_t *) sources;
	vecof(diag_t) diagnostics;
	emitter_t *emitter;
} codemap_t;

__api emitter_t *IL_DFT_EMITTER;

__api void codemap_init(codemap_t *self, emitter_t *emitter);
__api void codemap_destroy(codemap_t *self);
__api source_t *codemap_src_push(codemap_t *self, char __const *str, bool virt);
__api source_t *codemap_src_find(codemap_t *self, loc_t *loc);
__api void codemap_diagnostic(codemap_t *self, diag_t diag);
__api bool codemap_has_errors(codemap_t *self);
__api int codemap_emit(codemap_t *self, FILE *stream);

#endif /* !__IL_CODEMAP_H */
/*!@} */
