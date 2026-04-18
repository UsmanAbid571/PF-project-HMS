#include"../headers/globals.h"
#include"../headers/cleaning.h"

string fixGender(string g)
{
    if(g=="M" || g=="m")
        return "Male";
    else if(g=="F" || g=="f")
        return "Female";
    else
        return g;
}

string fixDate(string d)
{
    if(d.length()==10 && d[4]=='-' && d[7]=='-'){
        string year=d.substr(0,4);
        string month=d.substr(5,2);
        string day=d.substr(8,2);
        return month+"-"+day+"-"+year;
    }
    return d;   
}

string fixTime(string t){
    if(t.find(':')!=string::npos) return t;
    
        int hour=stoi(t);
        string am_pm= (hour>=12) ? "PM" : "AM";
        if(hour==0) hour=12;
        else if(hour>12) hour-=12;

        string hourStr= (hour<10) ? "0"+to_string(hour) : to_string(hour);
        return hourStr+":00 "+am_pm;
}

bool isValidContact(const string& c){
    if(c.length()!=11) return false;
    for(char ch : c){
        if(!isdigit(ch)) return false;
    }
    return true;
}

bool isValidRecord(const Patient& p){
    if(p.patientId<=0) return false;
    if(p.name.empty()) return false;
    if(!isValidContact(p.contact)) return false;
    return true;
}

static void cleanPatientsFile(){
    ifstream inFile(PATIENTS_FILE);
    if(!inFile){
        cerr<<RED<<"Error opening patient file for cleaning."<<RESET<<endl;
        return;
    }
    Patient temp[MAX_RECORDS];
    int count=0;
    string line;
    while(getline(inFile,line) && count<MAX_RECORDS){
        if(line.empty()) continue;

        istringstream ss(line);
        Patient p;
        string tok;

        if(!getline(ss,tok,'#')) continue;
            try {
                p.patientId = stoi(tok);
            }
            catch (...) {
            continue;
            }

        if(!getline(ss,p.name,'#')) continue;
        if(!getline(ss,tok,'#')) continue;
       try{
            p.age = stoi(tok);
        }
        catch(...){
            continue;
        }
        if(!getline(ss,p.gender,'#')) continue;
        if(!getline(ss,p.contact,'#')) continue;
        if(!getline(ss,tok,'#')) p.balance=0.0;
        else p.balance=stod(tok);

        p.gender=fixGender(p.gender);
        p.contact=p.contact;
        if(isValidRecord(p)){
            temp[count++]=p;
        }
    }
       inFile.close();

       bool removed[MAX_RECORDS]={false};
         for(int i=0;i<count;i++){
            if (removed[i]) continue;
              for(int j=i+1;j<count;j++){
                if(temp[i].patientId==temp[j].patientId){
                     removed[j]=true;
                }
              }
         }

        ofstream outFile(PATIENTS_FILE);
        int written=0;
        if(!outFile){
            cerr<<RED<<"Error opening patient file for writing."<<RESET<<endl;
            return;
        }
        for(int i=0;i<count;i++){
            if(removed[i]) continue;
                outFile<<temp[i].patientId<<"#"<<temp[i].name<<"#"<<temp[i].age<<"#" << temp[i].gender<<"#"<<temp[i].contact<<"#"<<fixed<<setprecision(2) << temp[i].balance << "\n";
        written++;
        }
        outFile.close();
        cout << GREEN << "  [Cleaned] patients.txt  - " << written << " valid record(s) kept." << RESET << "\n";
}

static void cleanDoctorsFile(){
    ifstream inFile(DOCTORS_FILE);
    if(!inFile){
        cerr<<RED<<"Error opening doctor file for cleaning."<<RESET<<endl;
        return;
    }
    Doctor temp[MAX_RECORDS];
    int count=0;
    string line;
    while(getline(inFile,line) && count<MAX_RECORDS){
        if(line.empty()) continue;

        istringstream ss(line);
        Doctor d;
        string tok;

        if(!getline(ss,tok,'#')) continue;
        d.doc_id=stoi(tok);

        if(!getline(ss,d.name,'#')) continue;
        if(!getline(ss, d.specialty,'#')) continue;
        if(!getline(ss, tok,'#')) continue;
        d.experience=stoi(tok); 
        if(d.doc_id>0 && !d.name.empty() && !d.specialty.empty() && d.experience>=0){
            temp[count++]=d;
        }
}
    inFile.close();

    ofstream outFile(DOCTORS_FILE);
    for (int i = 0; i < count; i++)
        outFile << temp[i].doc_id << "#" << temp[i].name << "#"
             << temp[i].specialty << "#" << temp[i].experience << "\n";
    outFile.close();
    cout << GREEN << "  [Cleaned] doctors.txt   - " << count << " valid record(s) kept." << RESET << "\n";
}

