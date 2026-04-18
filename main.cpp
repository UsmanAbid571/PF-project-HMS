#include<iostream>
#include"headers/login.h"
#include"headers/cleaning.h"
    
using namespace std;
int main()
{
    string empId="EMP001";
    string password="pass123";
    validateCredentials( empId, password);
    login();
    cleanFile();
    return 0;
}