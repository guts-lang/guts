/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
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

/*!@file nt/cpu.h
 * @author uael
 */
#ifndef __NT_CPU_H
# define __NT_CPU_H

#if defined __LP64__ || defined __64BIT__ || defined _LP64 \
  || defined __x86_64 || defined __x86_64__ || defined __amd64 \
  || defined __amd64__ || defined __arm64 || defined __arm64__ \
  || defined __sparc64__ || defined __PPC64__ || defined __ppc64__ \
  || defined __powerpc64__ || defined _M_X64 || defined _M_AMD64 \
  || defined _M_IA64 || (defined __WORDSIZE && __WORDSIZE == 64)
# define CPU_SIZE (64)
# define CPU_BYTE (8)
# define CPU_ALIGN (7)
# define CPU_32 (0)
# define CPU_64 (1)
# define CPU_SHIFT (6)
#else
# define CPU_SIZE (32)
# define CPU_BYTE (4)
# define CPU_ALIGN (3)
# define CPU_32 (1)
# define CPU_64 (0)
# define CPU_SHIFT (5)
#endif

#endif /* !__NT_CPU_H */
