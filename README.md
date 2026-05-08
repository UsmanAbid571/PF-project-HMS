# Hospital Management System

## Project Overview
A comprehensive C++ console-based hospital management system designed to manage patients, doctors, and appointments efficiently.

## Features

### Patient Management
- Add new patients with personal and medical information
- Update patient records
- View all patients or search by ID
- Manage patient account balance
- Delete patient records

### Doctor Management
- Register doctors with specialization
- Update doctor information
- View all doctors or search by ID
- Delete doctor records

### Appointment Management
- Schedule appointments between patients and doctors
- Check patient balance before appointment confirmation
- Deduct consultation fees from patient balance
- View all scheduled appointments
- Cancel appointments
- Track appointment history

## Technical Stack
- **Language:** C++
- **Build System:** Standard C++ compilation
- **Platform:** Windows
- **Data Storage:** Text files

## Project Structure
```
PF proect/
├── modules/
│   ├── patient.cpp
│   ├── doctor.cpp
│   ├── appointment.cpp
│   ├── cleaning.cpp
│   └── ...
├── headers/
│   ├── patient.h
│   ├── doctor.h
│   ├── appointment.h
│   ├── cleaning.h
│   ├── globals.h
│   └── ...
├── main.cpp
└── README.md
```

## How to Use
1. Compile the project using a C++ compiler
2. Run the executable
3. Navigate through the menu system to access features:
   - Patient Management
   - Doctor Management
   - Appointment Management

## File Format
Data is stored in text files with `#` as delimiter:
- **Appointments:** `patientId#doctorId#date#time`
- **Patients:** ID, name, contact, balance, etc.
- **Doctors:** ID, name, specialization, contact, etc.

## Notes
- Patient balance is required before scheduling appointments
- Consultation fees are deducted automatically upon appointment scheduling
- Date format: YYYY-MM-DD
- Time format: HH:MM AM/PM
