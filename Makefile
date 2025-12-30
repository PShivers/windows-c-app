# Makefile for Windows C Application
# Compiles for Windows using MinGW cross-compiler

CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall
LIBS = -luser32 -lgdi32
TARGET = MyApp.exe
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
