CC = gcc
CFLAGS = -Wall -Wextra -lpthread
BFLAGS = -I include
LFLAGS = -Luniversal -luniversal.h

all: map/map_unittest

run:
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(arg)/$(arg)_unittest

clean:
	rm -r */*.o
	rm -r */*_unittest

################# UNIVERSAL #################
############ START OF UNIVERSAL ############
universal/universal.o: universal/universal.c
	$(CC) -c -o $@ $< $(CFLAGS)
unittest/unittest.o: unittest/unittest.c
	$(CC) -c -o $@ $< $(CFLAGS)
############# END OF UNIVERSAL #############

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

################# MAP #################
############ START OF MAP ############
map/map.o: map/map.c map/map_internal.c
	$(CC) -c -o $@ $< $(CFLAGS)
map/map_internal.o: map/map_internal.c
	$(CC) -c -o $@ $< $(CFLAGS)
map/map_unittest.o: map/map_unittest.c
	$(CC) -c -o $@ $< $(CFLAGS)
map/map_unittest: map/map_unittest.o map/map.o map/map_internal.o universal/universal.o unittest/unittest.o
	$(CC) -o $@ $^ $(CFLAGS)
############# END OF MAP #############

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