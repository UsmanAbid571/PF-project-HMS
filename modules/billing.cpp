#include"../headers/globals.h"
#include"../headers/billing.h"
#include"../headers/patient.h"
#include"../headers/doctor.h"

struct Bill {
    int patientId;
    double amount;
    string status;
};

static bool parseLine(const string& line, Bill& b) {
    if(line.empty()) return false;

    istringstream ss(line);
    string tok;

    if(!getline(ss,tok,'#')) return false;
    b.patientId = stoi(tok);

    if(!getline(ss,tok,'#')) return false;
    b.amount = stod(tok);

    if(!getline(ss,tok,'#')) return false;
    b.status = tok;   // ✅ FIXED

    return true;
}

static void writeBill(ofstream& f, const Bill& b){
    f <<b.patientId<<"#"<<fixed<<setprecision(2)<<b.amount<< "#" << b.status << "\n";
}

static int loadBills(Bill* arr, int maxSize){
    ifstream f(BILLS_FILE);
    int count =0;
    string line;
    while(getline(f,line) && count < maxSize){
        Bill b;
        if(parseLine(line,b)){
            arr[count++] = b;
        }
    }
    return count;
}

void updatePayment(){
    printHeader("Update Payment");

    int pid;
    cout<<"\n Enter Patient ID: ";
    cin>>pid;
    cin.ignore();

    Bill arr[MAX_RECORDS];
    int n=loadBills(arr, MAX_RECORDS);
    cout << "\n  Unpaid bills for Patient " << pid << ":\n";
    cout << CYAN << left << setw(5) << "#" << setw(12) << "Amount" << setw(10) << "Status" << RESET << "\n";
    printDivider('-', 27);

    int indices[MAX_RECORDS]; 
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i].patientId==pid && arr[i].status=="Unpaid") {
            cout<<left<<setw(5)<<cnt+1
                 <<setw(12)<<fixed<<setprecision(2)<<arr[i].amount
                 <<"Unpaid\n";
            indices[cnt++] = i;
        }
    }

    if (cnt == 0) {
        cout<<YELLOW<<"  No unpaid bills found for this patient.\n" << RESET;
        pressEnter();
        return;
    }
    cout<<"\n Enter 0 to mark as Paid (1-"<<cnt<<"): ";
    int sel;
    cin>>sel;
    if(sel < 1 || sel > cnt){
        cout<<RED<<"\n  Invalid selection.\n" << RESET;
        pressEnter();
        return;
    }
    arr[indices[sel-1]].status = "Paid";

    ofstream f(BILLS_FILE,ios::trunc);
    for(int i=0;i<n;i++){
        writeBill(f,arr[i]);
    }
    cout<<GREEN<<"\n  Bill Marked as Paid.\n"<<RESET;
    pressEnter();
}

void generateBill(){
    printHeader("Generate Patient Bill");

    int pid;
    cout<<"\n Enter Patient ID: ";
    cin>>pid;

    if(!patientExists(pid)){
        cout<<RED<<"\n  Error: Patient ID "<<pid<<" not found.\n"<<RESET;
        pressEnter();
        return;
    }

    Patient p = findPatientById(pid);
    int docId = -1;
    {
        ifstream af(APPOINTMENTS_FILE);
        string line;
        while(getline(af,line)){
            if(line.empty()) continue;
            istringstream ss(line);
            string tok;
            int apPid, apDocId;
            if(!getline(ss,tok,'#')) continue; apPid = stoi(tok);
            if(!getline(ss,tok,'#')) continue; apDocId = stoi(tok);
            if(apPid == pid){
                docId = apDocId; 
                break;
            }
        }
    }
    string docName = (docId != -1) ? findDoctorById(docId).name : "N/A";

    struct TreatRow {
        string desc;
        double cost;
        bool paid;
    };
    TreatRow rows[MAX_RECORDS];
    int tCount = 0;
    double totalDue=0.0;
    {
        ifstream tf(TREATMENTS_FILE);
        string line;
        while(getline(tf,line)){
            if(line.empty()) continue;
            istringstream ss(line);
            string sp, desc, sc, spaid;
            if(!getline(ss,sp,'#')) continue;
            if(!getline(ss,desc,'#')) continue;
            if(!getline(ss,sc,'#')) continue;
            if(!getline(ss,spaid,'#')) spaid = "false";
            if(stoi(sp) == pid){
                rows[tCount] = { desc, stod(sc), (spaid == "true") };
                if(!rows[tCount].paid) totalDue += rows[tCount].cost;
                tCount++;
            }
        }
    }

    printDivider('=', 65);
    cout<<BOLD<<BLUE<<"  HOSPITAL MANAGEMENT SYSTEM — PATIENT BILL\n"<<RESET;
    printDivider('=', 65);
    cout<<"  Patient Name   : "<<BOLD<<p.name<<RESET<<"\n";
    cout<<"  Patient ID     : "<<p.patientId<<"\n";
    cout<<"  Doctor Name    : "<<docName<<"\n";
    cout<<"  Balance on file: Rs. "<<fixed<<setprecision(2)<<p.balance<<"\n";
    printDivider('-', 65);
    cout<<CYAN<<left<<setw(32)<<"Treatments\n"
        <<setw(14)<<"Cost (Rs.)"
        <<setw(12)<<"Status\n"
        <<RESET<<"\n";
    printDivider('-', 58);
    if(tCount == 0){
        cout<<YELLOW<<"  No treatments records found\n"<<RESET;
    }
    else{
        for(int i=0;i<tCount;i++){
            string status = rows[i].paid ? (GREEN+"Paid"+RESET) : (RED+"Unpaid"+RESET);
            cout<<left<<setw(32)<<rows[i].desc
                <<setw(14)<<fixed<<setprecision(2)<<rows[i].cost
                <<setw(12)<<status<<"\n";
        }
    }
    printDivider('-', 65);
    cout<<BOLD<<"  TOTAL DUE: Rs. "<<fixed<<setprecision(2)<<totalDue<<RESET<<"\n";
    printDivider('=', 65);
    pressEnter();
}

void billingMenu(){
    int choice=0;
    do{
        printHeader("Billing Management");
        cout<<"\n 1. Generate Patient Bill";
        cout<<"\n 2. Update Payment Status";
        cout<<"\n 3. Back to Main Menu";
        cout<<"\n\n Enter your choice: ";
        cin>>choice;
        switch(choice){
            case 1: generateBill(); break;
            case 2: updatePayment(); break;
            case 3: break;
            default:
                cout<<RED<<"\n Invalid choice. Please try again.\n"<<RESET;
                pressEnter();
        }
    }while(choice!=3);
}
    