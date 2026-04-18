#include<iostream>
#include"headers/login.h"
    
using namespace std;
int main()
{
    string empId="EMP001";
    string password="pass123";
    validateCredentials( empId, password);
    login();
    return 0;
}