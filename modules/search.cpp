#include"../headers/search.h"
#include"../headers/patient.h"
#include"../headers/doctor.h"
#include"../headers/globals.h"

void searchPatientById() {
    printHeader("Search Patient by ID");

    int id;
    cout<<"\n Enter Patient ID: ";
    cin>>id;

    Patient p = findPatientById(id);
    if(p.patientId == -1){
        cout<<RED<<"\n No patient found with this ID.\n"<<RESET;
        pressEnter();
        return;
    }
    cout<<"\n"<<GREEN<<"  Record found:\n" <<RESET;
    printDivider('-', 60);
    cout <<"  ID       : "<<p.patientId<<"\n"
         <<"  Name     : "<<p.name   <<"\n"
         <<"  Age      : "<<p.age    <<"\n"
         <<"  Gender   : "<<p.gender<<"\n"
         <<"  Contact  : "<<p.contact<<"\n"
         <<"  Balance  : Rs.  "<<fixed<< setprecision(2)<<p.balance<<"\n";
    printDivider('-', 60);
    pressEnter();
}

void searchPatientByName() {
    printHeader("Search Patient by Name");

    string name;
    cout<<"\n Enter Patient Name: ";
    cin.ignore();
    getline(cin,name);

    string lowerName = toLower(name);
    Patient arr[MAX_RECORDS];
    int n=loadAllPatients(arr, MAX_RECORDS);
        cout << "\n" << CYAN
         <<left<<setw(6)  << "ID"
         <<setw(22)<<"Name"
         <<setw(6) <<"Age"
         <<setw(10)<<"Gender"
         <<setw(14)<<"Contact"
         <<setw(12)<<"Balance"
         << RESET << "\n";
    printDivider('-', 70);

    int found=0;
    for(int i=0;i<n;i++){
        if(toLower(arr[i].name).find(lowerName) != string::npos){
            cout<<left
                 <<setw(6) <<arr[i].patientId
                 <<setw(22)<<arr[i].name
                 <<setw(6) <<arr[i].age
                 <<setw(10)<<arr[i].gender
                 <<setw(14)<<arr[i].contact
                 <<right<<setw(12)<<fixed<<setprecision(2)<<arr[i].balance<<"\n";
            found++;
        }
    }
    printDivider('-', 70);
    if(found == 0){
        cout<<YELLOW<<"\n No patient found with this name.\n"<<RESET;
    }
    else{
        cout<<GREEN<<" "<<found<<" record(s) found.\n"<<RESET;
    }
    pressEnter();
}

void searchDoctorById() {
    printHeader("Search Doctor by ID");

    int id;
    cout<<"\n Enter Doctor ID: ";
    cin>>id;

    Doctor d = findDoctorById(id);
    if (d.doc_id==-1)
    {
        cout<<RED<<"\n Doctor ID "<<id<<" not found.\n"<<RESET;
        pressEnter();
        return;
    }

    cout<<"\n"<<GREEN<<"  Record found:\n" <<RESET;
    printDivider('-', 50);
    cout<<"  ID       : "<<d.doc_id<<"\n"
        <<"  Name     : "<<d.name<<"\n"
        <<"  Speciality: "<<d.specialty<<"\n"
        <<"  Experience  : "<<d.experience<<" yrs"<<"\n";
    printDivider('-', 50);
    pressEnter();
}

void searchDoctorBySpecialty(){
    printHeader("Search Doctor by Speciality");

    string query;
    cout<<"\n Enter Speciality: ";
    cin.ignore();
    getline(cin,query);
    string lowerQuery = toLower(query);

    Doctor arr[MAX_RECORDS];
    int n=loadAllDoctors(arr, MAX_RECORDS);
    cout << "\n" << CYAN
         <<left<<setw(6)  << "ID"
         <<setw(22)<<"Name"
         <<setw(14)<<"Speciality"
         <<setw(10)<<"Experience"
         << RESET << "\n";
    printDivider('-', 62);

    int found=0;
    for(int i=0;i<n;i++){
        if(toLower(arr[i].specialty).find(lowerQuery) != string::npos){
            cout<<left
                 <<setw(6) <<arr[i].doc_id
                 <<setw(22)<<arr[i].name
                 <<setw(14)<<arr[i].specialty
                 <<right<<setw(10)<<arr[i].experience<<" yrs\n";
            found++;
        }
    }
    printDivider('-', 62);
    if(found == 0){
        cout<<YELLOW<<"  No doctors found with specialty \""<<query<<"\".\n"<<RESET;
    }
    else{
        cout<<GREEN<<" "<<found<<" record(s) found.\n"<<RESET;
    }
    pressEnter();
}

