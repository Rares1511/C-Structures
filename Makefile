CC = gcc
CFLAGS = -Wall -Wextra -lpthread -lm
BFLAGS = -I include
LFLAGS = -Luniversal -luniversal.h

all: map/map_unittest deque/deque_unittest

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

################# VECTOR #################
############ START OF VECTOR ############
vector.o: vector/vector.c
	$(CC) -c -o $@ $< $(CFLAGS)
############# END OF VECTOR #############

################# DEQUE #################
############ START OF DEQUE ############
deque/deque.o: deque/deque.c
	$(CC) -c -o $@ $< $(CFLAGS)
deque/deque_unittest.o: deque/deque_unittest.c
	$(CC) -c -o $@ $< $(CFLAGS)
deque/deque_unittest: deque/deque_unittest.o deque/deque.o universal/universal.o unittest/unittest.o
	$(CC) -o $@ $^ $(CFLAGS)
############# END OF DEQUE #############

################# HASH TABLE #################
############ START OF HASH TABLE ############
hash_table.o: hash_table/hash_table.c
	$(CC) -c -o $@ $< $(CFLAGS)
############# END OF HASH TABLE #############

################# HEAP #################
############ START OF HEAP ############
heap.o: heap/heap.c
	$(CC) -c -o $@ $< $(CFLAGS)
############# END OF HEAP #############

################# LIST #################
############ START OF LIST ############
list.o: list/list.c
	$(CC) -c -o $@ $< $(CFLAGS)
############# END OF LIST #############

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

################# PAIR #################
############ START OF PAIR ############
pair.o: pair/pair.c
	$(CC) -c -o $@ $< $(CFLAGS)
############# END OF PAIR #############

################# QUEUE #################
############ START OF QUEUE ############
queue.o: queue/queue.c
	$(CC) -c -o $@ $< $(CFLAGS)
############# END OF QUEUE #############

################# STACK #################
############ START OF STACK ############
stack.o: stack/stack.c
	$(CC) -c -o $@ $< $(CFLAGS)
############# END OF STACK #############

################# STRING #################
############ START OF STRING ############
string.o: string/string.c
	$(CC) -c -o $@ $< $(CFLAGS)
############# END OF STRING #############

################# LARGE NUMBER #################
############ START OF LARGE NUMBER ############
large_number_internal.o: large_number/large_number_internal.c
	$(CC) -c -o $@ $< $(CFLAGS)
large_number.o: large_number/large_number.c 
	$(CC) -c -o $@ $< $(CFLAGS)
############# END OF LARGE NUMBER #############