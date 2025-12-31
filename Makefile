# Makefile for Windows C Application
# Compiles for Windows using MinGW cross-compiler

CC = x86_64-w64-mingw32-gcc # MinGW cross-compiler
CFLAGS = -Wall # Enable all warnings
LIBS = -luser32 -lgdi32 # Link to user32 and gdi32 libraries
TARGET = MyApp.exe # Target executable name
SRC = main.c # Source file

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
