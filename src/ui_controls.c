#include "ui_controls.h"

// Global control handles
HWND hwndRoommateInput;
HWND hwndBillNameInput;
HWND hwndBillAmountInput;
HWND hwndBillDueDateInput;
HWND hwndBillCombo;
HWND hwndRoommateCombo;
HWND hwndRightPanel;
HWND hwndLeftPanel;

// Create roommate input controls
void createRoommateControls(HWND hwndParent, HINSTANCE hInstance) {
    // Label: "Add Roommate:"
    CreateWindow("STATIC", "Add Roommate:",
        WS_VISIBLE | WS_CHILD,
        10, 10, 180, 20,
        hwndParent, NULL, hInstance, NULL);

    // Text input for roommate name
    hwndRoommateInput = CreateWindow("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        10, 35, 160, 25,
        hwndParent, (HMENU)ID_ROOMMATE_INPUT, hInstance, NULL);

    // Button: "Add Roommate"
    CreateWindow("BUTTON", "Add",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 65, 160, 25,
        hwndParent, (HMENU)ID_ADD_ROOMMATE_BTN, hInstance, NULL);
}

// Create bill input controls
void createBillControls(HWND hwndParent, HINSTANCE hInstance) {
    // Label: "Add Bill:"
    CreateWindow("STATIC", "Add Bill:",
        WS_VISIBLE | WS_CHILD,
        220, 10, 150, 20,
        hwndParent, NULL, hInstance, NULL);

    // Label: "Name:"
    CreateWindow("STATIC", "Name:",
        WS_VISIBLE | WS_CHILD,
        220, 35, 50, 20,
        hwndParent, NULL, hInstance, NULL);

    // Text input for bill name
    hwndBillNameInput = CreateWindow("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        280, 35, 100, 25,
        hwndParent, (HMENU)ID_BILL_NAME_INPUT, hInstance, NULL);

    // Label: "Amount:"
    CreateWindow("STATIC", "Amount:",
        WS_VISIBLE | WS_CHILD,
        220, 65, 50, 20,
        hwndParent, NULL, hInstance, NULL);

    // Text input for bill amount
    hwndBillAmountInput = CreateWindow("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        280, 65, 100, 25,
        hwndParent, (HMENU)ID_BILL_AMOUNT_INPUT, hInstance, NULL);

    // Label: "Due Date:"
    CreateWindow("STATIC", "Due Date:",
        WS_VISIBLE | WS_CHILD,
        220, 95, 60, 20,
        hwndParent, NULL, hInstance, NULL);

    // Text input for due date
    hwndBillDueDateInput = CreateWindow("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        280, 95, 100, 25,
        hwndParent, (HMENU)ID_BILL_DUE_DATE_INPUT, hInstance, NULL);

    // Button: "Add Bill"
    CreateWindow("BUTTON", "Add Bill",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        220, 125, 160, 25,
        hwndParent, (HMENU)ID_ADD_BILL_BTN, hInstance, NULL);
}

// Create assignment controls
void createAssignmentControls(HWND hwndParent, HINSTANCE hInstance) {
    // Label: "Assign Bill to Roommate:"
    CreateWindow("STATIC", "Assign Bill to Roommate:",
        WS_VISIBLE | WS_CHILD,
        220, 10, 200, 20,
        hwndParent, NULL, hInstance, NULL);

    // Label: "Bill:"
    CreateWindow("STATIC", "Bill:",
        WS_VISIBLE | WS_CHILD,
        220, 40, 40, 20,
        hwndParent, NULL, hInstance, NULL);

    // Combo box for bill selection
    hwndBillCombo = CreateWindow("COMBOBOX", "",
        WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
        270, 40, 120, 200,
        hwndParent, (HMENU)ID_BILL_COMBO, hInstance, NULL);

    // Label: "Roommate:"
    CreateWindow("STATIC", "Roommate:",
        WS_VISIBLE | WS_CHILD,
        220, 75, 70, 20,
        hwndParent, NULL, hInstance, NULL);

    // Combo box for roommate selection
    hwndRoommateCombo = CreateWindow("COMBOBOX", "",
        WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
        300, 75, 120, 200,
        hwndParent, (HMENU)ID_ROOMMATE_COMBO, hInstance, NULL);

    // Button: "Assign"
    CreateWindow("BUTTON", "Assign",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        220, 110, 200, 30,
        hwndParent, (HMENU)ID_ASSIGN_BTN, hInstance, NULL);

    // Separator line label
    // CreateWindow("STATIC", "───────────────────────────────────────────────",
    //     WS_VISIBLE | WS_CHILD,
    //     20, 260, 550, 20,
    //     hwndParent, NULL, hInstance, NULL);
}

// Create right panel for totals display
void createRightPanel(HWND hwndParent, HINSTANCE hInstance) {
    // Create a child window as the right panel using custom window class
    hwndRightPanel = CreateWindowEx(
        WS_EX_CLIENTEDGE,           // Sunken border style
        "RightPanelClass",          // Custom window class (registered in main.c)
        "",                         // No text
        WS_CHILD | WS_VISIBLE,      // Child, visible
        640, 10,                    // Position (x, y) - positioned on far right
        200, 540,                   // Size (width, height)
        hwndParent,                 // Parent window
        NULL,                       // No menu
        hInstance,                  // Instance handle
        NULL                        // No additional data
    );

    // Create roommate controls inside the right panel
    createRoommateControls(hwndRightPanel, hInstance);
}

// Create left panel for bills display
void createLeftPanel(HWND hwndParent, HINSTANCE hInstance) {
    // Create a child window as the left panel using custom window class
    hwndLeftPanel = CreateWindowEx(
        WS_EX_CLIENTEDGE,           // Sunken border style
        "LeftPanelClass",           // Custom window class (registered in main.c)
        "",                         // No text
        WS_CHILD | WS_VISIBLE,      // Child, visible
        10, 10,                     // Position (x, y)
        200, 540,                   // Size (width, height)
        hwndParent,                 // Parent window
        NULL,                       // No menu
        hInstance,                  // Instance handle
        NULL                        // No additional data
    );

    // Add "Add Bill" button at the top of the left panel
    CreateWindow("BUTTON", "Add Bill",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 10, 160, 30,
        hwndLeftPanel, (HMENU)ID_OPEN_ADD_BILL_DIALOG, hInstance, NULL);
}
