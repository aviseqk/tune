$(info SRCS = $(SRCS))
$(info OBJS = $(OBJS))

# Compiler and flags
CC = gcc
CFLAGS = -g -fsanitize=address -Wall -Wextra -I$(INC_DIR)
LDFLAGS = -fsanitize=address

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
LIB_NAME = libtune_backend.a

# Source and Object Files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o, $(SRCS))

# Default: build the static library
all: $(LIB_NAME)

# compile object files

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# build static library
$(LIB_NAME): $(OBJS)
	ar rcs $@ $^

# clean build
clean:
	rm -rf $(BUILD_DIR) $(LIB_NAME)