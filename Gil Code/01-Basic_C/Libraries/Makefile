# Makefile to compile a complex program composed of many libraries

### Variables for this project ###
# The files that must be compiled, with a .o extension
OBJECTS = person.o
# The header files
DEPENDS = person.h
# The executable program to be created
MAIN = test

### Variables for the compilation rules ###
# These should work for most projects, but can be modified when necessary
# The compiler program to use
CC = gcc
# Options to use when compiling object files
CFLAGS = -Wall -g -std=c99 -pedantic # -O2
# Options to use for the final linking process
# This one links the math library
LDLIBS = -lm

# Default rule
all: $(MAIN)

# Rule to make the executable
$(MAIN): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Rule to make the object files
%.o: %.c $(DEPENDS)
	$(CC) $< -c -o $@ $(CFLAGS)

clean:
	rm -rf *.o $(MAIN)

zip: clean
	zip -r $(MAIN).zip *
	
.PHONY: clean all zip
