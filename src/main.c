#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "roommate.h"
#include "bill.h"
#include "ui_controls.h"

// Dialog procedure for Add Bill popup
LRESULT CALLBACK AddBillDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Create labels and input fields
            CreateWindow("STATIC", "Bill Name:",
                WS_VISIBLE | WS_CHILD,
                10, 10, 100, 20,
                hwndDlg, NULL, GetModuleHandle(NULL), NULL);

            CreateWindow("EDIT", "",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                120, 10, 150, 25,
                hwndDlg, (HMENU)ID_BILL_NAME_INPUT, GetModuleHandle(NULL), NULL);

            CreateWindow("STATIC", "Amount:",
                WS_VISIBLE | WS_CHILD,
                10, 45, 100, 20,
                hwndDlg, NULL, GetModuleHandle(NULL), NULL);

            CreateWindow("EDIT", "",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                120, 45, 150, 25,
                hwndDlg, (HMENU)ID_BILL_AMOUNT_INPUT, GetModuleHandle(NULL), NULL);

            CreateWindow("STATIC", "Due Date:",
                WS_VISIBLE | WS_CHILD,
                10, 80, 100, 20,
                hwndDlg, NULL, GetModuleHandle(NULL), NULL);

            CreateWindow("EDIT", "",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                120, 80, 150, 25,
                hwndDlg, (HMENU)ID_BILL_DUE_DATE_INPUT, GetModuleHandle(NULL), NULL);

            // Create OK and Cancel buttons
            CreateWindow("BUTTON", "OK",
                WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                60, 120, 80, 30,
                hwndDlg, (HMENU)ID_DIALOG_OK, GetModuleHandle(NULL), NULL);

            CreateWindow("BUTTON", "Cancel",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                150, 120, 80, 30,
                hwndDlg, (HMENU)ID_DIALOG_CANCEL, GetModuleHandle(NULL), NULL);

            return 0;
        }

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_DIALOG_OK: {
                    char billName[MAX_BILL_NAME_LENGTH];
                    char amountStr[50];
                    char dueDate[20];
                    float amount;

                    GetDlgItemText(hwndDlg, ID_BILL_NAME_INPUT, billName, MAX_BILL_NAME_LENGTH);
                    GetDlgItemText(hwndDlg, ID_BILL_AMOUNT_INPUT, amountStr, 50);
                    GetDlgItemText(hwndDlg, ID_BILL_DUE_DATE_INPUT, dueDate, 20);

                    // Parse and validate amount
                    if (strlen(billName) > 0 && sscanf(amountStr, "%f", &amount) == 1
                        && amount > 0 && billCount < MAX_BILLS) {
                        strncpy(bills[billCount].name, billName, MAX_BILL_NAME_LENGTH - 1);
                        bills[billCount].name[MAX_BILL_NAME_LENGTH - 1] = '\0';
                        bills[billCount].amount = amount;
                        strncpy(bills[billCount].dueDate, dueDate, 19);
                        bills[billCount].dueDate[19] = '\0';
                        bills[billCount].assignedCount = 0;
                        bills[billCount].isActive = 1;
                        billCount++;

                        updateBillComboBox(hwndBillCombo);
                        InvalidateRect(hwndLeftPanel, NULL, TRUE);
                        InvalidateRect(hwndRightPanel, NULL, TRUE);

                        HWND hwndParent = GetParent(hwndDlg);
                        EnableWindow(hwndParent, TRUE);
                        SetForegroundWindow(hwndParent);
                        DestroyWindow(hwndDlg);
                    } else {
                        MessageBox(hwndDlg, "Please enter a valid bill name and amount.", "Invalid Input", MB_OK | MB_ICONWARNING);
                    }
                    return 0;
                }

                case ID_DIALOG_CANCEL: {
                    HWND hwndParent = GetParent(hwndDlg);
                    EnableWindow(hwndParent, TRUE);
                    SetForegroundWindow(hwndParent);
                    DestroyWindow(hwndDlg);
                    return 0;
                }
            }
            break;

        case WM_CLOSE: {
            HWND hwndParent = GetParent(hwndDlg);
            EnableWindow(hwndParent, TRUE);
            SetForegroundWindow(hwndParent);
            DestroyWindow(hwndDlg);
            return 0;
        }

        case WM_DESTROY: {
            HWND hwndParent = GetParent(hwndDlg);
            if (hwndParent) {
                EnableWindow(hwndParent, TRUE);
                SetForegroundWindow(hwndParent);
            }
            return 0;
        }
    }
    return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
}

// Left panel window procedure for bills display
LRESULT CALLBACK LeftPanelProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == ID_OPEN_ADD_BILL_DIALOG) {
                // Create and show the Add Bill dialog as a popup window
                HWND hwndParent = GetParent(hwnd);
                HWND hwndDialog = CreateWindowEx(
                    WS_EX_DLGMODALFRAME,
                    "AddBillDialogClass",
                    "Add Bill",
                    WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
                    CW_USEDEFAULT, CW_USEDEFAULT,
                    300, 200,
                    hwndParent,
                    NULL,
                    GetModuleHandle(NULL),
                    NULL
                );
                EnableWindow(hwndParent, FALSE);  // Make it modal
            }
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Set background to white
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

            // Draw "BILLS:" header (moved down to make room for button)
            const char* header = "BILLS:";
            TextOut(hdc, 10, 50, header, strlen(header));

            // Draw each bill with name, amount, and due date
            int yPos = 80;
            for (int i = 0; i < billCount; i++) {
                if (!bills[i].isActive) continue;

                char line[200];
                sprintf(line, "%s", bills[i].name);
                TextOut(hdc, 10, yPos, line, strlen(line));
                yPos += 20;

                sprintf(line, "$%.2f", bills[i].amount);
                TextOut(hdc, 10, yPos, line, strlen(line));
                yPos += 20;

                sprintf(line, "Due: %s", bills[i].dueDate);
                TextOut(hdc, 10, yPos, line, strlen(line));
                yPos += 30;  // Extra space between bills
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

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

    // Define window class for left panel
    const char LEFT_PANEL_CLASS_NAME[] = "LeftPanelClass";

    WNDCLASS wcLeftPanel = {0};
    wcLeftPanel.lpfnWndProc = LeftPanelProc;
    wcLeftPanel.hInstance = hInstance;
    wcLeftPanel.lpszClassName = LEFT_PANEL_CLASS_NAME;
    wcLeftPanel.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcLeftPanel.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    // Register left panel window class
    RegisterClass(&wcLeftPanel);

    // Define window class for Add Bill dialog
    const char DIALOG_CLASS_NAME[] = "AddBillDialogClass";

    WNDCLASS wcDialog = {0};
    wcDialog.lpfnWndProc = AddBillDialogProc;
    wcDialog.hInstance = hInstance;
    wcDialog.lpszClassName = DIALOG_CLASS_NAME;
    wcDialog.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcDialog.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    // Register dialog window class
    RegisterClass(&wcDialog);

    // Create window (wider to accommodate both panels)
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "Bill Splitter",                // Window title
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Position
        850, 550,                       // Size (wider for both panels)
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    // Create UI controls using modular functions
    createLeftPanel(hwnd, hInstance);   // Left panel for bills list with Add Bill button
    createAssignmentControls(hwnd, hInstance);
    createRightPanel(hwnd, hInstance);  // Right panel with roommate controls inside

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
