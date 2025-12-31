#include "bill.h"

// Global bill data
Bill bills[MAX_BILLS] = {0};
int billCount = 0;

// Update bill combo box with current bills
void updateBillComboBox(HWND hwndCombo) {
    SendMessage(hwndCombo, CB_RESETCONTENT, 0, 0);

    for (int i = 0; i < billCount; i++) {
        if (bills[i].isActive) {
            SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)bills[i].name);
        }
    }
}
