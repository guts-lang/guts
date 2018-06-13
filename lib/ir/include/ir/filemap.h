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

/*!@file ir/filemap.h
 * @author uael
 *
 * @addtogroup ir.filemap @{
 */
#ifndef __IR_FILEMAP_H
# define __IR_FILEMAP_H

#include "ir/span.h"
#include "ir/vector.h"

typedef struct {
	bool virtual;
	char __const *filename;
	char __const *src;
	size_t srclen;
	ir_loc_t loc;
	vecof(u32_t) lines;
} ir_filemap_t;

__api int ir_filemap_virtual(ir_filemap_t *self, char __const *src);
__api int ir_filemap_real(ir_filemap_t *self, char __const *filename);
__api void ir_filemap_dtor(ir_filemap_t *self);
__api char ir_filemap_peek(ir_filemap_t *self, u8_t n);
__api char ir_filemap_next(ir_filemap_t *self);
__api char *ir_filemap_readline(ir_filemap_t *self, u32_t line);

#endif /* !__IR_FILEMAP_H */
/*!@} */
