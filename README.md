# Basic Windows C Application

A simple Windows GUI application written in C using the Win32 API.

## What This Application Does

This program creates a basic window that displays "Hello, Windows!" text. It demonstrates:
- Creating a window using Win32 API
- Handling window messages
- Drawing text on the window
- Basic event loop

## Prerequisites

To compile this application, you need a C compiler that supports Windows API:
- **MinGW-w64** (recommended for WSL/Linux cross-compilation)
- **Microsoft Visual C++** (MSVC)
- **Clang with Windows SDK**

## Compilation Instructions

### Using MinGW-w64 (Cross-compile from WSL/Linux)

Install MinGW-w64:
```bash
sudo apt-get update
sudo apt-get install mingw-w64
```

Compile:
```bash
x86_64-w64-mingw32-gcc main.c -o MyApp.exe -luser32 -lgdi32
```

### Using MSVC (Windows Command Prompt)

```cmd
cl main.c /link user32.lib gdi32.lib
```

### Using GCC on Windows (MinGW)

```cmd
gcc main.c -o MyApp.exe -luser32 -lgdi32
```

## Running the Application

After compilation, run the executable:
```cmd
MyApp.exe
```

A window should appear with the title "My First Windows App" displaying "Hello, Windows!" text.

## Code Structure

- **WinMain**: Entry point for Windows GUI applications
- **WindowProc**: Callback function that handles window messages
- **WM_PAINT**: Handles window drawing/rendering
- **WM_DESTROY**: Handles window closure
- **Message Loop**: Processes window events

## Next Steps

To expand this application, you could:
- Add buttons and controls
- Handle mouse and keyboard input
- Add menus
- Draw graphics and shapes
- Load and display images
