#include "headers/globals.h"
#include "headers/cleaning.h"
#include "headers/login.h"
#include "headers/patient.h"
#include "headers/doctor.h"
#include "headers/appointment.h"
#include "headers/treatment.h"
#include "headers/billing.h"
#include "headers/search.h"
#include "headers/bonus.h"

Patient* patients = nullptr;
Doctor*  doctors  = nullptr;
int      patientCount = 0;
int      doctorCount  = 0;

static void allocateDynamicMemory() {
    patientCount = countLines(PATIENTS_FILE);
    doctorCount  = countLines(DOCTORS_FILE);

    patients = new Patient[patientCount == 0 ? 1 : patientCount];
    doctors  = new Doctor [doctorCount  == 0 ? 1 : doctorCount ];

    loadAllPatients(patients, patientCount);
    loadAllDoctors (doctors,  doctorCount);
}

static void freeDynamicMemory() {
    delete[] patients;
    delete[] doctors;
    patients = nullptr;
    doctors  = nullptr;
}

static void showDashboard() {
    int totalPatients     = countLines(PATIENTS_FILE);
    int totalDoctors      = countLines(DOCTORS_FILE);
    int totalAppointments = countLines(APPOINTMENTS_FILE);
    int totalTreatments   = countLines(TREATMENTS_FILE);

    double revenue = 0.0, unpaid = 0.0;
    {
        ifstream f(BILLS_FILE);
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            istringstream ss(line);
            string sp, sa, st;
            getline(ss, sp, '#'); getline(ss, sa, '#'); getline(ss, st, '#');
            if (st == "Paid")   revenue += stod(sa);
            else                unpaid  += stod(sa);
        }
    }

    printDivider('=', 60, BLUE);
    cout<< BOLD<<BLUE <<"  SYSTEM DASHBOARD\n" << RESET;
    printDivider('-', 60);
    cout<<"  "<<CYAN <<left<<setw(28)<<"Patients"<< RESET <<": "<<BOLD<<totalPatients << RESET <<"\n"
         <<"  "<<CYAN<<left<<setw(28)<<"Doctors"<< RESET <<": "<<BOLD<<totalDoctors << RESET <<"\n"
         <<"  "<<CYAN <<left << setw(28)<<"Appointments"<< RESET <<": "<<BOLD<<totalAppointments<< RESET << "\n"
         <<"  "<< CYAN << left << setw(28)<<"Treatment Records"<< RESET <<": "<<BOLD<< totalTreatments   << RESET << "\n";
    printDivider('-', 60);
    cout<<"  "<<GREEN <<left<<setw(28)<<"Total Revenue (Paid)"<<RESET<<": "<<GREEN<<BOLD<< "Rs. "<<fixed<<setprecision(2) << revenue << RESET << "\n"
         <<"  " <<RED << left << setw(28) << "Outstanding (Unpaid)"<<RESET <<": " <<RED  << BOLD<< "Rs. "<<fixed<<setprecision(2)<<unpaid << RESET<< "\n";
    printDivider('=', 60, BLUE);
    pressEnter();
}

static void showMainMenu() {
    int choice;
    do {
        printHeader("Hospital Management System - Main Menu");
        cout <<"\n"
             <<"  "<<CYAN<<"1."<<RESET<<"  Patient Management\n"
             <<"  "<<CYAN<<"2."<<RESET<<"  Doctor Management\n"
             <<"  "<<CYAN<<"3."<<RESET<<"  Appointment Scheduling\n"
             <<"  "<<CYAN<<"4."<<RESET<<"  Treatment Records\n"
             <<"  "<<CYAN<<"5."<<RESET<<"  Billing\n"
             <<"  "<<CYAN<<"6."<<RESET<<"  Search & Reports\n"
             <<"  "<<YELLOW << "7."<<RESET<< "  Weekly Doctor Schedule\n"
             <<"  "<<RED<<"8."<< RESET << "  Exit\n"
             <<"\n  Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: patientMenu();     break;
            case 2: doctorMenu();      break;
            case 3: appointmentMenu(); break;
            case 4: treatmentMenu();   break;
            case 5: billingMenu();     break;
            case 6: searchMenu();      break;
            case 7: bonusMenu();       break;
            case 8:
                cout<<"\n"<<GREEN<<"  Goodbye! Freeing memory and exiting.\n" << RESET <<"\n";
                break;
            default:
                cout<<RED<<"  Invalid choice. Please enter 1-8.\n" << RESET;
        }
    } while (choice != 8);
}

int main() {
    cleanFile();

    allocateDynamicMemory();

    if (!login()) {
        freeDynamicMemory();
        return 1;
    }

    showDashboard();

    showMainMenu();

    freeDynamicMemory();

    return 0;
}
