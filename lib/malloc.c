/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <my_malloc/malloc.h>
#include <stdbool.h>
#include <stdint.h> // used for byte-sized pointer arithmetic
#include "global_mem.h"

/**
 * Gets the block after this one.
 * Returns `NULL` if block is the last block.
 */
static struct block *next_block(struct block *block) {
	if(block == NULL)
		return NULL;
	if(block->tags.last)
		return NULL;

	// offsetof accounts for metadata. 
	// Should be 9 bytes if I aligned the fields properly
	return block + 1;
}

static struct block *prev_block(struct block *block) {
	if(block == NULL)
		return NULL;
	if(block->tags.first)
		return NULL;

	return block - 1;
}


// static void combine_free_blocks_after(struct block *block) {
// 	struct block *current = block;
// 	struct block *next = next_block(current);
// 
// 	while(next) {
// 		if(next->tags.occupied)
// 			return;
// 		// this is done to account for the case when prev block is occupied
// 		if(current->tags.occupied)
// 			continue;
// 
// 
// 		current->len += next->len + sizeof(struct block);
// 		next = next_block(current);
// 	}
// }

static void insert_block(size_t index, struct block block) {
	struct block replaced;
	replaced = block;
	
	for(size_t i = index; i <= g_malloc_mem.blocks.len; i++) {
		struct block val = g_malloc_mem.blocks.blocks[i];
		g_malloc_mem.blocks.blocks[i] = replaced;
		replaced = val;
	}
}

/**
 * Splits a block into two with the second one having a length of `len`, 
 * and the first having the rest of the memory.
 */
static struct block *split_block(struct block *block, size_t len) {
	// if(block->len == len) 
	// 	return block;

	// block->len -= (len + sizeof(struct block));

	// bool was_last = block->tags.last;
	// block->tags.last = false;

	// struct block *n_block = next_block(block);
	// *n_block = (struct block){
	// 	.len = len,
	// 	.tags = {.last = was_last, .first = false, .occupied = false},
	// };

	// return n_block;

	if(block->len == len)
		return block;

	g_malloc_mem.blocks.len++;
	block->len -= len;
	
	struct block new_block = {
		.len = len,
		.tags = {
			.first = false,
			.last = block->tags.last,
			.occupied = false
		},
		.data = block->data + block->len
	};

	size_t idx = (block - g_malloc_mem.blocks.blocks) + 1;
	insert_block(idx, new_block);
	return &g_malloc_mem.blocks.blocks[idx];
}

/**
 * Returns the first block out of `g_malloc_mem` that has a size of `len` or greater.
 */
static struct block *get_first_block_of_size(size_t len) {
	for(size_t i = 0; i < g_malloc_mem.blocks.len; i++) {
		struct block *block = &g_malloc_mem.blocks.blocks[i];
		if(block->len >= len && !block->tags.occupied) 
			return block;
	}

	return NULL;
}

/**
 * Gets a pointer to the block from a pointer to the start of the data.
 */
static struct block *block_from_ptr(void *ptr) {
	for(size_t i = 0; i < g_malloc_mem.blocks.len; i++) {
		struct block *block = &g_malloc_mem.blocks.blocks[i];

		if(block->data == ptr)
			return block;
	}

	return NULL;
}

void *my_malloc(size_t len) {
	struct block *block = get_first_block_of_size(len);
	if(!block)
		return NULL;

	struct block *data_block = split_block(block, len);
	
	return &data_block->data;
}

void my_free(void *ptr) {
	struct block *block = block_from_ptr(ptr);
	if(!block) // TODO: error handling
		return; 

	block->tags.occupied = false;

	// struct block *prev = prev_block(block);
	// if(prev == NULL)
	// 	combine_free_blocks_after(block);
	// else
	// 	combine_free_blocks_after(prev);
}

