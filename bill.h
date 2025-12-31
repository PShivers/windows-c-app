#ifndef BILL_H
#define BILL_H

#include <windows.h>

// Constants
#define MAX_BILL_NAME_LENGTH 50
#define MAX_BILLS 20
#define MAX_ASSIGNED_ROOMMATES 10

// Bill structure
typedef struct {
    char name[MAX_BILL_NAME_LENGTH];
    float amount;
    int assignedRoommates[MAX_ASSIGNED_ROOMMATES];
    int assignedCount;
    int isActive;
} Bill;

// Global bill data (extern - defined in bill.c)
extern Bill bills[MAX_BILLS];
extern int billCount;

// Function declarations
void updateBillComboBox(HWND hwndCombo);

#endif // BILL_H
