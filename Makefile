# Compiler and flags
CC = gcc
CFLAGS = -Wall -pthread

# Source files
SRCS = main.c inventory.c order_manager.c logger.c

# Output binary name
TARGET = ecommerce

# Default target to build the project
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean build files
clean:
	rm -f $(TARGET)

# Run the program
run: all
	./$(TARGET)

.PHONY: all clean run
