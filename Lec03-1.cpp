#include <iostream>
#include <fstream>
#include "../Headers/strlib.h"
using namespace std;
int main()
{
    ifstream input;
    input.open("../in.txt");
    string ln;
    int cnt=0;
    // while (!input.fail() && getline(input, ln))
    // {
    //     ++cnt;
    //     cout << ln << endl;
    // }
    cout<<stringToInteger("123")+1<<endl;
    getchar();
    while(input >> ln){cout<<ln<<endl;++cnt;}
    cout<<cnt<<endl;
    input.close();
}