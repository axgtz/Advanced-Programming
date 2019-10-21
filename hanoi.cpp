//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-10-16.
//
#include <iostream>

using namespace std;

void hanoi(int n, char a, char b, char c) //n = el numero de discos, y a b c son los postes
{
    if (n == 1)
    {
        cout<<"Mover de "<<n<<" a "<<c<<endl;
    }
    else
    {
        hanoi(n-1,a,c,b);
        cout<<"Mover de "<< n <<" a "<<c<<endl;
        hanoi(n-1,b,a,c);
    }
}

int main()
{
    hanoi(3,'A','B','C');


    return 0;
}
