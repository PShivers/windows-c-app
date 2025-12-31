#include "roommate.h"
#include "bill.h"
#include <string.h>

// Global roommate data
Roommate roommates[MAX_ROOMMATES] = {0};
int roommateCount = 0;

// Calculate total amount owed by a roommate
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

// Update roommate combo box with current roommates
void updateRoommateComboBox(HWND hwndCombo) {
    SendMessage(hwndCombo, CB_RESETCONTENT, 0, 0);

    for (int i = 0; i < roommateCount; i++) {
        if (roommates[i].isActive) {
            SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)roommates[i].name);
        }
    }
}
