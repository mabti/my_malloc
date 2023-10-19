#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Metadata for a block.
 */
struct block_tags {
	/**
	 * Is this block occupied?
	 */
	bool occupied : 1;
	/**
	 * Is this the last block?
	 */
	bool last : 1;
	/**
	 * Is this the first block?
	 */
	bool first : 1;
};

/**
 * A block of memory.
 */
struct block {
	/**
	 * The length (in bytes) of `data`.
	 * Excludes metadata.
	 */
	size_t len;
	/**
	 * The tags for a block.
	 *
	 * \see block_tags
	 */
	struct block_tags tags;
	/**
	 * The data for the block.
	 */
	uint8_t *data;
};

struct malloc_blocks {
	size_t len, capacity;
	struct block *blocks;
};

struct malloc_mem_t {
	size_t len;
	struct malloc_blocks blocks;
	uint8_t *mem;
};

/**
 * The global memory for malloc/free
 */
extern struct malloc_mem_t g_malloc_mem;