static void cleanAppointmentsFile(){
    ifstream inFile(APPOINTMENTS_FILE);
    if(!inFile){
        cerr<<RED<<"Error opening appointments file for cleaning."<<RESET<<endl;
        return;
    }
    struct RawAppt{
        int pid;
        int did;
        string date;
        string time;
    };
    RawAppt temp[MAX_RECORDS];
    int count=0;
    string line;
    while(getline(inFile,line) && count<MAX_RECORDS){
        if(line.empty()) continue;

        istringstream ss(line);
        RawAppt a;
        string tok;
        string sDate, sTime;

        if(!getline(ss,tok,'#')) continue;
        try{
            a.pid = stoi(tok);
        }
        catch(...){
            continue;
        }

        if(!getline(ss,tok,'#')) continue;
         try {
            a.did = stoi(tok);
        }
        catch (...) {
          continue;
        }

        if(!getline(ss,sDate,'#')) continue;
        a.date = fixDate(sDate);
        if(!getline(ss,sTime,'#')) sTime = "09:00 AM"; a.time = fixTime(sTime);

        if(a.pid>0 && a.did>0 ){
            temp[count++]=a;
        }
    }
    inFile.close();

    ofstream outFile(APPOINTMENTS_FILE);
    for (int i = 0; i < count; i++)
        outFile << temp[i].pid << "#" << temp[i].did << "#"
             << temp[i].date << "#" << temp[i].time << "\n";
    outFile.close();
    cout<< GREEN << "  [Cleaned] appointments.txt  - " << count << " valid record(s) kept." << RESET << "\n";
}

static void cleanTreatmentsFile(){
    ifstream inFile(TREATMENTS_FILE);
    if(!inFile.is_open()) return;

    Treatment temp[MAX_RECORDS];
    int count=0;
    string line;

    while(getline(inFile,line) && count < MAX_RECORDS){
        if(line.empty()) continue;
        istringstream ss(line);
        string tok;
        Treatment t;

        if (!getline(ss, tok,'#')) continue; 
        try{
            t.patientId = stoi(tok);
        }
        catch(...){
            continue;
        }
        if (!getline(ss, t.description,'#')) continue;
        if (!getline(ss, tok,         '#'))try{
            t.cost = stod(tok);
        }
        catch(...){
            continue;
        }
        if (!getline(ss, tok,         '#')) t.paid = false;
        else t.paid = (tok == "true" || tok == "1");
 
        if (t.patientId > 0 && !t.description.empty() && t.cost >= 0)
            temp[count++] = t;
    }
    inFile.close();

    ofstream outFile(TREATMENTS_FILE);
    for (int i = 0; i<count; i++)
        outFile<<temp[i].patientId<<"#" << temp[i].description<<"#"
             <<fixed<<setprecision(2)<< temp[i].cost<<"#"
             << (temp[i].paid ? "true" : "false")<<"\n";
        outFile.close();
    cout << GREEN << "  [Cleaned] treatments.txt  - " << count << " valid record(s) kept." << RESET << "\n";
}

static void cleanBillsFile(){
    ifstream inFile(BILLS_FILE);
    if(!inFile.is_open()) return;

    struct Bill{
        int pid;
        double amount;
        string status;
    };
    Bill temp[MAX_RECORDS];
    int count=0;
    string line;

    while(getline(inFile,line) && count < MAX_RECORDS){
        if(line.empty()) continue;
        istringstream ss(line);
        string tok;
        Bill b;

        if(!getline(ss,tok,'#')) continue; try{
            b.pid = stoi(tok);
        }
        catch(...){
            continue;
        }
        if(!getline(ss,tok,'#')) continue; try{
            b.amount = stod(tok);
        }
        catch(...){
            continue;
        }
        if(!getline(ss,b.status,'#')) b.status = "Unpaid";
        if(b.pid > 0 && b.amount >= 0)
            temp[count++] = b;
    }
    inFile.close();

    ofstream outFile(BILLS_FILE);
    for (int i = 0; i < count; i++)
        outFile << temp[i].pid << "#" << fixed << setprecision(2) << temp[i].amount << "#" << temp[i].status << "\n";
    outFile.close();
    cout << GREEN << "  [Cleaned] bills.txt  - " << count << " valid record(s) kept." << RESET << "\n";
}

void cleanFile(){
    cout << "\n  Running startup validation on all data files...\n\n";
    cleanPatientsFile();
    cleanDoctorsFile();
    cleanAppointmentsFile();
    cleanTreatmentsFile();
    cleanBillsFile();
    cout << "\n" << CYAN << "  All files validated successfully." << RESET << "\n";
    pressEnter();
}