void generateReport(){
    printHeader("System Summary Report");

    int totalPatients= countLines(PATIENTS_FILE);
    int totalDoctors = countLines(DOCTORS_FILE);
    int totalAppointments = countLines(APPOINTMENTS_FILE);

    double totalRevenue = 0.0, totalUnpaid = 0.0;
    int unpaidCount = 0, paidCount = 0;
    {
        ifstream f(BILLS_FILE);
        string line;
        while(getline(f, line)){
            if(line.empty()) continue;
            istringstream ss(line);
            string sp,sa,status;
            if(!getline(ss,sp,'#')) continue;
            if(!getline(ss,sa,'#')) continue;
            if(!getline(ss,status,'#')) status = "Unpaid";
            double amount = stod(sa);
            if(status == "Paid"){
                totalRevenue += amount;
                paidCount++;
            }
            else{
                totalUnpaid += amount;
                unpaidCount++;
            }
    }
}
    printDivider('=', 55);
    cout<<BOLD <<BLUE<<"\tHOSPITAL MANAGEMENT SYSTEM - REPORT\n"<<RESET;
    printDivider('=', 55);
    cout<<"\n"
         <<"  "<< CYAN <<left <<setw(30)<<"Total Patients"<<RESET<<": "<<BOLD<< totalPatients     << RESET<< "\n"
         <<"  "<< CYAN <<left <<setw(30)<<"Total Doctors"<<RESET<<": "<<BOLD<< totalDoctors      <<RESET << "\n"
         <<"  "<< CYAN <<left<<setw(30)<<"Total Appointments"<<RESET<<": "<<BOLD<< totalAppointments<<RESET<< "\n"
         <<"\n";
    printDivider('-', 55);
    cout <<"  " <<GREEN<<left<<setw(30)<<"Total Revenue (Paid)"<<RESET
         <<": " <<GREEN<<BOLD<<"Rs. "<<fixed << setprecision(2)<<totalRevenue<<RESET
         <<"  ("<<paidCount <<" bill(s))\n"
         <<"  " <<RED<<left<< setw(30)<<"Outstanding (Unpaid)"<<RESET
         <<": " <<RED<<BOLD<<"Rs. " <<fixed<<setprecision(2)<<totalUnpaid<<RESET
         <<"  (" <<unpaidCount<<" bill(s))\n";
    printDivider('=', 55);
    pressEnter();
}

void searchMenu(){
    int choice=0;
    do{
        printHeader("Search & Reports");
        cout<<"\n 1. Search Patient by ID";
        cout<<"\n 2. Search Patient by Name";
        cout<<"\n 3. Search Doctor by ID";
        cout<<"\n 4. Search Doctor by Speciality";
        cout<<"\n 5. Generate System Report";
        cout<<"\n 6. Back to Main Menu";
        cout<<"\n\n Enter your choice: ";
        cin>>choice;

        switch(choice){
            case 1: searchPatientById(); break;
            case 2: searchPatientByName(); break;
            case 3: searchDoctorById(); break;
            case 4: searchDoctorBySpecialty(); break;
            case 5: generateReport(); break;
            case 6: cout<<GREEN<<"\n Returning to Main Menu...\n"<<RESET; pressEnter(); break;
            default: cout<<RED<<"\n Invalid choice. Please try again.\n"<<RESET; pressEnter();
        }
    }while(choice != 6);
}