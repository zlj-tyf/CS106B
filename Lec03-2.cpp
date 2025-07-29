// #include"../Headers/strlib.h"
#include<iostream>
#include<fstream>
using namespace std;
int main(){
    ifstream ff;
    string s;
    ff.open("../in.txt");
    cout<<getline(ff,s)<<endl;
    cout<<s<<endl;
}