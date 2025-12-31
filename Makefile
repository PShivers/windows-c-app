# Makefile for Windows C Application
# Compiles for Windows using MinGW cross-compiler

CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Iinclude
LIBS = -luser32 -lgdi32

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Files
TARGET = MyApp.exe
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(BUILD_DIR)/*.o
	rm -rf $(BUILD_DIR)

.PHONY: all clean
