#include"../headers/globals.h"
#include"../headers/patient.h"
#include"../headers/cleaning.h"


static bool parseLine(const string& line, Patient& p) {
    if(line.empty()) return false;
    istringstream ss(line);
    string tok;

    if(!getline(ss,tok,'#')) return false; 
    p.patientId = stoi(tok);
    if(!getline(ss,p.name,'#')) return false;
    if(!getline(ss,tok,'#')) return false; 
    p.age = stoi(tok);
    if(!getline(ss,p.gender,'#')) return false; 
    if(!getline(ss,p.contact,'#')) return false; 
    if(!getline(ss,tok,'#')) p.balance=0.0;
    else p.balance = stod(tok); 
    return true;
}

static void writeLine(ofstream& f, const Patient& p){
    f<<p.patientId<<"#"<<p.name<<"#"<<p.age<<"#"<<p.gender<<"#"<<p.contact<<"#"<<fixed<<setprecision(2)<<p.balance<<endl;
}

int loadAllPatients(Patient* arr, int maxSize){
    ifstream f(PATIENTS_FILE);
    int count =0;
    string line;
    while(getline(f,line) && count < maxSize){
        Patient p;
        if(parseLine(line,p)){
            arr[count++] = p;
        }
    }
    return count;
}

bool patientExists(int patientId){
    Patient arr[MAX_RECORDS];
    int n=loadAllPatients(arr, MAX_RECORDS);
    for(int i=0;i<n;i++){
        if(arr[i].patientId == patientId){
            return true;
        }
    }
    return false;
}

Patient findPatientById(int patientId){
    Patient arr[MAX_RECORDS];
    int n=loadAllPatients(arr, MAX_RECORDS);
    for(int i=0;i<n;i++){
        if(arr[i].patientId == patientId){
            return arr[i];
        }
    }
    Patient empty{};
    empty.patientId = -1;
    return empty;
}

void updatePatientBalance(int patientId, double newBalance){
    Patient arr[MAX_RECORDS];
    int n=loadAllPatients(arr, MAX_RECORDS);
    for(int i=0;i<n;i++){
        if(arr[i].patientId == patientId){
            arr[i].balance = newBalance;
            break;
        }
    }
    ofstream f(PATIENTS_FILE);
    for(int i=0;i<n;i++){
        writeLine(f, arr[i]);
    }
}

static void printTableHeader(){
    cout<<CYAN
        <<left
        <<setw(6)<<"ID"
        <<setw(22)<<"Name"
        <<setw(6)<<"Age"
        <<setw(10)<<"Gender"
        <<setw(14)<<"Contact"
        <<setw(12)<<"Balance (Rs.)"
        <<RESET<<"\n";
    printDivider('-', 70);
}

static void printPatientRow(const Patient& p){
    cout<<left
        <<setw(6)<<p.patientId
        <<setw(22)<<p.name
        <<setw(6)<<p.age
        <<setw(10)<<p.gender
        <<setw(14)<<p.contact
        <<right<<setw(12)<<fixed<<setprecision(2)<<p.balance
        <<"\n";
}

void addPatient(){
    printHeader("Add New Patient");
    Patient p;
    cout<<"\n Patient ID: ";
    cin>>p.patientId;
    cin.ignore();
    cout<<"\n Full Name: ";
    getline(cin, p.name);
    cout<<" Age: ";
    cin>>p.age;
    cin.ignore();
    cout<<" Gender (Male/Female): ";
    getline(cin, p.gender);
    cout<<" Contact: ";
    getline(cin, p.contact);
    cout<<" Initial Balance (Rs.): ";
    cin>>p.balance;

    if(p.patientId<=0){
        cout<<RED<<"\n Invalid Patient ID. Must be a positive number.\n"<<RESET;
        pressEnter();
        return;
    }
    if(p.name.empty()){
        cout<<RED<<"\n Name cannot be empty.\n"<<RESET;
        pressEnter();
        return;
    }
    if(!isValidContact(p.contact)){
        cout<<RED<<"\n Invalid contact number. Must be 11 digits.\n"<<RESET;
        pressEnter();
        return;
    }
    if(patientExists(p.patientId)){
        cout<<RED<<"\n Patient ID"<<p.patientId<<" already exists. Try a different ID.\n"<<RESET;
        pressEnter();
        return;
    }

    ofstream f(PATIENTS_FILE, ios::app);
    writeLine(f, p);
    cout<<GREEN<<"\n Patient added successfully!\n"<<RESET;
    pressEnter();
}

