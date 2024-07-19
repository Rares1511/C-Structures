CC = gcc
CFLAGS = -Wall -Wextra
BFLAGS = -I include
LFLAGS = -Luniversal -luniversal.h

all: main

run:
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./main

clean:
	rm *.o
	rm main

universal.o: universal/universal.c
	$(CC) -c -o $@ $< $(CFLAGS)
vector.o: vector/vector.c
	$(CC) -c -o $@ $< $(CFLAGS)
deque.o: deque/deque.c
	$(CC) -c -o $@ $< $(CFLAGS)
hash_table.o: hash_table/hash_table.c
	$(CC) -c -o $@ $< $(CFLAGS)
heap.o: heap/heap.c
	$(CC) -c -o $@ $< $(CFLAGS)
list.o: list/list.c
	$(CC) -c -o $@ $< $(CFLAGS)
map.o: map/map.c
	$(CC) -c -o $@ $< $(CFLAGS)
pair.o: pair/pair.c
	$(CC) -c -o $@ $< $(CFLAGS)
queue.o: queue/queue.c
	$(CC) -c -o $@ $< $(CFLAGS)
stack.o: stack/stack.c
	$(CC) -c -o $@ $< $(CFLAGS)
string.o: string/string.c
	$(CC) -c -o $@ $< $(CFLAGS)
large_number_internal.o: large_number/large_number_internal.c
	$(CC) -c -o $@ $< $(CFLAGS)
large_number.o: large_number/large_number.c 
	$(CC) -c -o $@ $< $(CFLAGS)
main.o: main.c
	$(CC) -c -o $@ $< $(CFLAGS)

zip:
	zip CS.zip deque/* hash_table/* heap/* include/* list/* map/* pair/* queue/* stack/* string/* universal/* vector/* large_number/*

main: main.o universal.o large_number.o large_number_internal.o
	$(CC) -o $@ $^ $(CFLAGS)