CC = gcc

# Directories
BACKEND_DIR = ../backend
INCLUDE_DIR = $(BACKEND_DIR)/include
LIB_DIR = $(BACKEND_DIR)
LIB_NAME = tune_backend

# Flags
CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra -fsanitize=address -g
LDFLAGS = -L$(BACKEND_DIR) -l$(LIB_NAME)

# Target Executable
TARGET = main
SRCS = main.c

all: $(TARGET)

$(TARGET):
	$(info COMPILE: $(CC) $(CFLAGS) main.c -o $(TARGET) $(LDFLAGS))
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS) 

clean:
	rm -rf $(TARGET)