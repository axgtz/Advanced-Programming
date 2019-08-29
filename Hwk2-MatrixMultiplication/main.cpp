//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-28.
//


#include <stdio.h>
#include <unistd.h> // Library to get flags, getopt

int getopt (int argc, char *const *argv, const char *options);
/*  i vertical - j horizontal - k dentro de la linea
 * A B C      J K L         (A*J)+(B*M)+(C*P)  (A*K)+(B*N)+(C*Q)  (A*L)+(B*O)+(C*R)
 * D E F  *   M N O     =   (D*J)+(E*M)+(F*P)  (D*K)+(E*N)+(F*Q)  (D*L)+(E*O)+(F*R)
 * G H I      P Q R         (G*J)+(H*M)+(I*P)  (G*K)+(H*N)+(I*Q)  (G*L)+(H*O)+(I*R)
 */

void multiplyMatrix(string fileName1, string fileName2){
    // Read Text file 1
    FILE *file;
    file = fopen(fileName1 + ".txt", "r");
    if (file) {

        fclose(file);
    }

    int arr[2][2] = {{1,2},{3,4}};
    int arr2[2][2] = {{1,2},{3,4}};
    int res[2][2];
    for(int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++) {
            for(int k = 0;k < 2;k++){
                res[i][j]+= = arr[k][j]*arr2[i][k];
            }
        }
    }
    for (int l = 0; l < 2; ++l) {
        for (int i = 0; i < 2; ++i) {
            fprintf();
        }
    }
    // Validate that matrix can be multiply
    // Num col 1st matrix = num rows 2nd matrix
    // Result = num of rows 1st matrix and num col 2nd matrix


    // Output result into text file
}

int main(){

    return 0;
}

