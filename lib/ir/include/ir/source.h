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

/*!@file ir/source.h
 * @author uael
 *
 * @addtogroup ir.source @{
 */
#ifndef __IR_SOURCE_H
# define __IR_SOURCE_H

#include "ir/span.h"
#include "ir/vector.h"

typedef struct {
	bool virtual;
	char __const *filename;
	char __const *src;
	size_t srclen;
	ir_loc_t loc;
	vecof(u32_t) lines;
} ir_src_t;

__api int ir_src_init(ir_src_t *self, char __const *str, bool virtual);
__api void ir_src_dtor(ir_src_t *self);
__api char ir_src_peek(ir_src_t *self, u8_t n);
__api char ir_src_next(ir_src_t *self);
__api u32_t ir_src_getoff(ir_src_t *self, u32_t line);
__api char *ir_src_getln(ir_src_t *self, u32_t line);
__api ir_loc_t ir_src_loc(ir_src_t *self, u32_t line, u32_t col);

#endif /* !__IR_SOURCE_H */
/*!@} */
