# Bill Splitter - Windows C Application

A Windows GUI application written in C using the Win32 API for splitting bills among roommates.

## What This Application Does

This program helps roommates manage and split bills fairly. It features:

- **Add Roommates** - Track who lives in your household
- **Add Bills** - Record bills with names and amounts
- **Assign Bills** - Assign each bill to specific roommates
- **Calculate Totals** - Automatically calculates how much each person owes (equal split among assigned roommates)

## Features

- Clean, modular codebase with separate files for different functionality
- Real-time calculation of amounts owed
- Intuitive GUI with text inputs, buttons, and dropdown menus
- Input validation and error handling

## Project Structure

```
windows-c-app/
├── src/                    # Source files
│   ├── main.c             # Entry point, event loop, message handling
│   ├── roommate.c         # Roommate data and calculations
│   ├── bill.c             # Bill data management
│   └── ui_controls.c      # UI creation functions
├── include/                # Header files
│   ├── roommate.h         # Roommate interface
│   ├── bill.h             # Bill interface
│   └── ui_controls.h      # UI controls interface
├── build/                  # Build artifacts (gitignored)
│   └── *.o                # Object files
├── Makefile               # Build configuration
└── MyApp.exe              # Compiled executable
```

## Prerequisites

To compile this application, you need:

- **MinGW-w64** (for WSL/Linux cross-compilation to Windows)
- **Microsoft Visual C++** (MSVC) - if compiling on Windows
- **Clang with Windows SDK** - alternative option

### Installing MinGW-w64 (WSL/Linux)

```bash
sudo apt-get update
sudo apt-get install mingw-w64
```

## Building the Application

### Using Make (Recommended)

```bash
make        # Build the application
make clean  # Remove all build artifacts
```

### Manual Compilation

#### MinGW-w64 (Cross-compile from WSL/Linux)

```bash
mkdir -p build
x86_64-w64-mingw32-gcc -Wall -Iinclude -c src/main.c -o build/main.o
x86_64-w64-mingw32-gcc -Wall -Iinclude -c src/roommate.c -o build/roommate.o
x86_64-w64-mingw32-gcc -Wall -Iinclude -c src/bill.c -o build/bill.o
x86_64-w64-mingw32-gcc -Wall -Iinclude -c src/ui_controls.c -o build/ui_controls.o
x86_64-w64-mingw32-gcc -Wall -Iinclude build/*.o -o MyApp.exe -luser32 -lgdi32
```

#### MSVC (Windows Command Prompt)

```cmd
cl /Iinclude src\main.c src\roommate.c src\bill.c src\ui_controls.c /link user32.lib gdi32.lib
```

## Running the Application

After compilation, run the executable:

```bash
MyApp.exe
```

**Note:** Since this is a Windows executable (`.exe`), you'll need to run it on Windows or through a Windows compatibility layer like Wine if you're on Linux/WSL.

### Using Wine (Linux/WSL)

```bash
wine MyApp.exe
```

## How to Use

1. **Add Roommates**
   - Type a name in the "Add Roommate" text box
   - Click "Add Roommate" button
   - Repeat for all roommates

2. **Add Bills**
   - Enter bill name (e.g., "Rent", "Utilities")
   - Enter amount (e.g., "1500")
   - Click "Add Bill" button

3. **Assign Bills to Roommates**
   - Select a bill from the dropdown
   - Select a roommate from the dropdown
   - Click "Assign" button
   - Repeat to assign the same bill to multiple roommates

4. **View Totals**
   - The bottom section automatically updates showing how much each roommate owes
   - Bills are split equally among all assigned roommates

### Example

```
Add Roommates: Alice, Bob, Carol

Add Bills:
- Rent: $1500
- Utilities: $150

Assignments:
- Rent → Alice, Bob, Carol (each owes $500)
- Utilities → Alice, Bob (each owes $75)

TOTALS OWED:
• Alice: $575.00
• Bob: $575.00
• Carol: $500.00
```

## Code Architecture

### Main Components

- **main.c** - Window creation, event loop, and message handling
  - `WinMain()` - Entry point, creates window and starts message loop
  - `WindowProc()` - Handles all window messages (button clicks, painting)

- **roommate.c/h** - Roommate data structures and calculations
  - `Roommate` struct - Stores roommate name and active status
  - `calculateRoommateTotal()` - Calculates amount owed by a roommate
  - `updateRoommateComboBox()` - Updates dropdown with current roommates

- **bill.c/h** - Bill data structures and management
  - `Bill` struct - Stores bill name, amount, and assigned roommates
  - `updateBillComboBox()` - Updates dropdown with current bills

- **ui_controls.c/h** - UI creation functions
  - `createRoommateControls()` - Creates roommate input section
  - `createBillControls()` - Creates bill input section
  - `createAssignmentControls()` - Creates assignment section

### Event-Driven Architecture

The application uses Windows' message-driven architecture:
- User interactions generate messages (WM_COMMAND for button clicks)
- WindowProc processes these messages
- UI updates trigger WM_PAINT for redrawing

## Technical Details

### Data Structures

- **Roommate**: Name (50 chars max), active flag
- **Bill**: Name (50 chars max), amount (float), assigned roommates (indices), count
- Maximum 10 roommates, 20 bills

### Message Handling

- **WM_DESTROY** - Application closure
- **WM_COMMAND** - Button clicks and control interactions
- **WM_PAINT** - Drawing/redrawing the totals display

### Libraries Used

- **user32.lib** - Window management, message handling, controls
- **gdi32.lib** - Graphics Device Interface for text rendering

## Development

### Adding New Features

The modular structure makes it easy to extend:
- Add new data types → Create new .c/.h files in src/include
- Add UI elements → Extend ui_controls.c
- Add calculations → Extend roommate.c or bill.c
- Update Makefile SRCS to include new .c files

### Build System

The Makefile uses pattern rules for automatic compilation:
- Compiles each .c file in src/ to .o in build/
- Links all .o files with required libraries
- Only recompiles changed files for faster builds

## Next Steps

Potential enhancements:
- Save/load data to file
- Print reports
- Support for unequal bill splits
- Payment tracking (who has paid their share)
- Multiple households
- Export to CSV
