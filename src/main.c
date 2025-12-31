#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "roommate.h"
#include "bill.h"
#include "ui_controls.h"

// Window procedure callback function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_ADD_ROOMMATE_BTN: {
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
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    break;
                }

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
                        InvalidateRect(hwnd, NULL, TRUE);
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
                            InvalidateRect(hwnd, NULL, TRUE);
                        }
                    }
                    break;
                }
            }
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Set background color
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            // Draw "TOTALS OWED:" header
            const char* header = "TOTALS OWED:";
            TextOut(hdc, 20, 300, header, strlen(header));

            // Draw each roommate's total
            int yPos = 330;
            for (int i = 0; i < roommateCount; i++) {
                if (!roommates[i].isActive) continue;

                float total = calculateRoommateTotal(i);
                char line[200];
                sprintf(line, "  %s: $%.2f", roommates[i].name, total);
                TextOut(hdc, 20, yPos, line, strlen(line));
                yPos += 25;
            }

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
    createRoommateControls(hwnd, hInstance);
    createBillControls(hwnd, hInstance);
    createAssignmentControls(hwnd, hInstance);

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
