CC = gcc
CFLAGS = -Iinclude
DEPS = 
OBJS = build/main.o build/list_allocator.o

bin/allocators: bin build $(OBJS)
	$(CC) -o bin/allocators $(OBJS) $(CFLAGS)

bin:
	mkdir -p bin

build:
	mkdir -p build

build/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm build/*.o
	rm bin/allocators
