#include "../headers/appointment.h"
#include "../headers/patient.h"
#include "../headers/doctor.h"
#include "../headers/globals.h"
#include<cstring>

static bool parseLine(const string& line, Appointment& a) {
    if(line.empty()) return false;
    istringstream ss(line);
    string pid,did,date,time;

    if(!getline(ss,pid,'#')) return false;
    if(!getline(ss,did,'#')) return false;
    if(!getline(ss,date,'#')) return false;
    if(!getline(ss,time,'#')) return false;

    a.patientId = stoi(pid);
    a.doctorId = stoi(did);
    strncpy(a.date, date.c_str(), sizeof(a.date)-1);
    strncpy(a.time, time.c_str(), sizeof(a.time)-1);
    return true;
}

static void writeLine(ofstream& f, const Appointment& a){
    f << a.patientId << "#" << a.doctorId << "#" << a.date << "#" << a.time << "\n";
}

static int loadAll(Appointment* arr, int maxSize){
    ifstream f(APPOINTMENTS_FILE);
    int count =0;
    string line;
    while(getline(f,line) && count < maxSize){
        Appointment a;
        if(parseLine(line,a)){
            arr[count++] = a;
        }
    }
    return count;
}

bool checkPatientBalance(int patientId, double cost){
    Patient p= findPatientById(patientId);
    if(p.patientId == -1) return false; 
    return p.balance >= cost;
}

void scheduleAppointment(){
    printHeader("Schedule Appointment");

    Appointment a;
    string date,time;
    double consultationCost;

    cout<<"\n Patient ID: ";
    cin>>a.patientId;
    cout<<"\n Doctor ID: ";
    cin>>a.doctorId;
    cin.ignore();
    cout<<"\n Date (YYYY-MM-DD): ";
    getline(cin,date);
    cout<<"\n Time (e.g. 10:00 AM): ";
    getline(cin,time);
    cout<<"\n Consultation Cost: ";
    cin>>consultationCost;

    if(!patientExists(a.patientId)){
        cout << RED << "\n  Error: Patient ID " << a.patientId << " not found.\n" << RESET;
        pressEnter(); return;
    }
    if(!doctorExists(a.doctorId)){
        cout << RED << "\n  Error: Doctor ID " << a.doctorId << " not found.\n" << RESET;
        pressEnter(); return;
    }

    if(!checkPatientBalance(a.patientId, consultationCost)){
        Patient p= findPatientById(a.patientId);
        cout<<RED<<"\n Insufficient Balance."<<"Available Rs. "<<fixed<<setprecision(2)<<p.balance<<" Required Rs. "<<consultationCost<<RESET;
        pressEnter(); return;
    }

    Patient p= findPatientById(a.patientId);
    updatePatientBalance(a.patientId, p.balance - consultationCost);

    strncpy(a.date, date.c_str(), sizeof(a.date)-1);
    strncpy(a.time, time.c_str(), sizeof(a.time)-1);    

    ofstream f(APPOINTMENTS_FILE,ios::app);
    writeLine(f,a);
    Doctor d= findDoctorById(a.doctorId);
    cout<<GREEN<<"\n Appointment Scheduled Successfully.\n"<<"  Patient: "<<p.name <<"  |  Doctor: "<<d.name<<"  |  Date: "<<a.date<<"  |  Time: "<<a.time<<"\n  Rs. "<<fixed<<setprecision(2)<<consultationCost<<" deducted. New balance: Rs. "<<(p.balance - consultationCost)<< RESET<<"\n";
    pressEnter();
}

void cancelAppointment(){
    printHeader("Cancel Appointment");

    int pid,did;
    cout<<"\n Patient ID : ";
    cin>>pid;
    cout<<"\n Doctor ID : ";
    cin>>did;

    Appointment arr[MAX_RECORDS];
    int n=loadAll(arr, MAX_RECORDS);
    int index=-1;
    for(int i=0;i<n;i++){
        if(arr[i].patientId == pid && arr[i].doctorId == did){
            index=i; break;
        }
    }
        if (index == -1) {
        cout<<RED<<"\n  No appointment found for this patient/doctor combination.\n"<< RESET;
        pressEnter(); 
        return;
    }

    cout<< YELLOW<<"\n Appointment on "<<arr[index].date<<" at "<<arr[index].time<<" - Cancel? (y/n): "<< RESET;
    char choice; 
    cin>>choice;
    if(choice == 'y' || choice == 'Y') {
        cout<<" Cancelled.\n";
        pressEnter();
        return;
    }

    ofstream f(APPOINTMENTS_FILE);
    for(int i=0;i<n;i++){
        if(i!=index){
            writeLine(f, arr[i]);
        }
    }
    cout<<GREEN<<"\n Appointment cancelled.\n"<<RESET;
    pressEnter();
}

void viewAppointments(){
    printHeader("View Appointments");

    Appointment arr[MAX_RECORDS];
    int n=loadAll(arr,MAX_RECORDS);
    
    if(n==0){
        cout<<YELLOW<<"\n No appointments found.\n"<<RESET;
        pressEnter();
        return;
    }

    cout<<"\n Total: "<<n<<"\n\n";
    cout<<CYAN
         <<left
         <<setw(12)<<"Patient ID"
         <<setw(22)<<"Patient Name"
         <<setw(10)<<"Doctor ID"
         <<setw(22)<<"Doctor Name"
         <<setw(14)<<"Date"
         <<setw(10)<<"Time"
         <<RESET<<"\n";
    printDivider('-', 90);

    for(int i=0;i<n;i++){
        Patient p= findPatientById(arr[i].patientId);
        Doctor d= findDoctorById(arr[i].doctorId);
        cout<<left
            <<setw(12)<<arr[i].patientId
            <<setw(22)<<p.name
            <<setw(10)<<arr[i].doctorId
            <<setw(22)<<d.name
            <<setw(14)<<arr[i].date
            <<setw(10)<<arr[i].time
            <<"\n";
    }
    printDivider('-', 90);
    pressEnter();
}

void appointmentMenu(){
    int choice=0;
    do{
        printHeader("Appointment Management");
        cout<<"\n 1. Schedule Appointment";
        cout<<"\n 2. View Appointments";
        cout<<"\n 3. Cancel Appointment";
        cout<<"\n 4. Back to Main Menu";
        cout<<"\n\n Enter your choice: ";
        cin>>choice;
        cin.ignore();

        switch(choice){
            case 1: scheduleAppointment(); break;
            case 2: viewAppointments(); break;
            case 3: cancelAppointment(); break;
            case 4: return;
            default: cout<<RED<<"\n Invalid choice. Try again.\n"<<RESET; pressEnter();
        }
    }while(choice!=4);
}

