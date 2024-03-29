CC=gcc
CFLAGS=-I.
DEPS = memory_allocator.h
OBJ = main.o memory_allocator.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o main