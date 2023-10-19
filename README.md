# Malloc

A simple malloc implementation.

This implementation is **NOT** production ready, and probably has many bugs.
It is also not particularly time efficient but is space efficient.

## Building

This project comes in the form of a static library, a test executable, and meson project.

To build, run:
```sh
meson setup build # replace build with whatever directory you want
meson compile -C build
```

If for some ungodly reason, you want to link `my_malloc.a` with a meson project, you can do the following:
```meson
# place my_malloc in <PROJECT_ROOT>/subprojects
my_malloc_proj = subproject('my_malloc') 
my_malloc_dep = my_malloc_proj.get_variable('my_malloc_dep')

executable('my_program', my_sources, dependencies: my_malloc)
```

## Implementation

This implementation allocates a chunk of memory that it uses for all allocations.
It creates one block. A block contains:
- The length of the data
- If the block is first and/or last.
- If the block is used.

See `struct block` and `struct block_tags` in `lib/global_mem.h` for a definition in C. 

Each block is stored in the same memory as the data.
The data the block stores is 7 bytes (the length of the metadata)
ahead of the metadata. 
This is internally calculated using `sizeof(struct block)` for maintainability.

When `malloc(size_t size)` is called, the first block with at least `size` bytes is found.
This is then split into two blocks: The first block is used for the requested allocation
and the second is used for later.
The first block is marked as used and a pointer to it's data is returned.

`free(void *ptr)` marks the block that contains `ptr` as not used.
`free` also merges at many blocks as it can from the block before the freed one.

### Major limitation

In the current implementation, `malloc` does not expand its memory once it runs out.
This will probably cause a segfault if too much memory is allocated.

Malloc also takes `O(n)` time to run as it has to search through every block until it finds a free one.

In implementations where block metadata is stored seperately to the data, `free` could possibly be implemented
in `O(log(n))`, however this `free` is `O(n)` as it has to find the block before. 
This could be improved to `O(1)` if a second pointer or length was stored in the metadata to the last block, 
but this would decrease space efficiency (taking 17 bytes per block instead of the current 9 bytes, nearly double the space!).
 
