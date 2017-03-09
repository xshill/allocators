#ifndef ALLOCATORS_LIST_ALLOCATOR
#define ALLOCATORS_LIST_ALLOCATOR

#include <stddef.h>
#include <stdint.h>

#define KILOBYTES * 1024
#define MEGABYTES * 1024 KILOBYTES
#define GIGABYTES * 1024 MEGABYTES

typedef struct memory_chunk_list* list_allocator;

list_allocator create_list_allocator(size_t size);

void free_list_allocator(list_allocator allocator);

size_t chunk_count(list_allocator list);

void print_memory(list_allocator list);

void* list_alloc(list_allocator allocator, size_t size);

void* list_free(list_allocator allocator, void* memory);

#endif /* end of include guard: ALLOCATORS_LIST_ALLOCATOR */