void updatePatient(){
    printHeader("Update Patient");

    int id;
    cout<<"\n Enter Patient ID to update: ";
    cin>>id;

    Patient arr[MAX_RECORDS];
    int n=loadAllPatients(arr, MAX_RECORDS);
    int index=-1;
    for(int i=0;i<n;i++){
        if(arr[i].patientId == id){
            index=i; break;
        }
    }
    if(index==-1){
        cout<<RED<<"\n Patient ID "<<id<<" not found.\n"<<RESET;
        pressEnter();
        return;
    }

    cout<<"\n What do you want to update?"
        <<"\n 1. Name"
        <<"\n 2. Age"
        <<"\n 3. Gender"
        <<"\n 4. Contact"
        <<"\n 5. Balance (Rs.)"
        <<"\n\n Enter your choice: ";
    int choice; 
    cin>>choice;
    cin.ignore();

    switch(choice){
        case 1:
            cout<<"\n New Name: ";
            getline(cin, arr[index].name);
            break;
        case 2:
            cout<<"\n New Age: ";
            cin>>arr[index].age;
            cin.ignore();
            break;
        case 3:
            cout<<"\n New Gender (Male/Female): ";
            getline(cin, arr[index].gender);
            break;
        case 4: {
            string c;
            cout<<"\n New Contact: ";
            getline(cin, c);
            if(!isValidContact(c)){
                cout<<RED<<"\n Invalid contact number. Must be 11 digits.\n"<<RESET;
                pressEnter();
                return;
            }
            arr[index].contact = c;
            break;
        } 
        case 5:
            cout<<"\n New Balance (Rs.): ";
            cin>>arr[index].balance;
            break;
        default: cout<<RED<<"\n Invalid choice. Try again.\n"<<RESET; pressEnter();
    }
    ofstream f(PATIENTS_FILE);
    for(int i=0;i<n;i++){
        writeLine(f, arr[i]);
    }
    cout<<GREEN<<"\n Patient updated successfully!\n"<<RESET;
    pressEnter();
}

void deletePatient(){
    printHeader("Delete Patient");

    int id;
    cout<<"\n Enter Patient ID to delete: ";
    cin>>id;
    Patient arr[MAX_RECORDS];
    int n=loadAllPatients(arr, MAX_RECORDS);
    int index=-1;
    for(int i=0;i<n;i++){
        if(arr[i].patientId == id){
            index=i; 
            break;
        }
    }
    if(index==-1){
        cout<<RED<<"\n Patient ID "<<id<<" not found.\n"<<RESET;
        pressEnter();
        return;
    }
    cout<<YELLOW<<"\n Confirm delete patient \""<<arr[index].name<<"\" (y/n): "<<RESET;
    char confirm;
    cin>>confirm;
    if(confirm != 'y' && confirm != 'Y'){
        cout<<"Cancelled.\n";
        pressEnter();
        return;
    }

    ofstream f(PATIENTS_FILE);
    for(int i=0;i<n;i++){
        if(i!=index){
            writeLine(f, arr[i]);
        }
    }
    cout<<GREEN<<"\n Patient deleted successfully!\n"<<RESET;
    pressEnter();
}

void viewPatients(){
    printHeader("All Patients");
    Patient arr[MAX_RECORDS];
    int n=loadAllPatients(arr, MAX_RECORDS);

    if(n==0){
        cout<<YELLOW<<"\n No patient records found.\n"<<RESET;
        pressEnter();
        return;
    }
    cout<<"\n Total Patients: "<<n<<"\n";
    printTableHeader();
    for(int i=0;i<n;i++){
        printPatientRow(arr[i]);
    }
    printDivider('-', 70);
    pressEnter();
}

void patientMenu(){
    int choice;
    do{
        printHeader("Patient Management");
        cout<<"\n 1. Add Patient"
            <<"\n 2. Update Patient"
            <<"\n 3. Delete Patient"
            <<"\n 4. View All Patients"
            <<"\n 5. Back to Main Menu"
            <<"\n\n Enter your choice: ";
        cin>>choice;
        switch(choice){
            case 1: addPatient(); break;
            case 2: updatePatient(); break;
            case 3: deletePatient(); break;
            case 4: viewPatients(); break;
            case 5: cout<<"\n Returning to main menu...\n"; break;
            default: cout<<RED<<"\n Invalid choice. Try again.\n"<<RESET; pressEnter();
        }
    }while(choice!=5);
 }
