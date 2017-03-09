#include <allocators/list_allocator.h>

#include <stdio.h>

int main(int argc, char* argv[])
{
    list_allocator allocator = create_list_allocator(1 GIGABYTES);
    void* mem = list_alloc(allocator, 1 GIGABYTES);
    
    print_memory(allocator);
    list_free(allocator, mem);
    print_memory(allocator);
    free_list_allocator(allocator);

    return 0;
}
