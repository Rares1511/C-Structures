# Vector Makefile

FILE = vector

# Correct path to the source file
SRC = $(FILE).c
OBJ = $(FILE).o
UNITTEST = $(FILE)_unittest.c
LIB = ../lib/lib$(FILE).so

# Compiler flags
CFLAGS = -Wall -Werror

# Compile the object file
$(OBJ): $(SRC)
	$(CC) -c -o $@ $^ $(CFLAGS)

# Create the static library
$(LIB): $(OBJ)
	$(CC) -shared -o $@ $^ $(CFLAGS)

# Install the library
install: $(LIB)
	@echo "Installing Vector library..."
	@cp $(LIB) /usr/local/lib

unittest: 
	$(CC) -o unittest $(UNITTEST) $(CFLAGS) -lvector
	@echo "Running unit tests..."
	@valgrind --leak-check=full ./unittest
	@echo "Unit tests completed."
	@rm -f unittest

# Clean up
clean:
	rm -f $(OBJ) $(LIB)

.PHONY: install clean