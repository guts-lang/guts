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

#include <string.h>

#include "test.h"

#include "ds/map.h"

typedef int (map_eq_t)(u8_t const *a, u8_t const *b);
typedef u32_t (map_hash_t)(u8_t const *key);

#define taken_bit 0x2
#define deleted_bit 0x1

#define pairof(TKey, TVal) struct { \
	TKey key; \
	TVal val; \
}

#define htableof(TItem) struct { \
	TItem *buckets; \
	htable_entry_t *entries; \
	u32_t len, bit; \
	u32_t ksz, psz; \
	map_eq_t *eq; \
	map_hash_t *hash; \
}

#define mapof(TKey, TVal) htableof(pairof(TKey, TVal))
#define setof(TItem) htableof(TItem)

typedef struct {
	u32_t flags: 2;
	u32_t hash: 30;
} htable_entry_t;

/* TODO: store key and value sizeof to avoid limit the key size to uintptr_t. */
typedef struct {
	char *buckets;
	htable_entry_t *entries;
	u32_t len, bit;
	u32_t ksz, psz;
	map_eq_t *eq;
	map_hash_t *hash;
} htable_t;

static int __eq(u8_t const *a, u8_t const *b)
{
	return a == b;
}

static u32_t __hash(u8_t const *key)
{
	return (u32_t)(uintptr_t)key;
}

static void htable_init(htable_t *map, usize_t ksz, usize_t psz,
						map_eq_t *eq, map_hash_t *hash)
{
	bzero(map, sizeof(htable_t));
	map->ksz = (u32_t)ksz;
	map->psz = (u32_t)psz;
	map->eq = eq ? eq : __eq;
	map->hash = hash ? hash : __hash;
}

static void swap_byte(void *a, void *b, size_t count)
{
	char *x = (char *)a;
	char *y = (char *)b;

	while (count--) {
		char t = *x;
		*x = *y;
		*y = t;
		x += 1;
		y += 1;
	}
}

static void swap_word(void *a, void *b, size_t count)
{
	char *x = (char *)a;
	char *y = (char *)b;
	long t[1];

	while (count--) {
		memcpy(t, x, sizeof(long));
		memcpy(x, y, sizeof(long));
		memcpy(y, t, sizeof(long));
		x += sizeof(long);
		y += sizeof(long);
	}
}

static void memswap(void *a, void *b, size_t size)
{
	size_t words = size / sizeof(long);
	size_t bytes = size % sizeof(long);

	swap_word(a, b, words);
	a = (char *)a + words * sizeof(long);
	b = (char *)b + words * sizeof(long);
	swap_byte(a, b, bytes);
}

/* TODO: shrink. */
/* TODO: re hash with a recursive function which mark items as moving and find
 * the new space to store it. Increment the flags field by one bit. */
static void htable_resize(htable_t *map, u32_t bit)
{
	u32_t i, old_max, new_max, new_mask;
	htable_entry_t *new_entries, *entry;

	old_max = map->bit ? 1U << map->bit : 0;

	new_max = 1U << bit;
	new_mask = new_max - 1;

	new_entries = calloc(new_max, sizeof(htable_entry_t));
	
	if (bit > map->bit) {
		map->buckets = realloc(map->buckets, new_max * map->psz);
		bzero(map->buckets + (old_max * map->psz),
			((new_max - old_max) * map->psz));
	}

	for (i = 0; i < old_max; ++i) {
		if (map->entries[i].flags == taken_bit) {

			entry = map->entries + i;
			entry->flags = 0;

			while (true) {
				u32_t j;

				j = entry->hash & new_mask;
				while (new_entries[j].flags)
					j = (j + 1) & new_mask;

				new_entries[j].flags = taken_bit;
				new_entries[j].hash = entry->hash;

				if (i == j)
					break;
				printf("move %u from %u to %u\n", entry->hash, i, j);
				if (j < old_max && map->entries[j].flags == taken_bit) {
					memswap(map->buckets + (i * map->psz),
						map->buckets + (j * map->psz), map->psz);
					entry = map->entries + j;
					entry->flags = 0;
				} else {
					memcpy(map->buckets + (j * map->psz),
						map->buckets + (i * map->psz), map->psz);
					break;
				}
			}
		}
	}

	free(map->entries);

	if (bit < map->bit)
		map->buckets = realloc(map->buckets, new_max * map->psz);
	
	map->bit = bit;
	map->entries = new_entries;
}

