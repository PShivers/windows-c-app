# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a Windows GUI application written in C using the Win32 API. The project demonstrates basic Windows programming concepts including window creation, message handling, and drawing operations.

## Development Commands

### Build
```bash
make                    # Compiles using MinGW cross-compiler
```

### Clean
```bash
make clean             # Removes compiled executable
```

### Manual Compilation (alternative methods)
```bash
# MinGW cross-compilation (primary method)
x86_64-w64-mingw32-gcc main.c -o MyApp.exe -luser32 -lgdi32

# MSVC (Windows only)
cl main.c /link user32.lib gdi32.lib

# GCC on Windows (MinGW)
gcc main.c -o MyApp.exe -luser32 -lgdi32
```

## Architecture

### Core Components
- **WinMain**: Entry point for Windows GUI applications - handles application initialization and main execution flow
- **WindowProc**: Message processing callback that handles all window events (WM_PAINT for drawing, WM_DESTROY for cleanup)
- **Message Loop**: Event-driven architecture using GetMessage/TranslateMessage/DispatchMessage pattern

### Key Dependencies
- **user32.lib/luser32**: Core Windows API for window management, message handling
- **gdi32.lib/lgdi32**: Graphics Device Interface for drawing operations and text rendering

### Application Flow
1. Window class registration with custom WindowProc callback
2. Window creation with specified dimensions (500x300) and style
3. Event loop processing messages until WM_QUIT received
4. Custom drawing in WM_PAINT handler (background fill + text rendering)

## Development Environment

Requires MinGW-w64 cross-compiler for Linux/WSL development targeting Windows. The Makefile is configured for cross-compilation using `x86_64-w64-mingw32-gcc`.