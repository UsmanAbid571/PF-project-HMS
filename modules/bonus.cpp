#include"../headers/globals.h"
#include"../headers/bonus.h"
#include"../headers/patient.h"
#include"../headers/doctor.h"

int weeklySchedule[5][8] = {0}; // 5 days, 8 time slots (9am-4pm)

static const string DAY_NAMES[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
static const string TIME_SLOTS[] = {"9AM", "10AM", "11AM", "12PM", "1PM", "2PM", "3PM", "4PM"};

void displayWeeklySchedule(int docId){
    Doctor d = findDoctorById(docId);
    string docName = (d.doc_id != -1) ? d.name : "Unknown Doctor";

    printDivider('=', 90);
    cout<<BOLD<<BLUE<<"\n\t\t Weekly Schedule - "<<docName<<" (ID: "<<docId<<")\n"<<RESET<<"\n";
    printDivider('=', 90);

    cout<<"             ";
    for(int s=0;s<8;s++){
        cout<<CYAN<<setw(10)<<TIME_SLOTS[s]<<RESET;
    }
    cout<<"\n";
    printDivider('-', 90);

    for(int day = 0; day < 5; day++){
        cout << left << setw(10) << DAY_NAMES[day] << "|  ";

    for(int slot = 0; slot < 8; slot++){
        if(weeklySchedule[day][slot] == 0){
            cout <<GREEN<< setw(10) << "Free"<<RESET;
        } 
        else {
            cout<<YELLOW << setw(10) << to_string(weeklySchedule[day][slot])<<RESET;
        }
    }
    cout << "\n";
}

printDivider('=', 90);
}

void bookSlotIfFree(int day, int slot, int patientId){
    if(day < 0 || day >= 5 || slot < 0 || slot >= 8){
        cout<<RED<<" Invalid day or time slot index.\n"<<RESET;
        return;
    }

    if(weeklySchedule[day][slot] != 0){
        cout<<YELLOW<<" Slot "<<TIME_SLOTS[slot]<<" on "<<DAY_NAMES[day]<<" is already booked by (Patient "<<weeklySchedule[day][slot]<<").\n"<<RESET;
        
    }
    else {
        weeklySchedule[day][slot] = patientId;
        cout<<GREEN<<" Slot "<<TIME_SLOTS[slot]<<" on "<<DAY_NAMES[day]<<" is booked for Patient "<<patientId<<".\n"<<RESET;
    }
}

int findNextFreeSlot(int day,int slot, int sch[][8]){
    if(day>=5)
        return -1;
    if(slot>=8){
        return findNextFreeSlot(day+1,0,sch);
    }
    if(sch[day][slot]!=0){
        return findNextFreeSlot(day,slot+1,sch);
    }
    cout<<GREEN<<" Next free slot: "<<DAY_NAMES[day]<<" at "<<TIME_SLOTS[slot]<<".\n"<<RESET;
    return slot;
}

void bonusMenu(){
    int choice;
    do{
        printHeader("Weekly Doctor Schedule");
        cout<<"\n"
        <<" 1. View Weekly Schedule\n"
        <<" 2. Book Slot\n"
        <<" 3. Find Next Free Slot\n"
        <<" 4. Back to Main Menu\n"
        <<"\n Enter your choice: ";
        cin>>choice;
        
        switch(choice){
            case 1: {
                int docId;
                cout<<"\n Enter Doctor ID: ";
                cin>>docId;
                displayWeeklySchedule(docId);
                pressEnter();
                break;
            }
            case 2: {
                int day, slot, patientId;
                cout<<"\n Enter day index (0=Mon, 4=Fri): ";
                cin>>day;
                cout<<" Enter time slot index (0=9AM, 7=4PM): ";
                cin>>slot;
                cout<<" Enter Patient ID: ";
                cin>>patientId;
                bookSlotIfFree(day, slot, patientId);
                pressEnter();
                break;
            }
            case 3: {
                cout<<"\n Searching for next free slot from Monday 9AM ...\n";
                int result= findNextFreeSlot(0, 0, weeklySchedule);
                if(result == -1){
                    cout<<RED<<" No free slots available this week.\n"<<RESET;
                }
                pressEnter();
                break;
            }
            case 4:
                cout<<"\n Returning to main menu...\n";
                break;
            default:
                cout<<RED<<"\n Invalid choice. Please try again.\n"<<RESET;
        }
    } while(choice != 4);
}