#include <allocators/list_allocator.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct memory_chunk_list
{
    void* start;
    size_t size;
    char used;
    struct memory_chunk_list* previous;
    struct memory_chunk_list* next;
} memory_chunk_list;

memory_chunk_list* create_chunk(void* start, size_t size, char used, memory_chunk_list* previous, memory_chunk_list* next)
{
    memory_chunk_list* chunk_list = malloc(sizeof(memory_chunk_list));
    chunk_list->start = start;
    chunk_list->size = size;
    chunk_list->used = used;
    chunk_list->previous = previous;
    chunk_list->next = next;

    if(previous != NULL)
    {
        previous->next = chunk_list;
    }

    if(next != NULL)
    {
        next->previous = chunk_list;
    }

    return chunk_list;
}

list_allocator create_list_allocator(size_t size)
{
    return create_chunk(malloc(size), size, 0, NULL, NULL);
}

void free_list_allocator(list_allocator allocator)
{
    free(allocator->start);

    memory_chunk_list* list = allocator;

    while(list != NULL)
    {
        memory_chunk_list* next = list->next;
        free(list);
        list = next;
    }
}

size_t chunk_count(list_allocator list)
{
    size_t count = 0;
    while(list != NULL)
    {
        count++;
        list = list->next;
    }

    return count;
}

size_t calculate_print_size(list_allocator list)
{
    size_t print_size = 1;
    const size_t suffix_length = 4;

    while(list != NULL)
    {
        size_t size = list->size;
        size_t no_chars = (size == 0) ? 1 : 0;

        while(size > 0)
        {
            no_chars++;
            size /= 10;
        }

        print_size += no_chars + suffix_length;
        list = list->next;
    }

    return print_size;
}

void print_memory(list_allocator list)
{
    size_t print_size = calculate_print_size(list);
    for(int i = 0 ; i < print_size; i++)
    {
        printf("-");
    }
    printf("\n");

    printf("|");
    while(list != NULL)
    {
        printf("%luB %s|", list->size, list->used ? "U" : "F");
        list = list->next;
    }
    printf("\n");

    for(int i = 0 ; i < print_size; i++)
    {
        printf("-");
    }
    printf("\n");
}

void* list_alloc(list_allocator allocator, size_t size)
{
    if(size == 0)
    {
        return NULL;
    }

    memory_chunk_list* list = allocator;

    while(list != NULL && (list->size < size || list->used))
    {
        list = list->next;
    }
    
    if(list == NULL)
    {
        return NULL;
    }

    void* memory = (void*)((uintptr_t)list->start + list->size - size);

    if(list->size == size)
    {
        list->used = 1;
    }
    else
    {
        list->size -= size;
        create_chunk(memory, size, 1, list, list->next);
    }

    return memory;
}

void combine_chunks(memory_chunk_list* chunk)
{
    while(chunk != NULL && !chunk->used)
    {
        while(chunk->next != NULL && !chunk->next->used && (uintptr_t)chunk->next->start == (uintptr_t)chunk->start + chunk->size)
        {
            chunk->size += chunk->next->size;
            memory_chunk_list* new_next = chunk->next->next;
            free(chunk->next);
            chunk->next = new_next;

            if(new_next != NULL)
            {
                new_next->previous = chunk;
            }
        }

        chunk = chunk->previous;
    }
}

void* list_free(list_allocator allocator, void* memory)
{
    memory_chunk_list* chunk = allocator;
    while(chunk != NULL && chunk->start != memory)
    {
        chunk = chunk->next;
    }

    if(chunk != NULL)
    {
        chunk->used = 0;
        combine_chunks(chunk);
    }
}
