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

/*!@file termc/color.h
 * @author uael
 *
 * @addtogroup termc @{
 */
#ifndef __TERMC_COLOR_H
# define __TERMC_COLOR_H

#include "termc/conf.h"

#define TERMC_BOLD (1 << 0)
#define TERMC_INTENSE (1 << 1)
#define TERMC_UNDERLINE (1 << 2)

enum color {
	TERMC_NONE,
	TERMC_BLACK,
	TERMC_BLUE,
	TERMC_GREEN,
	TERMC_RED,
	TERMC_CYAN,
	TERMC_MAGENTA,
	TERMC_YELLOW,
	TERMC_WHITE,
};

enum {
	TERMC_ANSI256 = TERMC_WHITE + 1,
	TERMC_RGB,
};

typedef union {
	u8_t kind;

	struct {
		u8_t __kind;
		u8_t c;
	} ansi256;

	struct {
		u8_t __kind;
		u8_t r, g, b;
	} rgb;
} color_t;

typedef struct {
	color_t fg;
	color_t bg;
	u8_t flags;
} color_spec_t;

__api color_t color(enum color color);
__api color_t color_ansi256(u8_t ansi);
__api color_t color_rgb(u8_t r, u8_t g, u8_t b);

#endif /* !__TERMC_COLOR_H */
/*!@} */
