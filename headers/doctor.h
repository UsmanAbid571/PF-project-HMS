#ifndef DOCTOR_H
#define DOCTOR_H

#include "globals.h"

void addDoctor();
void updateDoctor();
void deleteDoctor();
void viewDoctors();

void sortDoctorsByExperience(Doctor* arr, int count);

Doctor findDoctorById(int id);
bool   doctorExists(int id);
int    loadAllDoctors(Doctor* arr, int maxSize);

void doctorMenu();

#endif 