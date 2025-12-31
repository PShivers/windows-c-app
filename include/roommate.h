#ifndef ROOMMATE_H
#define ROOMMATE_H

#include <windows.h>

// Constants
#define MAX_NAME_LENGTH 50
#define MAX_ROOMMATES 10

// Roommate structure
typedef struct {
    char name[MAX_NAME_LENGTH];
    int isActive;
} Roommate;

// Global roommate data (extern - defined in roommate.c)
extern Roommate roommates[MAX_ROOMMATES];
extern int roommateCount;

// Function declarations
float calculateRoommateTotal(int roommateIndex);
void updateRoommateComboBox(HWND hwndCombo);

#endif // ROOMMATE_H
