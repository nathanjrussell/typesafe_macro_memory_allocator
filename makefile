CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

OBJS = main.o matrix_operations.o memory_allocator.o

random_matrices: $(OBJS)
	$(CC) $(CFLAGS) -o random_matrices $(OBJS) $(LDFLAGS)

random_matrices_malloc: CFLAGS += -DDISABLE_MYMALLOC
random_matrices_malloc: random_matrices

main.o: main.c matrix_operations.h memory_allocator.h
	$(CC) $(CFLAGS) -c main.c

matrix_operations.o: matrix_operations.c matrix_operations.h
	$(CC) $(CFLAGS) -c matrix_operations.c

memory_allocator.o: memory_allocator.c memory_allocator.h
	$(CC) $(CFLAGS) -c memory_allocator.c

clean:
	rm -f *.o random_matrices

test: clean random_matrices
	./random_matrices

test_malloc: clean random_matrices_malloc
	./random_matrices