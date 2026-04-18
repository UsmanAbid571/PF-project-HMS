#ifndef PATIENT_H
#define PATIENT_H

#include "globals.h"


void addPatient();
void updatePatient();
void deletePatient();
void viewPatients();

Patient findPatientById(int id);
bool    patientExists(int id);
void    updatePatientBalance(int id, double newBalance);
int     loadAllPatients(Patient* arr, int maxSize);

void patientMenu();

#endif 