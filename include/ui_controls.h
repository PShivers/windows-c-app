#ifndef UI_CONTROLS_H
#define UI_CONTROLS_H

#include <windows.h>

// Control identifiers
#define ID_ROOMMATE_INPUT    101
#define ID_ADD_ROOMMATE_BTN  102
#define ID_BILL_NAME_INPUT   103
#define ID_BILL_AMOUNT_INPUT 104
#define ID_ADD_BILL_BTN      105
#define ID_BILL_COMBO        106
#define ID_ROOMMATE_COMBO    107
#define ID_ASSIGN_BTN        108

// Global control handles (extern - defined in ui_controls.c)
extern HWND hwndRoommateInput;
extern HWND hwndBillNameInput;
extern HWND hwndBillAmountInput;
extern HWND hwndBillCombo;
extern HWND hwndRoommateCombo;
extern HWND hwndRightPanel;

// Function declarations
void createRoommateControls(HWND hwndParent, HINSTANCE hInstance);
void createBillControls(HWND hwndParent, HINSTANCE hInstance);
void createAssignmentControls(HWND hwndParent, HINSTANCE hInstance);
void createRightPanel(HWND hwndParent, HINSTANCE hInstance);

#endif // UI_CONTROLS_H
