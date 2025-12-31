#include <windows.h>
#include <stdio.h>

// Button control identifier
#define ID_BUTTON 1

// Global state for click counter
int clickCount = 0;

// Window procedure callback function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_COMMAND:
            // Handle button clicks
            if (LOWORD(wParam) == ID_BUTTON) {
                clickCount++;
                // Force window to repaint and show updated count
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Set background color
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            // Draw text
            const char* text = "Hello, Windows!";
            TextOut(hdc, 50, 50, text, strlen(text));

            // Draw click counter below button
            char counterText[100];
            sprintf(counterText, "Button clicked: %d times", clickCount);
            TextOut(hdc, 50, 150, counterText, strlen(counterText));

            EndPaint(hwnd, &ps);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {

    // Define window class
    const char CLASS_NAME[] = "BasicWindowClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    // Register window class
    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "My First Windows App",         // Window title
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Position
        500, 300,                       // Size
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    // Create button control
    HWND hwndButton = CreateWindow(
        "BUTTON",                                   // Predefined button class
        "Click Me!",                                // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        50, 100,                                    // x, y position
        200, 30,                                    // width, height
        hwnd,                                       // Parent window
        (HMENU)ID_BUTTON,                          // Button identifier
        hInstance,                                  // Instance handle
        NULL                                        // No additional data
    );

    // Show window
    ShowWindow(hwnd, nCmdShow);

    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
