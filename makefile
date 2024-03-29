CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

OBJS = main.o matrix_operations.o memory_allocator.o

my_program: $(OBJS)
	$(CC) $(CFLAGS) -o my_program $(OBJS) $(LDFLAGS)

main.o: main.c matrix_operations.h memory_allocator.h
	$(CC) $(CFLAGS) -c main.c

matrix_operations.o: matrix_operations.c matrix_operations.h
	$(CC) $(CFLAGS) -c matrix_operations.c

memory_allocator.o: memory_allocator.c memory_allocator.h
	$(CC) $(CFLAGS) -c memory_allocator.c

clean:
	rm -f *.o my_program