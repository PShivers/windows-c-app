# Makefile for Windows C Application
# Compiles for Windows using MinGW cross-compiler

CC = x86_64-w64-mingw32-gcc # MinGW cross-compiler
CFLAGS = -Wall # Enable all warnings
LIBS = -luser32 -lgdi32 # Link to user32 and gdi32 libraries
TARGET = MyApp.exe # Target executable name
SRCS = main.c roommate.c bill.c ui_controls.c # All source files
OBJS = $(SRCS:.c=.o) # Object files

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
