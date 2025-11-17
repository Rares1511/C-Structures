# Map Makefile

FILE = map

# Correct path to the source file
SRC = $(FILE).c
OBJ = $(FILE).o
UNITTEST = $(FILE)_unittest.c
LIB = ../lib/lib$(FILE).so

# Compile the object file
$(OBJ): $(SRC)
	$(CC) -c -o $@ $^ $(CFLAGS)

# Create the static library
$(LIB): $(OBJ)
	$(CC) -shared -o $@ $^ $(CFLAGS)

# Install the library
install: $(LIB)
	@echo "Installing Map library..."
	@cp $(LIB) /usr/local/lib

uninstall:
	@echo "Uninstalling Map library..."
	@rm -f /usr/local/lib/lib$(FILE).so

unittest: uninstall install
	$(CC) -o unittest $(UNITTEST) $(CFLAGS) -lmap
	@echo "Running unit tests..."
	@$(TEST_CMD)
	@echo "Unit tests completed."
	@rm -f unittest

# Clean up
clean:
	rm -f $(OBJ) $(LIB) unittest $(UNITTEST_LOG)

.PHONY: install clean