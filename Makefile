CC = gcc
CFLAGS = -Wall -Wextra

all: main

run:
	valgrind -s --leak-check=full ./main

clean:
	rm *.o
	rm main

universal.o: universal/universal.c universal/universal.h
	$(CC) -c -o $@ $< $(CFLAGS)
vector.o: vector/vector.c vector/vector.h
	$(CC) -c -o $@ $< $(CFLAGS)
deque.o: deque/deque.c deque/deque.h
	$(CC) -c -o $@ $< $(CFLAGS)
hash_table.o: hash_table/hash_table.c hash_table/hash_table.h
	$(CC) -c -o $@ $< $(CFLAGS)
heap.o: heap/heap.c heap/heap.h
	$(CC) -c -o $@ $< $(CFLAGS)
large_number.o: large_number/large_number.c large_number/large_number.h
	$(CC) -c -o $@ $< $(CFLAGS)
list.o: list/list.c list/list.h
	$(CC) -c -o $@ $< $(CFLAGS)
map.o: map/map_internal.c map/map_internal.h map/map.c map/map.h
	$(CC) -c -o $@ $< $(CFLAGS)
pair.o: pair/pair.c pair/pair.h
	$(CC) -c -o $@ $< $(CFLAGS)
queue.o: queue/queue.c queue/queue.h
	$(CC) -c -o $@ $< $(CFLAGS)
stack.o: stack/stack.c stack/stack.h
	$(CC) -c -o $@ $< $(CFLAGS)
string.o: string/string.c string/string.h
	$(CC) -c -o $@ $< $(CFLAGS)

main.o: main.c
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o universal.o pair.o
	$(CC) -o $@ $^ $(CFLAGS)