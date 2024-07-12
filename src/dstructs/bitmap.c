#include "../../include/dstructs/bitmap.h"

int bitmap_create(bitmap *bmap, size_t size)
{
	bmap->size = size;
	bool *bits;
	if (!(bmap = malloc(sizeof(bool) * size))) {
		return EXIT_FAILURE;
	}
	bmap->bits = bits;
	return EXIT_SUCCESS;
}

int bitmap_find_first(const bitmap *bmap, bool key)
{
	for (size_t idx = 0; idx < bmap->size; idx ++) {
		if (bmap[idx] == key) return idx;
	}
	return -1;
}

int bitmap_set_bit(bitmap *bmap, size_t idx, bool bit)
{
	if (idx >= bmap->size) {return EXIT_FAILURE;}
	bmap->bits[idx] = bit;
	return EXIT_SUCCESS;
}