static u32_t htable_put(htable_t *map, u8_t *key)
{
	htable_entry_t *entry;
	u32_t i, hash, mask;
	u8_t *pair_key;

	if (!map->bit) /* initialize */
		htable_resize(map, 4);
	else if (map->len >= (1U << map->bit) * 0.55) /* grow */
		htable_resize(map, map->bit + 1);

	hash = map->hash(key) << 2;
	mask = (1U << map->bit) - 1;
	i = hash & mask;

	pair_key = alloca(map->ksz);
	while (true) {

		memcpy(pair_key, map->buckets + (i * map->psz), map->ksz);
		entry = map->entries + i;

		if (!entry->flags || (entry->flags == deleted_bit)
			|| map->eq(*(u8_t **)pair_key, key)) {

			entry->flags = taken_bit;
			entry->hash = hash;

			printf("put %u to %u\n", hash, i);
			
			memcpy(map->buckets + (i * map->psz), &key, map->ksz);

			++map->len;
			return i;
		}

		i = (i + 1) & mask;
	}
}

static u32_t htable_get(htable_t *map, u8_t *key)
{
	htable_entry_t *entry;
	u32_t i, hash, last, mask;
	u8_t *pair_key;

	hash = map->hash(key) << 2;
	mask = (1U << map->bit) - 1;
	i = hash & mask;
	last = i;

	pair_key = alloca(map->ksz);
	while (true) {
		memcpy(pair_key, map->buckets + (i * map->psz), map->ksz);
		entry = map->entries + i;

		/* TODO: check for hash eq. */
		if (entry->flags == taken_bit
			&& map->eq(*(u8_t **)pair_key, key))
			return i;
		if (!entry->flags || (i = (i + 1) & mask) == last)
			return U32_MAX;
	}
}

static bool htable_del(htable_t *map, u8_t *key)
{
	u32_t i;

	if ((i = htable_get(map, key)) != U32_MAX) {
		htable_entry_t *entry;

		entry = map->entries + i;
		entry->flags = deleted_bit;
		--map->len;

		printf("del %u at %u\n", entry->hash, i);

		/* shrink */
		if (map->len <= (1U << map->bit) * 0.15 && map->bit > 4)
			htable_resize(map, map->bit - 1);

		return true;
	}

	return false;
}

static bool htable_contains(htable_t *map, u8_t *key)
{
	return (bool)(htable_get(map, key) != U32_MAX);
}

static int __streq(u8_t const *a, u8_t const *b)
{
	return !strcmp((const char *) a, (const char *) b);
}

static u32_t __strhash(u8_t const *key)
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

static mapof(char *, u32_t) __map;

#define mapinit(M, EQ, HASH) htable_init( \
	(htable_t *)(M), sizeof((M)->buckets->key), sizeof(*(M)->buckets), \
	EQ, HASH \
)
#define mapput(M, K, V) do { \
	usize_t __idx = htable_put((htable_t *)(M), (u8_t *)(K)); \
	(M)->buckets[__idx].val = (V); \
} while (false)
#define maphas(M, K) htable_contains((htable_t *)(M), (u8_t *)(K))
#define mapget(M, K) htable_get((htable_t *)(M), (u8_t *)(K))
#define mapdel(M, K) htable_del((htable_t *)(M), (u8_t *)(K))

#define setinit(M, EQ, HASH) htable_init( \
	(htable_t *)(M), sizeof(*(M)->buckets), sizeof(*(M)->buckets), \
	EQ, HASH \
)
#define setput(M, K) htable_put((htable_t *)(M), (u8_t *)(K))
#define sethas(M, K) htable_contains((htable_t *)(M), (u8_t *)(K))
#define setdel(M, K) htable_del((htable_t *)(M), (u8_t *)(K))

int main(void)
{
	u32_t i, idx;

	mapinit(&__map, __streq, __strhash);

	for (i = 0; i < KEYS; ++i)
		mapput(&__map, __keys[i], i);

	for (i = 0; i < KEYS; ++i) {
		ASSERT_TRUE(maphas(&__map, __keys[i]));

		idx = mapget(&__map, __keys[i]);
		ASSERT_NEQ(U32_MAX, idx);
		ASSERT_EQ(i, __map.buckets[idx].val);
		ASSERT_STREQ(__map.buckets[idx].key, __keys[i]);
		/*printf("check %s %u %u\n", __keys[i], __strhash(__keys[i]) << 2,
			__map.entries[idx].hash);
		ASSERT_EQ(__strhash(__keys[i]) << 2, __map.entries[idx].hash);*/

		ASSERT_TRUE(mapdel(&__map, __keys[i]));
		ASSERT_FALSE(maphas(&__map, __keys[i]));
		ASSERT_FALSE(mapdel(&__map, __keys[i]));
	}

	free(__map.buckets);
	free(__map.entries);
	return 0;
}
