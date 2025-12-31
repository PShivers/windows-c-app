#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "roommate.h"
#include "bill.h"
#include "ui_controls.h"

// Right panel window procedure for custom painting
LRESULT CALLBACK RightPanelProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            // Handle button clicks from controls within the panel
            if (LOWORD(wParam) == ID_ADD_ROOMMATE_BTN) {
                char name[MAX_NAME_LENGTH];
                GetWindowText(hwndRoommateInput, name, MAX_NAME_LENGTH);

                // Validate: not empty and under limit
                if (strlen(name) > 0 && roommateCount < MAX_ROOMMATES) {
                    strncpy(roommates[roommateCount].name, name, MAX_NAME_LENGTH - 1);
                    roommates[roommateCount].name[MAX_NAME_LENGTH - 1] = '\0';
                    roommates[roommateCount].isActive = 1;
                    roommateCount++;

                    // Clear input and update combo
                    SetWindowText(hwndRoommateInput, "");
                    updateRoommateComboBox(hwndRoommateCombo);
                    InvalidateRect(hwndRightPanel, NULL, TRUE);
                }
            }
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Set background to white
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

            // Draw "TOTALS OWED:" header (moved down to make room for roommate controls)
            const char* header = "TOTALS OWED:";
            TextOut(hdc, 10, 105, header, strlen(header));

            // Draw each roommate's total
            int yPos = 135;
            for (int i = 0; i < roommateCount; i++) {
                if (!roommates[i].isActive) continue;

                float total = calculateRoommateTotal(i);
                char line[200];
                sprintf(line, "%s: $%.2f", roommates[i].name, total);
                TextOut(hdc, 10, yPos, line, strlen(line));
                yPos += 25;
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Window procedure callback function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_ADD_BILL_BTN: {
                    char billName[MAX_BILL_NAME_LENGTH];
                    char amountStr[50];
                    float amount;

                    GetWindowText(hwndBillNameInput, billName, MAX_BILL_NAME_LENGTH);
                    GetWindowText(hwndBillAmountInput, amountStr, 50);

                    // Parse and validate amount
                    if (strlen(billName) > 0 && sscanf(amountStr, "%f", &amount) == 1
                        && amount > 0 && billCount < MAX_BILLS) {
                        strncpy(bills[billCount].name, billName, MAX_BILL_NAME_LENGTH - 1);
                        bills[billCount].name[MAX_BILL_NAME_LENGTH - 1] = '\0';
                        bills[billCount].amount = amount;
                        bills[billCount].assignedCount = 0;
                        bills[billCount].isActive = 1;
                        billCount++;

                        // Clear inputs and update combo
                        SetWindowText(hwndBillNameInput, "");
                        SetWindowText(hwndBillAmountInput, "");
                        updateBillComboBox(hwndBillCombo);
                        InvalidateRect(hwndRightPanel, NULL, TRUE);
                    }
                    break;
                }

                case ID_ASSIGN_BTN: {
                    int billIdx = SendMessage(hwndBillCombo, CB_GETCURSEL, 0, 0);
                    int roommateIdx = SendMessage(hwndRoommateCombo, CB_GETCURSEL, 0, 0);

                    if (billIdx != CB_ERR && roommateIdx != CB_ERR
                        && bills[billIdx].assignedCount < MAX_ASSIGNED_ROOMMATES) {

                        // Check if already assigned
                        int alreadyAssigned = 0;
                        for (int i = 0; i < bills[billIdx].assignedCount; i++) {
                            if (bills[billIdx].assignedRoommates[i] == roommateIdx) {
                                alreadyAssigned = 1;
                                break;
                            }
                        }

                        if (!alreadyAssigned) {
                            bills[billIdx].assignedRoommates[bills[billIdx].assignedCount] = roommateIdx;
                            bills[billIdx].assignedCount++;
                            InvalidateRect(hwndRightPanel, NULL, TRUE);
                        }
                    }
                    break;
                }
            }
            return 0;

        case WM_SIZE: {
            // Keep right panel pinned to right edge when window is resized
            if (hwndRightPanel) {
                int windowWidth = LOWORD(lParam);
                int windowHeight = HIWORD(lParam);

                SetWindowPos(hwndRightPanel, NULL,
                             windowWidth - 200, 10,           // Always 200px from right edge, start at y=10
                             200, windowHeight - 10,          // Full height minus top offset
                             SWP_NOZORDER);
            }
            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Set background color
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            EndPaint(hwnd, &ps);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {

    // Define window class for main window
    const char CLASS_NAME[] = "BasicWindowClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    // Register main window class
    RegisterClass(&wc);

    // Define window class for right panel
    const char PANEL_CLASS_NAME[] = "RightPanelClass";

    WNDCLASS wcPanel = {0};
    wcPanel.lpfnWndProc = RightPanelProc;
    wcPanel.hInstance = hInstance;
    wcPanel.lpszClassName = PANEL_CLASS_NAME;
    wcPanel.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcPanel.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    // Register panel window class
    RegisterClass(&wcPanel);

    // Create window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "Bill Splitter",                // Window title
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Position
        600, 550,                       // Size (wider and taller)
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    // Create UI controls using modular functions
    createBillControls(hwnd, hInstance);
    createAssignmentControls(hwnd, hInstance);
    createRightPanel(hwnd, hInstance);  // This now also creates roommate controls inside

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
