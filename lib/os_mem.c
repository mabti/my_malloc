/* 
 * This file contains OS calls.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stddef.h>
#include "global_mem.h"

// stops clangd complaining
#ifndef MEM_SIZE 
#define MEM_SIZE 4098
#endif

struct malloc_mem_t g_malloc_mem = { 0 };

static void *request_os_mem(size_t len);
static void free_os_mem(void *ptr, size_t len);

void init_my_malloc() {
	if(g_malloc_mem.mem) // if already allocated
		return;

	// assign fields
	g_malloc_mem.len = MEM_SIZE;
	g_malloc_mem.mem = request_os_mem(MEM_SIZE);

	// assign first block
	g_malloc_mem.mem->len = MEM_SIZE;
	g_malloc_mem.mem->tags = (struct block_tags){ 
		.last = true, 
		.first = true, 
		.occupied = false 
	};
	//TODO: raise error if allocation failed
}

void quit_my_malloc() {
	free_os_mem(g_malloc_mem.mem, g_malloc_mem.len);
	g_malloc_mem.len = 0;
	g_malloc_mem.mem = NULL;
}

// Unix impl
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))

#include <sys/mman.h>

void *request_os_mem(size_t len) {
	return mmap(NULL, 
		len,
		PROT_READ | PROT_WRITE, 
		MAP_ANON | MAP_SHARED,
		-1,
		0);
}

void free_os_mem(void *ptr, size_t len) {
	munmap(ptr, len);
}

#endif

// Windows impl
#ifdef WIN32

#include <windows.h>

// void allocate_global_memory() {
// 	g_malloc_mem.mem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MEM_SIZE);
// }
// 
// void free_global_memory() {
// 	HeapFree(GetProcessHeap(), 0, g_malloc_mem.mem);
// }

void *request_os_mem(size_t len) {
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, len);
}

void free_os_mem(void *ptr, size_t _len) {
	HeapFree(GetProcessHeap(), 0, ptr);
}

#endif


