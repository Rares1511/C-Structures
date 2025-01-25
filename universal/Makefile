# Universal Makefile

FILE = universal

# Correct path to the source file
SRC = $(FILE).c 
OBJ = $(FILE).o
LIB = ../lib/lib$(FILE).a

# Compiler flags
CFLAGS = -Wall -Werror

# Compile the object file
$(OBJ): $(SRC)
	$(CC) -c -o $@ $^ $(CFLAGS)

# Create the static library
$(LIB): $(OBJ)
	$(AR) rcs $@ $^

# Install the library
install: $(LIB)
	sudo install -m 644 $^ $(LIBDIR)
	sudo install -d $(PATH_INCLUDEDIR)
	sudo install -m 644 $(LOCAL_INCLUDEDIR)/$(FILE).h $(PATH_INCLUDEDIR)

# Clean up
clean:
	rm -f $(OBJ) $(LIB)

.PHONY: install clean