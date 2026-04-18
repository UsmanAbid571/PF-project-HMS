#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

// File paths 
const string PATIENTS_FILE    = "data/patients.txt";
const string DOCTORS_FILE     = "data/doctors.txt";
const string APPOINTMENTS_FILE= "data/appointments.txt";
const string TREATMENTS_FILE  = "data/treatments.txt";
const string BILLS_FILE       = "data/bills.txt";

// Array size limits
const int MAX_RECORDS = 500;

// ANSI colour codes 
const string RESET  = "\033[0m";
const string BOLD   = "\033[1m";
const string RED    = "\033[1;31m";
const string GREEN  = "\033[1;32m";
const string YELLOW = "\033[1;33m";
const string BLUE   = "\033[1;34m";
const string CYAN   = "\033[1;36m";
const string WHITE  = "\033[1;37m";


//  STRUCTS

struct Patient {
    int    patientId;       
    string name;            
    int    age;
    string gender;         
    string contact;       
    double balance;         
};

struct Doctor {
    int    doc_id;
    string name;
    string specialty;       
    int    experience;      
};

struct Appointment {
    int  patientId;
    int  doctorId;
    char date[11];         
    char time[10];          
};

struct Treatment {
    int    patientId;
    string description;    
    double cost;
    bool   paid;            
};


// Convert string to lowercase
inline string toLower(string s) {
    for (char& c : s) c = tolower(c);
    return s;
}

// Print a styled section divider
inline void printDivider(char ch = '-', int width = 65, string color = CYAN) {
    cout << color;
    for (int i = 0; i < width; i++) cout << ch;
    cout << RESET << "\n";
}

// Print a banner heading
inline void printHeader(const string& title) {
    cout << "\n";
    printDivider('=', 65, BLUE);
    cout << BLUE << BOLD << " \t " << title << RESET << "\n";
    printDivider('=', 65, BLUE);
}

// Count lines (records) in a file
inline int countLines(const string& filename) {
    ifstream f(filename);
    if (!f.is_open()) return 0;
    int count = 0;
    string line;
    while (getline(f, line))
        if (!line.empty()) count++;
    return count;
}

// Pause and wait for user
inline void pressEnter() {
    cout << YELLOW << "\n  Press Enter to continue..." << RESET;
    cin.ignore(1000, '\n');
    cin.get();
}

#endif 