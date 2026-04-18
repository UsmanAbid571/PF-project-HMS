#ifndef BONUS_H
#define BONUS_H

#include "globals.h"

// 2D weekly schedule: schedule[day][slot]
// 0 = Free, otherwise = patientId booked in that slot
// Days:  0=Mon 1=Tue 2=Wed 3=Thu 4=Fri
// Slots: 0=9AM 1=10AM 2=11AM 3=12PM 4=1PM 5=2PM 6=3PM 7=4PM

extern int weeklySchedule[5][8];

void displayWeeklySchedule(int docId);
void bookSlotIfFree(int day, int slot, int patientId);
int  findNextFreeSlot(int day, int slot, int sch[][8]);

void bonusMenu();

#endif 