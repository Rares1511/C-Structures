FILE = cs_global

LIB_NAME = ../lib/lib$(FILE).a
SRC = $(FILE).c
OBJ = $(SRC:.c=.o)

# Default target to create the library
all: $(LIB_NAME)

# Compile cs_global.c into an object file
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Create static library
$(LIB_NAME): $(OBJ)
	$(AR) rcs $@ $^

# Ensure gcc can find the library automatically
install: $(LIB_NAME)
	cp $(LIB_NAME) $(LIBDIR)
	ldconfig

uninstall:
	rm -f $(LIBDIR)/lib$(FILE).a
	ldconfig

unittest:
	$(CC) $(CFLAGS) -o $(FILE)_unittest $(FILE)_unittest.c -lcs_global
	./$(FILE)_unittest

# Clean build files
clean:
	rm -f $(OBJ) $(LIB_NAME) $(FILE)_unittest

.PHONY: all install clean