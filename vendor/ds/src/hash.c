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

#include "ds/hash.h"

FORCEINLINE PURE
u32_t hash_u32(u8_t const *key)
{
	return (u32_t)(uintptr_t)key;
}

FORCEINLINE PURE
u32_t hash_u64(u8_t const *key)
{
	const u64_t k = (u64_t)(uintptr_t)key;

	return (u32_t)(k >> 33 ^ k << 11);
}

FORCEINLINE
u32_t hash_str(u8_t const *key)
{
	const char *s = (const char *) key;
	u32_t h;

	if ((h = (u32_t) *s))
		for (++s; *s; ++s)
			h = (h << 5) - h + (u32_t) *s;
	return h;
}

FORCEINLINE
u32_t hash_strn(u8_t const *key, u32_t n)
{
	const char *s = (const char *) key;
	u32_t h;

	if ((h = (u32_t) *s))
		for (++s; *s && (u32_t)(s -  (const char *) key) < n; ++s)
			h = (h << 5) - h + (u32_t) *s;
	return h;
}

FORCEINLINE PURE
bool eq_u32(u8_t const *a, u8_t const *b)
{
	return (bool)(a == b);
}

FORCEINLINE PURE
bool eq_u64(u8_t const *a, u8_t const *b)
{
	return (bool)(a == b);
}

FORCEINLINE
bool eq_str(u8_t const *a, u8_t const *b)
{
	return (bool)(!strcmp((const char *)a, (const char *)b));
}
