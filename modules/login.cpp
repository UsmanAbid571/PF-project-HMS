#include "../headers/globals.h"
#include "../headers/login.h"

bool validateCredentials(const string& empId, const string& password) {
    if (empId == "EMP001" && password == "pass123") return true;

    return false;
}

bool login() {
    printHeader("Hospital Management System - Login");

    const int MAX_ATTEMPTS = 3;

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        string empId, password;

        cout << "\n  " << CYAN << "Attempt " << attempt << " of " << MAX_ATTEMPTS << RESET << "\n";
        cout << "  Employee ID : ";
        cin  >> empId;
        cout << "  Password    : ";
        cin  >> password;

        if (validateCredentials(empId, password)) {
            cout << "\n" << GREEN << "  Login successful! Welcome, " << empId << "." << RESET << "\n";
            pressEnter();
            return true;
        }

        int remaining = MAX_ATTEMPTS - attempt;
        if (remaining > 0)
            cout << RED << "  Invalid credentials. " << remaining << " attempt(s) remaining.\n" << RESET;
        else
            cout << RED << "  Too many failed attempts. Exiting.\n" << RESET;
    }

    return false;
}