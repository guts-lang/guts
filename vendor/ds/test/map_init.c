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

#include "test.h"

#include "ds/map.h"

typedef int (map_eq_t)(uintptr_t a, uintptr_t b);
typedef u32_t (map_hash_t)(uintptr_t key);

#define taken_bit 0x2
#define deleted_bit 0x1

#define entryof(TVal) struct { \
	u32_t flags: 2; \
	u32_t hash: 30; \
	uintptr_t key; \
	TVal val; \
}

#define mapof(TVal) struct { \
	entryof(TVal) *buckets; \
	u32_t len, bit; \
	u64_t esz; \
	map_eq_t *eq; \
	map_hash_t *hash; \
}

/* TODO: store hash and flags in a separate buffer ? */
typedef struct {
	u32_t flags: 2; \
	u32_t hash: 30; \
	uintptr_t key;
} entry_t;

/* TODO: store key and value sizeof to avoid limit the key size to uintptr_t. */
typedef struct {
	void *buckets;
	u32_t len, bit;
	u64_t esz;
	map_eq_t *eq;
	map_hash_t *hash;
} htable_t;

FORCEINLINE CONST
static int __eq(uintptr_t a, uintptr_t b)
{
	return a == b;
}

FORCEINLINE CONST
static u32_t __hash(uintptr_t key)
{
	return (u32_t)key;
}

static void htable_init(htable_t *map, usize_t esz, map_eq_t *eq, map_hash_t *hash)
{
	assert(esz >= sizeof(entry_t));

	bzero(map, sizeof(htable_t));
	map->esz = esz;
	map->eq = eq ? eq : __eq;
	map->hash = hash ? hash : __hash;
}

/* TODO: shrink. */
/* TODO: re hash with a recursive function which mark items as moving and find
 * the new space to store it. Increment the flags field by one bit. */
static void htable_resize(htable_t *map, u32_t bit)
{
	u32_t i, new_max, old_max, mask;

	old_max = 1U << map->bit;

	map->bit = bit;
	new_max = 1U << bit;
	mask = new_max - 1;
	map->buckets = realloc(map->buckets, new_max * map->esz);

	for (i = 0; i < old_max; ++i) {
		entry_t *entry, *next;
		u32_t j;

		entry = (entry_t *) ((char *)map->buckets + (i * map->esz));

		/* Deleted bucket are reused. */
		if (entry->flags == deleted_bit) entry->flags = 0;
		else if (entry->flags && (entry->hash & mask) != i) {
			entry->flags = 0;
			j = entry->hash & mask;

			/* Find a new entry. */
			while (true) {
				next = (entry_t *) ((char *)map->buckets + (j * map->esz));
				if (next->flags == 0 || (next->flags == deleted_bit))
					break;

				j = (j + 1) & mask;
			}

			/* Copy entry if we find a better space to store it. */
			if (j == i) next = entry;
			else memcpy(next, entry, map->esz);

			/* Mark new entry has taken. */
			next->flags = taken_bit;
		}
	}
}

static u32_t htable_put(htable_t *map, uintptr_t key)
{
	entry_t *entry;
	u32_t i, hash, mask;

	if (!map->bit) /* initialize */
		htable_resize(map, 4);
	else if (map->len >= (1U << map->bit) * 0.55) /* grow */
		htable_resize(map, map->bit + 1);

	hash = map->hash(key) << 2;
	mask = (1U << map->bit) - 1;
	i = hash & mask;

	while (true) {
		entry = (entry_t *) ((char *)map->buckets + (i * map->esz));

		if (!entry->flags || (entry->flags == deleted_bit)
			|| map->eq(entry->key, key)) {

			entry->flags = taken_bit;
			entry->key = key;
			entry->hash = hash;

			++map->len;
			return i;
		}

		i = (i + 1) & mask;
	}
}

static u32_t htable_get(htable_t *map, uintptr_t key)
{
	entry_t *entry;
	u32_t idx, hash, last, mask;

	hash = map->hash(key) << 2;
	mask = (1U << map->bit) - 1;
	idx = hash & mask;
	last = idx;

	while (true) {
		entry = (entry_t *) ((char *)map->buckets + (idx * map->esz));

		if (entry->flags == taken_bit && map->eq(entry->key, key))
			return idx;
		if (!entry->flags || (idx = (idx + 1) & mask) == last)
			return U32_MAX;
	}
}

static bool htable_delat(htable_t *map, u32_t idx)
{
	if (map->bit && idx < (1U << map->bit)) {
		entry_t *entry;

		entry = (entry_t *) ((char *)map->buckets + (idx * map->esz));

		if (entry->flags != taken_bit)
			return false;

		entry->flags = deleted_bit;
		--map->len;

		/* shrink */
		if (map->len <= (1U << map->bit) * 0.15 && map->bit > 4)
			htable_resize(map, map->bit - 1);

		return true;
	}

	return false;
}

