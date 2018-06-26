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

#include "ds/htable.h"

#define TAKEN_ENTRY 0x1
#define DELETED_ENTRY 0x2
#define GROW_FACTOR (0.66)
#define SHRINK_FACTOR (0.21)

FORCEINLINE
static u8_t **__entry(htable_t *self, u32_t idx)
{
	return (u8_t **)(self->entries + (idx * self->esz));
}

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
	for (i = 0; i < old_max; ++i) {
		if (self->spans[i].flags & TAKEN_ENTRY) {

			span = self->spans + i;
			span->flags = 0;

			while (true) {
				u32_t j;

				j = span->hash & new_mask;
				while (new_spans[j].flags)
					j = (j + 1) & new_mask;

				new_spans[j].flags = TAKEN_ENTRY;
				new_spans[j].hash = span->hash;

				if (i == j)
					break;
				if (j < old_max && self->spans[j].flags & TAKEN_ENTRY) {
					memswap(__entry(self, j), __entry(self, i), self->esz);
					span = self->spans + j;
					span->flags = 0;
				} else {
					memcpy(__entry(self, j), __entry(self, i), self->esz);
					break;
				}
			}
		}
	}

	free(self->spans);

	/* shrink */
	if (bit < self->bit)
		self->entries = realloc(self->entries, new_max * self->esz);

	self->bit = bit;
	self->spans = new_spans;
}

FORCEINLINE
void htable_init(htable_t *self, usize_t ksz, usize_t esz,
				 eq_fn_t *eq, hash_fn_t *hash)
{
	bzero(self, sizeof(htable_t));
	self->ksz = (u32_t)ksz;
	self->esz = (u32_t)esz;
	self->eq = eq ? eq : eq_u32;
	self->hash = hash ? hash : hash_u32;
}

FORCEINLINE
void htable_dtor(htable_t *self)
{
	free(self->entries);
	free(self->spans);
}

u32_t htable_put(htable_t *self, u8_t const *key)
{
	htable_span_t *span, s;
	u32_t i, mask;

	if (!self->bit) /* initialize */
		__resize(self, 4);
	else if (self->len >= (1U << self->bit) * GROW_FACTOR) /* grow */
		__resize(self, self->bit + 1);

	s.hash = self->hash(key) << 2;
	mask = (1U << self->bit) - 1;
	i = s.hash & mask;

	while (true) {
		u8_t **entry;

		span = self->spans + i;
		entry = __entry(self, i);

		if (!span->flags || (span->flags & DELETED_ENTRY)
			|| (s.hash == span->hash && self->eq(*entry, key))) {

			span->flags = TAKEN_ENTRY;
			span->hash = s.hash;

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

	if (!self->len)
		return U32_MAX;

	s.hash = self->hash(key) << 2;
	mask = (1U << self->bit) - 1;
	i = s.hash & mask;
	last = i;

	while (true) {
		span = self->spans + i;

		if (span->flags & TAKEN_ENTRY && s.hash == span->hash
			&& self->eq(*__entry(self, i), key))
			return i;

		if (!span->flags || (i = (i + 1) & mask) == last)
			return U32_MAX;
	}
}

FORCEINLINE
bool htable_has(htable_t *self, u8_t const *key)
{
	return (bool)(htable_get(self, key) != U32_MAX);
}

FORCEINLINE
bool htable_del(htable_t *self, u8_t const *key)
{
	u32_t i;

	if ((i = htable_get(self, key)) != U32_MAX) {

		self->spans[i].flags = DELETED_ENTRY;
		--self->len;

		/* shrink */
		if (self->len <= (1U << self->bit) * SHRINK_FACTOR && self->bit > 4)
			__resize(self, self->bit - 1);

		return true;
	}

	return false;
}

FORCEINLINE
void htable_iter_init(htable_iter_t *self, htable_t *htable)
{
	self->idx = 0;
	self->item = NULL;
	self->table = htable;
}

FORCEINLINE
bool htable_iter_hasnext(htable_iter_t *self)
{
	const u32_t max = 1U << self->table->bit;

	for (; self->idx < max; ++self->idx) {
		if (self->table->spans[self->idx].flags & TAKEN_ENTRY) {
			self->item = (u8_t *) __entry(self->table, self->idx);
			return true;
		}
	}
	return false;

}

FORCEINLINE
void htable_iter_next(htable_iter_t *self)
{
	++self->idx;
}
