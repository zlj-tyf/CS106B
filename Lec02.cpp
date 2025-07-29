#include <iostream>
using namespace std;
// void fc(int x=1,char y='*'){
//     for (int i = 0; i < x; i++)
//     {
//         cout<<y;
//     }
//     cout<<endl;
// }
// int main(){
//     fc(3);
//     fc('?');
//     fc(3,'?');
// }

void nameDiamond(string str);
int main()
{
    string str;
    cin >> str;
    // cout<< str + 41;
    // return 0;
    nameDiamond(str);
}
void nameDiamond(string str)
{
    int len = str.length();
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            cout << str[j];
        }
        cout << endl;
    }
    for (int i = 1; i < len; i++)
    {
        for(int j=0;j<len;j++){
            if(j<i)cout<<' ';
            else cout<<str[j];
        }
        cout<<endl;
    }
}