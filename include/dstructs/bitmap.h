#pragma once 

#include <stdlib.h>

typedef struct bitmap_ {
	bool 	*bits;
	size_t 	size;
} bitmap;

int bitmap_create(bitmap *bmap, size_t size);
void bitmap_destroy(bitmap *bmap);
int bitmap_find_first(const bitmap *bmap, bool key);
int bitmap_set_bit(bitmap *bmap, size_t idx, bool bit);