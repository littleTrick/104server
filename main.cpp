#include<iostream>
#include<iec104.h>

using namespace std;

int main()
{
    IEC104 iec104;
    iec104.connectTCP();
    return 0;
}
