# CS2106 Lab 3 Part 3 - H2O Molecule Simulation Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pthread
LDFLAGS = -pthread

# Target executable
TARGET = h2o

# Source files
SOURCES = runner.c h2o.c barrier.c 
HEADERS = runner.h h2o.h barrier.h

# Object files (automatically generated from source files)
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Build object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Declare phony targets
.PHONY: all clean
