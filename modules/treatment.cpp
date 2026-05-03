#include"../headers/globals.h"
#include"../headers/treatment.h"
#include"../headers/doctor.h"
#include"../headers/patient.h"

static bool parseLine(const string& line, Treatment& t) {
    if(line.empty()) return false;
    istringstream ss(line);
    string tok;

    if(!getline(ss,tok,'#')) return false; t.patientId = stoi(tok);
    if(!getline(ss,t.description,'#')) return false;
    if(!getline(ss,tok,'#')) return false; t.cost = stod(tok);
    if(!getline(ss,tok,'#')) t.paid = false;
    else t.paid = (tok == "true" || tok == "1");
    return true;
}

static void writeLine(ofstream& f, const Treatment& t){
    f << t.patientId << "#" << t.description << "#" << fixed << setprecision(2) << t.cost << "#" << (t.paid ? "true" : "false") << "\n";
}

static int loadAll(Treatment* arr, int maxSize){
    ifstream f(TREATMENTS_FILE);
    int count =0;
    string line;
    while(getline(f,line) && count < maxSize){
        Treatment t;
        if(parseLine(line,t)){
            arr[count++] = t;
        }
    }
    return count;
}

void addTreatment(){
    printHeader("Add Treatment Record");

    Treatment t;
    cout<<"\n Patient ID: ";
    cin>>t.patientId;
    cin.ignore();
    cout<<"\n Description: ";
    getline(cin,t.description);
    cout<<"\n Cost: Rs. ";
    cin>>t.cost;
    t.paid = false;

    if(!patientExists(t.patientId)){
        cout << RED << "\n  Error: Patient ID " << t.patientId << " not found.\n" << RESET;
        pressEnter(); return;
    }
    if(t.description.empty() || t.cost < 0){
        cout << RED << "\n  Error: Invalid Description or Cost.\n" << RESET;
        pressEnter(); return;
    }

    ofstream tf(TREATMENTS_FILE, ios::app);
    writeLine(tf,t);
    
    ofstream bf(BILLS_FILE, ios::app);
    bf<< t.patientId << "#" << fixed << setprecision(2) << t.cost << "#Unpaid\n";

    Patient p = findPatientById(t.patientId);
    cout<<GREEN<<"\n Treatment added successfully.\n  Patient: "<<p.name<<" - Rs. "<<t.cost<<" (Unpaid)\n"<<RESET;
    pressEnter();
}

void viewTreatments(){
    printHeader("View Treatments");
    int pid;
    cout<<"\n Enter Patient ID (0 = all):  ";
    cin>>pid;

    Treatment arr[MAX_RECORDS];
    int n=loadAll(arr,MAX_RECORDS);

    cout<<"\n"<<CYAN<<left
    <<setw(12)<<"PatientID"
    <<setw(30)<<"Description"
    <<setw(14)<<"Cost"
    <<setw(10)<<"Status"
    <<RESET<<"\n";
    printDivider('-', 66);

    int shown = 0;
    for(int i=0;i<n;i++){
        if(pid == 0 || arr[i].patientId != pid) continue;
        cout<<left
            <<setw(12)<<arr[i].patientId
            <<setw(30)<<arr[i].description
            <<setw(14)<<fixed<<setprecision(2)<<arr[i].cost
            <<setw(10)<<(arr[i].paid ? "Paid" : "Unpaid")<<"\n";
        shown++; 
    }
    if(shown==0)
        cout<<YELLOW<<"\n No treatments found.\n"<<RESET;
    printDivider('-', 66);
    pressEnter();
}

void viewTreatmentByDoctor(){
    printHeader("Treatments by Doctor");
    int docId;
    cout<<"\n Enter Doctor ID:  ";
    cin>>docId;
    if(!doctorExists(docId)){
        cout << RED << "\n  Error: Doctor ID " << docId << " not found.\n" << RESET;
        pressEnter(); return;
    }

    ifstream af(APPOINTMENTS_FILE);
    int patIds[MAX_RECORDS];
    int patCount = 0;
    string line;
    while(getline(af,line)){
        if(line.empty()) continue;
        istringstream ss(line);
        string pid,did;
        int p,d;
        if(!getline(ss,pid,'#')) continue;
        if(!getline(ss,did,'#')) continue;
        p=stoi(pid);
        d=stoi(did);
        if(d==docId) patIds[patCount++]=p;
    }
    if(patCount==0){
        cout << YELLOW << "\n  No appointments found for this doctor.\n" << RESET;
        pressEnter(); return;
    }

    Doctor doc = findDoctorById(docId);
    cout<<"\n Treatments for patients of Dr. "<<doc.name<<": \n\n";

    Treatment arr[MAX_RECORDS];
    int n=loadAll(arr,MAX_RECORDS);

    cout<<CYAN<<left
    <<setw(12)<<"PatientID"
    <<setw(22)<<"Patient Name"
    <<setw(28)<<"Description"
    <<setw(12)<<"Cost"
    <<setw(10)<<"Status"
    <<RESET<<"\n";
    printDivider('-', 84);

    bool any=false;
    for(int i=0;i<n;i++){
        for(int j=0;j<patCount;j++){
            if(arr[i].patientId == patIds[j]){
                Patient p = findPatientById(arr[i].patientId);
                cout<<left
                    <<setw(12)<<arr[i].patientId
                    <<setw(22)<<(p.patientId != -1 ? p.name : "Unknown")
                    <<setw(28)<<arr[i].description
                    <<setw(12)<<fixed<<setprecision(2)<<arr[i].cost
                    <<setw(10)<<(arr[i].paid ? "Paid" : "Unpaid")<<"\n";
                any=true;
                break;
            }
        }
    }
    if(!any){
        cout << YELLOW << "\n  No treatments found for this doctor.\n" << RESET;
    }
    printDivider('-', 84);
    pressEnter();
}

void treatmentMenu(){
    int choice=0;
    do{
        printHeader("Treatment Management");
        cout<<"\n 1. Add Treatment Record";
        cout<<"\n 2. View Treatments (by Patient)";
        cout<<"\n 3. View Treatments (by Doctor)";
        cout<<"\n 4. Back to Main Menu";
        cout<<"\n\n Enter choice: ";
        cin>>choice;
        switch(choice){
            case 1: addTreatment(); break;
            case 2: viewTreatments(); break;
            case 3: viewTreatmentByDoctor(); break;
            case 4: return;
            default:
                cout << RED << "\n  Invalid choice. Please try again.\n" << RESET;
                pressEnter();
        }
    }while(choice != 4);
}