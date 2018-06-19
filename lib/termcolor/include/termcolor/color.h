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

/*!@file termcolor/color.h
 * @author uael
 *
 * @addtogroup termcolor @{
 */
#ifndef __TERMCOLOR_COLOR_H
# define __TERMCOLOR_COLOR_H

#include "termcolor/conf.h"

#ifdef OS_WIN
# include <io.h>
#endif

#define TERMCOLOR_BOLD (1 << 0)
#define TERMCOLOR_INTENSE (1 << 1)
#define TERMCOLOR_UNDERLINE (1 << 2)

enum color {
	TERMCOLOR_GREY = 0,
#ifndef OS_WIN
	TERMCOLOR_BLACK,
	TERMCOLOR_BLUE,
	TERMCOLOR_GREEN,
	TERMCOLOR_RED,
	TERMCOLOR_CYAN,
	TERMCOLOR_MAGENTA,
	TERMCOLOR_YELLOW,
	TERMCOLOR_WHITE,
#else
	TERMCOLOR_BLACK,
	TERMCOLOR_BLUE = FOREGROUND_BLUE,
	TERMCOLOR_GREEN = FOREGROUND_GREEN,
	TERMCOLOR_RED = FOREGROUND_RED,
	TERMCOLOR_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
	TERMCOLOR_MAGENTA = FOREGROUND_BLUE | FOREGROUND_RED,
	TERMCOLOR_YELLOW = FOREGROUND_GREEN | FOREGROUND_RED,
	TERMCOLOR_WHITE = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
#endif
};

enum {
	TERMCOLOR_ANSI256 = TERMCOLOR_WHITE + 1,
	TERMCOLOR_RGB,
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

/*!@brief
 * Initialize a standard terminal color on the stack.

 * @param color The standard color (`TERMCOLOR_GREY' to `TERMCOLOR_WHITE')
 * @return      A color.
 */
__api color_t color(enum color color);

/*!@brief
 * Initialize a ANSI 256 terminal color on the stack.
 *
 * @param ansi A 256 ANSI color code,
 * @return     A color.
 */
__api color_t color_ansi256(u8_t ansi);

/*!@brief
 * Initialize a RGB terminal color on the stack.
 *
 * @param r The `r' from rgb
 * @param g The `b' from rgb
 * @param b The `g' from rgb
 * @return  A color.
 */
__api color_t color_rgb(u8_t r, u8_t g, u8_t b);

#endif /* !__TERMCOLOR_COLOR_H */
/*!@} */
