# Vector Makefile

FILE = vector

# Correct path to the source file
SRC = $(FILE).c
OBJ = $(FILE).o
UNITTEST = $(FILE)_unittest.c
LIB = ../lib/lib$(FILE).a

# Compiler flags
CFLAGS = -Wall -Werror

# Compile the object file
$(OBJ): $(SRC)
	$(CC) -c -o $@ $^ $(CFLAGS) -lcs_global

# Create the static library
$(LIB): $(OBJ)
	$(AR) rcs $@ $^

# Install the library
install: $(LIB)
	sudo install -d $(PATH_INCLUDEDIR)
	sudo install -m 644 $(LOCAL_INCLUDEDIR)/$(FILE).h $(PATH_INCLUDEDIR)
	sudo install -m 644 $^ $(LIBDIR)

unittest: 
	$(CC) -o unittest $(UNITTEST) $(CFLAGS)

# Clean up
clean:
	rm -f $(OBJ) $(LIB) unittest

.PHONY: install clean