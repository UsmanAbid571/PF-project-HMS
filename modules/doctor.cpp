#include "../headers/globals.h"
#include "../headers/doctor.h"

static bool parseLine(const string& line, Doctor& d) {
    if (line.empty()) return false;
    istringstream ss(line);
    string tok;
    if (!getline(ss, tok,'#')) return false; 
    d.doc_id= stoi(tok);
    if (!getline(ss, d.name, '#')) return false;
    if (!getline(ss, d.specialty,'#')) return false;
    if (!getline(ss, tok,'#')) return false; 
    d.experience= stoi(tok);
    return true;
}

static void writeLine(ofstream& f, const Doctor& d) {
    f<<d.doc_id<<"#"<<d.name<<"#"<<d.specialty<<"#"<<d.experience<<"\n";
}

int loadAllDoctors(Doctor* arr, int maxSize) {
    ifstream f(DOCTORS_FILE);
    int count=0;
    string line;
    while(getline(f, line) && count < maxSize) {
        Doctor d;
        if(parseLine(line, d)) 
            arr[count++] = d;
    }
    return count;
}

bool doctorExists(int id) {
    Doctor arr[MAX_RECORDS];
    int n=loadAllDoctors(arr, MAX_RECORDS);
    for(int i= 0;i <n;i++)
        if (arr[i].doc_id == id) return true;
    return false;
}

Doctor findDoctorById(int id) {
    Doctor arr[MAX_RECORDS];
    int n=loadAllDoctors(arr, MAX_RECORDS);
    for (int i= 0;i<n;i++)
        if(arr[i].doc_id == id) return arr[i];
    Doctor empty{}; empty.doc_id = -1;
    return empty;
}

void sortDoctorsByExperience(Doctor* arr, int count) {
    for(int i =0;i<count -1;i++) {
        for(int j=0;j<count - i - 1;j++) {
            if (arr[j].experience > arr[j+1].experience) {
                Doctor tmp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=tmp;
            }
        }
    }
}

static void printTableHeader() {
    cout<<CYAN
        <<left
        <<setw(8) <<"ID"
        <<setw(22)<<"Name"
        <<setw(18)<<"Specialty"
        <<setw(12)<<"Experience"
        <<RESET << "\n";
    printDivider('-', 60);
}

static void printDoctorRow(const Doctor& d) {
    cout<<left
        <<setw(8) <<d.doc_id
        <<setw(22)<<d.name
        <<setw(18)<<d.specialty
        <<setw(12)<<(to_string(d.experience) + " yrs")
        <<"\n";
}

void addDoctor() {
    printHeader("Add New Doctor");

    Doctor d;
    cout<<"\n  Doctor ID         : "; 
    cin>>d.doc_id;
    cin.ignore();
    cout<<"  Full Name         : "; 
    getline(cin, d.name);
    cout<<"  Specialty         : "; 
    getline(cin, d.specialty);
    cout<<"  Years Experience  : "; 
    cin>>d.experience;

    if (d.doc_id <= 0 || d.name.empty() || d.specialty.empty() || d.experience < 0) {
        cout << RED << "\n  Error: All fields are required and must be valid.\n" << RESET;
        pressEnter(); return;
    }
    if (doctorExists(d.doc_id)) {
        cout << RED << "\n  Error: Doctor ID " << d.doc_id << " already exists.\n" << RESET;
        pressEnter(); return;
    }

    ofstream f(DOCTORS_FILE, ios::app);
    writeLine(f, d);
    cout<<GREEN<<"\n  Doctor added successfully!\n"<<RESET;
    pressEnter();
}

void updateDoctor() {
    printHeader("Update Doctor");

    int id;
    cout<<"\n  Enter Doctor ID to update: "; 
    cin>>id;

    Doctor arr[MAX_RECORDS];
    int n=loadAllDoctors(arr, MAX_RECORDS);
    int idx = -1;
    for(int i = 0; i < n; i++)
        if (arr[i].doc_id == id) { idx = i; break; }

    if (idx == -1) {
        cout<<RED<< "\n  Error: Doctor ID " << id << " not found.\n" << RESET;
        pressEnter(); return;
    }

    cout<<"\n  1. Name\n  2. Specialty\n  3. Experience\n  Choice: ";
    int c; 
    cin>>c;
    cin.ignore();
    switch(c) {
        case 1: 
            cout<<"  New Name       : "; 
            getline(cin, arr[idx].name);      
            break;
        case 2: 
            cout<<"  New Specialty  : "; 
            getline(cin, arr[idx].specialty); 
            break;
        case 3: 
            cout<<"  New Experience : "; 
            cin>>arr[idx].experience;        
            break;
        default: 
            cout<<RED<<"  Invalid.\n" << RESET; 
            pressEnter(); 
            return;
    }

    ofstream f(DOCTORS_FILE);
    for (int i = 0; i < n; i++) writeLine(f, arr[i]);
    cout << GREEN << "\n  Doctor updated successfully!\n" << RESET;
    pressEnter();
}

void deleteDoctor() {
   printHeader("Delete Doctor");

    int id;
    cout<<"\n Enter Doctor ID to delete: ";
    cin>>id;

    Doctor arr[MAX_RECORDS];
    int n=loadAllDoctors(arr,MAX_RECORDS);
    int idx=-1;

    for(int i=0;i<n;i++)
        if(arr[i].doc_id==id){idx=i;break;}

    if(idx==-1){
        cout<<RED<<"\n Error: Doctor ID "<<id<<" not found.\n"<<RESET;
        pressEnter();
        return;
    }

    cout<<YELLOW<<"\n Confirm delete Dr. \""<<arr[idx].name<<"\"? (y/n): "<<RESET;
    char confirm;
    cin>>confirm;

    if(confirm!='y'&&confirm!='Y'){
        cout<<" Cancelled.\n";
        pressEnter();
        return;
    }

    ofstream f(DOCTORS_FILE);

    for(int i=0;i<n;i++)
        if(i!=idx) writeLine(f,arr[i]);

    cout<<GREEN<<"\n Doctor deleted successfully!\n"<<RESET;
    pressEnter();
}

void viewDoctors() {
    printHeader("All Doctors (Sorted by Experience)");

    Doctor arr[MAX_RECORDS];
    int n = loadAllDoctors(arr, MAX_RECORDS);

    if (n == 0) {
        cout << YELLOW << "\n  No doctor records found.\n" << RESET;
        pressEnter(); return;
    }

    sortDoctorsByExperience(arr, n);
    cout << "\n  Total records: " << n << "\n\n";
    printTableHeader();
    for (int i = 0; i < n; i++) printDoctorRow(arr[i]);
    printDivider('-', 60);
    pressEnter();
}

void doctorMenu() {
    int choice;
    do {
        printHeader("Doctor Management");
        cout<<"\n"
            <<"  1. Add Doctor\n"
            <<"  2. Update Doctor\n"
            <<"  3. Delete Doctor\n"
            <<"  4. View All Doctors (Sorted by Experience)\n"
            <<"  0. Back to Main Menu\n"
            <<"\n  Choice: ";
        cin>>choice;

        switch (choice) {
            case 1: addDoctor();  break;
            case 2: updateDoctor(); break;
            case 3: deleteDoctor(); break;
            case 4: viewDoctors();  break;
            case 0: break;
            default: cout<<RED<<"  Invalid choice.\n"<<RESET;
        }
    } while (choice != 0);
}
