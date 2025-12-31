#include <windows.h>
#include <stdio.h>
#include <string.h>

// Control identifiers
#define ID_ROOMMATE_INPUT    101
#define ID_ADD_ROOMMATE_BTN  102
#define ID_BILL_NAME_INPUT   103
#define ID_BILL_AMOUNT_INPUT 104
#define ID_ADD_BILL_BTN      105
#define ID_BILL_COMBO        106
#define ID_ROOMMATE_COMBO    107
#define ID_ASSIGN_BTN        108

// Data structure constants
#define MAX_NAME_LENGTH 50
#define MAX_ROOMMATES 10
#define MAX_BILL_NAME_LENGTH 50
#define MAX_BILLS 20
#define MAX_ASSIGNED_ROOMMATES 10

// Roommate structure
typedef struct {
    char name[MAX_NAME_LENGTH];
    int isActive;
} Roommate;

// Bill structure
typedef struct {
    char name[MAX_BILL_NAME_LENGTH];
    float amount;
    int assignedRoommates[MAX_ASSIGNED_ROOMMATES];
    int assignedCount;
    int isActive;
} Bill;

// Global data arrays
Roommate roommates[MAX_ROOMMATES] = {0};
Bill bills[MAX_BILLS] = {0};
int roommateCount = 0;
int billCount = 0;

// Global control handles
HWND hwndRoommateInput;
HWND hwndBillNameInput;
HWND hwndBillAmountInput;
HWND hwndBillCombo;
HWND hwndRoommateCombo;

// Helper function: Calculate total owed by a roommate
float calculateRoommateTotal(int roommateIndex) {
    float total = 0.0;
    for (int i = 0; i < billCount; i++) {
        if (!bills[i].isActive) continue;

        // Check if roommate is assigned to this bill
        int isAssigned = 0;
        for (int j = 0; j < bills[i].assignedCount; j++) {
            if (bills[i].assignedRoommates[j] == roommateIndex) {
                isAssigned = 1;
                break;
            }
        }

        if (isAssigned && bills[i].assignedCount > 0) {
            total += bills[i].amount / bills[i].assignedCount;
        }
    }
    return total;
}

// Helper function: Update bill combo box
void updateBillComboBox(HWND hwndCombo) {
    SendMessage(hwndCombo, CB_RESETCONTENT, 0, 0);
    for (int i = 0; i < billCount; i++) {
        if (bills[i].isActive) {
            SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)bills[i].name);
        }
    }
}

// Helper function: Update roommate combo box
void updateRoommateComboBox(HWND hwndCombo) {
    SendMessage(hwndCombo, CB_RESETCONTENT, 0, 0);
    for (int i = 0; i < roommateCount; i++) {
        if (roommates[i].isActive) {
            SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)roommates[i].name);
        }
    }
}

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

    // ===== ROOMMATE SECTION =====
    // Label: "Add Roommate:"
    CreateWindow("STATIC", "Add Roommate:",
        WS_VISIBLE | WS_CHILD,
        20, 20, 150, 20,
        hwnd, NULL, hInstance, NULL);

    // Text input for roommate name
    hwndRoommateInput = CreateWindow("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        20, 45, 200, 25,
        hwnd, (HMENU)ID_ROOMMATE_INPUT, hInstance, NULL);

    // Button: "Add Roommate"
    CreateWindow("BUTTON", "Add Roommate",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        230, 45, 120, 25,
        hwnd, (HMENU)ID_ADD_ROOMMATE_BTN, hInstance, NULL);

    // ===== BILL SECTION =====
    // Label: "Add Bill:"
    CreateWindow("STATIC", "Add Bill:",
        WS_VISIBLE | WS_CHILD,
        20, 90, 150, 20,
        hwnd, NULL, hInstance, NULL);

    // Text input for bill name
    hwndBillNameInput = CreateWindow("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        20, 115, 150, 25,
        hwnd, (HMENU)ID_BILL_NAME_INPUT, hInstance, NULL);

    // Text input for bill amount
    hwndBillAmountInput = CreateWindow("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        180, 115, 100, 25,
        hwnd, (HMENU)ID_BILL_AMOUNT_INPUT, hInstance, NULL);

    // Button: "Add Bill"
    CreateWindow("BUTTON", "Add Bill",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        290, 115, 100, 25,
        hwnd, (HMENU)ID_ADD_BILL_BTN, hInstance, NULL);

    // ===== ASSIGNMENT SECTION =====
    // Label: "Assign Bill to Roommate:"
    CreateWindow("STATIC", "Assign Bill to Roommate:",
        WS_VISIBLE | WS_CHILD,
        20, 160, 200, 20,
        hwnd, NULL, hInstance, NULL);

    // Combo box for bill selection
    hwndBillCombo = CreateWindow("COMBOBOX", "",
        WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
        20, 185, 150, 200,
        hwnd, (HMENU)ID_BILL_COMBO, hInstance, NULL);

    // Combo box for roommate selection
    hwndRoommateCombo = CreateWindow("COMBOBOX", "",
        WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
        180, 185, 150, 200,
        hwnd, (HMENU)ID_ROOMMATE_COMBO, hInstance, NULL);

    // Button: "Assign"
    CreateWindow("BUTTON", "Assign",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        340, 185, 100, 25,
        hwnd, (HMENU)ID_ASSIGN_BTN, hInstance, NULL);

    // Separator line label
    CreateWindow("STATIC", "───────────────────────────────────────────────",
        WS_VISIBLE | WS_CHILD,
        20, 260, 550, 20,
        hwnd, NULL, hInstance, NULL);

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
