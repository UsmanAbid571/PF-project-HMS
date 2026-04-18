#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include "globals.h"

void viewAppointments();
void cancelAppointment();

void scheduleAppointment();
bool checkPatientBalance(int patientId, double cost);

void appointmentMenu();

#endif