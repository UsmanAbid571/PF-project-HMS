#ifndef CLEANING_H
#define CLEANING_H

#include "globals.h"

void cleanFile();


string fixGender(string g);
string fixDate(string d);
string fixTime(string t);


bool isValidContact(const string& c);
bool isValidRecord(const Patient& p);


void removeDuplicatePatients();

#endif 