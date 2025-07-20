# Compiler and flags
CC = gcc
# STRICT COMPILATION
# CFLAGS = -g -fsanitize=address -Wall -Wextra -Werror

# either use valgrind to analyse memory leaks or use AddressSanitizer by using this -fsanitize flag. Both of these wont work together
CFLAGS = -g -fsanitize=address
LDFLAGS = -fsanitize=address

# Executable name
TARGET = process_info

# Source files
SRCS = process_info.c data_structures.c thread_parser.c proc_parser.c manage_memory.c cpu_monitor.c

# Header files (not strictly required unless using make dependencies)
HDRS = data_structures.h data_types.h manage_memory.h memory_monitor.h hdr.h proc_thread_parser.h cpu_monitor.h

# Object files (optional optimization)
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o
