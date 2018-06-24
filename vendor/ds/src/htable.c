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
#include "ds/htable.h"

FORCEINLINE
u8_t **__entry(htable_t *self, u32_t idx)
{
	return self->entries + (idx * self->esz);
}

#define taken_bit 0x2
#define deleted_bit 0x1

static void __resize(htable_t *self, u32_t bit)
{
	u32_t i, old_max, new_max, new_mask;
	htable_span_t *new_spans, *span;

	old_max = self->bit ? 1U << self->bit : 0;
	new_max = 1U << bit;
	new_mask = new_max - 1;
	new_spans = calloc(new_max, sizeof(htable_span_t));

	/* grow */
	if (bit > self->bit) {
		self->entries = realloc(self->entries, new_max * self->esz);
		bzero(self->entries + (old_max * self->esz),
			((new_max - old_max) * self->esz));
	}

	/* rehash */
	printf("before rehash\n");
	for (i = 0; i < old_max; ++i) {
		if (self->spans[i].flags & taken_bit) {

			span = self->spans + i;
			span->flags = 0;

			while (true) {
				u32_t j;

				j = span->hash & new_mask;
				while (new_spans[j].flags)
					j = (j + 1) & new_mask;

				new_spans[j].flags = taken_bit;
				new_spans[j].hash = span->hash;

				if (i == j)
					break;
				if (j < old_max && self->spans[j].flags & taken_bit) {
					printf("memswap a\n");
					memswap(__entry(self, i), __entry(self, j), self->esz);
					printf("memswap b\n");
					span = self->spans + j;
					span->flags = 0;
				} else {
					printf("memcpy a %u/%u\n", j, new_max);
					memcpy(__entry(self, j), __entry(self, i), self->esz);
					printf("memcpy b\n");
					break;
				}
			}
		}
	}

	printf("after rehash\n");
	free(self->spans);

	/* shrink */
	if (bit < self->bit)
		self->entries = realloc(self->entries, new_max * self->esz);

	self->bit = bit;
	self->spans = new_spans;
}

void htable_init(htable_t *self, usize_t ksz, usize_t esz,
	hash_fn_t *hash, eq_fn_t *eq)
{
	bzero(self, sizeof(htable_t));
	self->ksz = (u32_t)ksz;
	self->esz = (u32_t)esz;
	self->eq = eq ? eq : eq_u32;
	self->hash = hash ? hash : hash_u32;
	printf("init %llu %llu\n", ksz, esz);
}

u32_t htable_put(htable_t *self, u8_t const *key)
{
	htable_span_t *span, s;
	u32_t i, mask;

	if (!self->bit) /* initialize */
		__resize(self, 4);
	else if (self->len >= (1U << self->bit) * 0.55) /* grow */
		__resize(self, self->bit + 1);

	s.hash = self->hash(key) << 2;
	mask = (1U << self->bit) - 1;
	i = s.hash & mask;

	while (true) {
		u8_t **entry;

		span = self->spans + i;
		entry = __entry(self, i);

		if (!span->flags || (span->flags & deleted_bit)
			|| (s.hash == span->hash && self->eq(*entry, key))) {

			span->flags = taken_bit;
			span->hash = s.hash;

			printf("put %s to %u\n", key, i);

			memcpy(entry, &key, self->ksz);

			++self->len;
			return i;
		}

		i = (i + 1) & mask;
	}
}

u32_t htable_get(htable_t *self, u8_t const *key)
{
	htable_span_t *span, s;
	u32_t i, last, mask;

	s.hash = self->hash(key) << 2;
	mask = (1U << self->bit) - 1;
	i = s.hash & mask;
	last = i;

	while (true) {
		span = self->spans + i;

		if (span->flags & taken_bit && s.hash == span->hash
			&& self->eq(*__entry(self, i), key))
			return i;

		if (!span->flags || (i = (i + 1) & mask) == last)
			return U32_MAX;
	}
}

bool htable_has(htable_t *self, u8_t const *key)
{
	return (bool)(htable_get(self, key) != U32_MAX);
}

bool htable_del(htable_t *self, u8_t const *key)
{
	u32_t i;

	if ((i = htable_get(self, key)) != U32_MAX) {

		self->spans[i].flags = deleted_bit;
		--self->len;

		/* shrink */
		if (self->len <= (1U << self->bit) * 0.15 && self->bit > 4)
			__resize(self, self->bit - 1);

		return true;
	}

	return false;
}