static bool htable_del(htable_t *map, uintptr_t key)
{
	u32_t idx;

	if ((idx = htable_get(map, key)) != U32_MAX) {
		entry_t *entry;

		entry = (entry_t *) ((char *)map->buckets + (idx * map->esz));
		entry->flags = deleted_bit;
		--map->len;

		/* shrink */
		if (map->len <= (1U << map->bit) * 0.15 && map->bit > 4)
			htable_resize(map, map->bit - 1);

		return true;
	}

	return false;
}

static bool htable_contains(htable_t *map, uintptr_t key)
{
	return htable_get(map, key) != U32_MAX;
}

static int __streq(uintptr_t a, uintptr_t b)
{
	return !strcmp((const char *) a, (const char *) b);
}

static u32_t __strhash(uintptr_t key)
{
	const char *s = (const char *) key;
	u32_t h = (u32_t) *s;

	if (h) for (++s; *s; ++s) h = (h << 5) - h + (u32_t) *s;
	return h;
}

#define KEYS 250

static __const char *__keys[KEYS] = {
	"4WKiZpGsSR", "Gx7rBxvSAB", "VUFZavMcyd", "i0hDzoGg6j", "dzbwIJOw9p",
	"EmGRiKvDrY", "7Y89C5z4ik", "v4j0EXmYOT", "ZOI2L60FNf", "9OPbbzzG1s",
	"5CqtkWIg2s", "lwXJhzfGnq", "0qX0ElLWif", "gjzwpVf0m6", "ChAYAGfZDh",
	"SWtdXDWY5U", "MSRQt9vIyE", "LFcYVhhwWf", "yIo4JFUg4n", "rLU0JgR28k",
	"r7feJrxLd2", "l9Nt2EpdLj", "9SkUvOgkSB", "O3dYbADHFn", "DSbWmqVEO7",
	"V4YCOOqsGA", "wK6UKXrdXL", "NLY984NofN", "wZFpicO5Hm", "Sfj0qurqEu",
	"5I3WXRPuA7", "SmCiTHP5Lh", "HHR9Jbc0Ic", "psmXPLxsQy", "fC6ph1ynIM",
	"Bnb7tCGWmz", "OmXY4g3k61", "hEs8Y3mhMa", "oRcKJIWF50", "SPyjJ7i710",
	"iP6nBtinqd", "SndhK1oA3j", "DboAfh8PPr", "HGVuC5hf6h", "GHzETGt2R5",
	"aa4ydaZA7A", "8Wh3ozehsy", "fGJyCoVdUU", "0GlXt79itu", "VbeHQHOhqH",
	"tpsw9csIxi", "wRGgP0dehj", "WH5nwJ7UVp", "fKEaRF83UN", "ZSa9jQ5LPM",
	"JWfUGWLTlZ", "O09dtFZ7jn", "ugEkfcymGC", "GqEVYI1Wpb", "Plt6ATkaa3",
	"DzrG3qf0mA", "Pkxv34Gpya", "EUCyMHYACu", "C2FZp8NTgb", "pUt3oB2pVO",
	"UC16SRksuI", "VEZiDANoEI", "cE6BZih00w", "FR873jVDzQ", "uyYw1QZ78j",
	"oxPjTZzFbq", "aqMJUzGp6y", "D0pd2MRYsU", "hgp8EzdzKn", "ikGtpmN5v1",
	"CHEFSw6fPv", "qbwFfVGOxL", "3WvA4JahWa", "Kmo39UaYAO", "iWVi3uy6IY",
	"0S0j3xay2g", "ac2yZbBlus", "Os8pkXiGQH", "lf98OLvNSK", "whnyCaY0Oc",
	"nQx6JDmT0Q", "zEowKaGmGk", "IxLszQ8A88", "ZmxissfL9F", "HSy66tgxbm",
	"WjSxBYb2uT", "sPFxGYQuXu", "XWS4DiyZ7H", "hJCl0Ea9hj", "TcpDmX0Shl",
	"ADJyZl7Ulc", "3S0FJLIUy5", "IvWr3BHuGU", "bpgL9HH6hk", "IwB638CjQi",
	"KHFPvkpQf0", "sIvyvvphGJ", "PafhbalzCf", "dC1CElfwPn", "TjIzLzfzw0",
	"qNkShXiS5l", "XEc2NNzlJG", "lKbF2vQcmT", "ljOZVy2MIC", "hKXRLNTh0K",
	"ewn4eFqFiy", "Y9GWFcZLl7", "FR5YuRr9dm", "RVFSlNYxjb", "RK9ESZj9NN",
	"e971kZLuFH", "G5i7SED9Cl", "G2khyovJIz", "dv7mS4wqG0", "auZxksRS39",
	"UmHxuTy5iC", "ljAop2HuPm", "xJGLm4scaZ", "QVbtDgSdwH", "OcYJ3bwegB",
	"HADZoMxAPb", "E7zOmc0wGR", "0E260ZPFcP", "QAjjTed00I", "KBuCFxEbDp",
	"fQs6XfBXoa", "4qzNyO8KBi", "HR6dQIAPaj", "Rfz0GoLcr2", "B0rltLdglk",
	"ljSPG4T7wI", "J2QXwHEvc8", "Qq4kACK2v4", "pZNt8YRMK4", "a1tqpxvk00",
	"sOxJdREzsN", "Dz3YBqPPfT", "ZaOnQqiLew", "B15e4ZxOCf", "9GMZtZtKKr",
	"SWiHX5b3am", "ROQmZ368Yp", "YIVwHiLJux", "2AQsIOrbmO", "SDMHn7325c",
	"olxDumrR58", "cDi1ATcBDi", "wdq3Yw2mmN", "54UnlmnZ81", "rINgopUYeq",
	"qrgdppyTN8", "8X1A38XshI", "6BPcOeJSHw", "abQqrDZauT", "OefbdAmRHX",
	"6awtOFXMQE", "VJK1jo8MIo", "stPoxGHsWY", "CkEKhGqUfc", "vRFefAOEst",
	"FnVAcnOVjb", "Vay08KuzPQ", "99D37x2glw", "P2GVc5mSLH", "NALKoAoLAj",
	"jH4X249zPu", "hifOOaKrD1", "7LcAA6NeSN", "Bjfe7dhOKD", "8NsPNAGgMW",
	"RnYc8Kb92B", "u6P786mlqW", "UML8A1Eicf", "jVpzJVsDIN", "adu1iKXOn4",
	"L4izr9kk6k", "bvSbuo4CAR", "qgLkQLiPtj", "8YG30RhFIJ", "jKlsL7BBUx",
	"2W6GI3W35o", "R33MmSYavZ", "OoRqGQgTsJ", "Q7L8WL42VB", "OBcZ3zvUUg",
	"qb67Jy2rfY", "hmMHwftAea", "HM85v2iybH", "IFV4bSfmSh", "62l6Kgrusg",
	"CSO0ky8L5P", "WQN2hNlMD7", "X24w3XQWeX", "riCaLWZwV5", "4gepCAldFK",
	"iq8g5aqDOI", "1Iy6vgNc3Q", "zge0jR5M5Z", "qP5n1p0RTg", "7vV9ZoYsv1",
	"t9AUyNc6jp", "SpXuX3NNwc", "OCDLybuY72", "GtAqfSKJgD", "kSxYyYBJeM",
	"23fJwnjdcK", "wReY0lCNUy", "tqBkodQbpj", "zCIN0Ro2ZD", "TGSX5zm0V7",
	"TawigGI7AS", "WdfLOJ9S3W", "10iHJpFnIN", "8TMRRwnHzm", "zafkVQ2Y8a",
	"Un7sqdv7je", "pqwAnW68UO", "HYPQpJyZkJ", "Vnvr2UnjFV", "XiX4m9IGBS",
	"pESY5D3oS0", "6lfBUew5H9", "KkizcyiXIb", "XkKKKW1AQG", "ASgxySxdD8",
	"zv1dAy8ZeC", "48fCdCIoay", "IGogFwsHL6", "bq8NUfchXi", "0Eoa5ENi0q",
	"VACRXwyDQE", "KGVwoVCSbS", "g06crMmW6O", "hA2cpH5lRR", "VNg0QTjsh7",
	"29NC8a0hEe", "IH6dhwKnur", "3UKqoQLqpN", "DMND2E2KyE", "yWK0W0bDPI",
	"lQBpXSNdCr", "tyecI9yCnQ", "Q5TiFHHfvn", "lxP6DeavWK", "WXfjpRcX0t",
};

int main(void)
{
	mapof(u32_t) map;
	u32_t i, idx;

	htable_init((htable_t *)&map, sizeof(*map.buckets), __streq, __strhash);

	for (i = 0; i < KEYS; ++i) {
		idx = htable_put((htable_t *)&map, (uintptr_t)__keys[i]);
		map.buckets[idx].val = i;
	}

	for (i = 0; i < KEYS; ++i) {
		ASSERT_TRUE(htable_contains((htable_t *)&map, (uintptr_t)__keys[i]));

		idx = htable_get((htable_t *)&map, (uintptr_t)__keys[i]);
		ASSERT_NEQ(U32_MAX, idx);
		ASSERT_EQ(i, map.buckets[idx].val);
	}

	free(map.buckets);
	return 0;
}
