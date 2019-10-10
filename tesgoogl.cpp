//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-10-09.
//

#include <iostream>
#include <string>

using namespace std;

string solution(string T) {
    // Assuming that all the input is valid. Rem T[2] = :
    // Check if no ??
    if(T[0] != '?' && T[1] != '?'&& T[3] != '?' &&T[4] != '?'){
        return T;
    }

    // Hours
    // First hour
    if(T[0] == '?'){
        if(T[1] == '?' || T[1] < '4'){//TEST this
            T[0] = '2';
        }else{
            T[0] = '1';
        }
    }
    // Second hour
    if(T[1] == '?'){
        if(T[0] == '2'){
            T[1] = '3';
        }else{
            T[1] = '9';
        }
    }

    // Minutes
    // First min
    if(T[3] == '?'){
        T[3] = '5';
    }

    // Second min
    if(T[4] == '?'){
        T[4] = '9';
    }

    return T;
}

int main() {
    // Read from stdin, solve the problem, write answer to stdout.
    string input;
    cin >> input;
    cout << solution(input);
}
