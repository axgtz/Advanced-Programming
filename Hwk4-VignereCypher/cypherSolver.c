//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-09-15.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 50

int main () {
    // Ask Gil about variable declaration at the beginning or where you use it
    FILE * file_ptr = NULL;
    int option = 0;
    char fileName[STR_SIZE];
    char encryptionKey[STR_SIZE];
    // (i+k) % 25;
    // i = posicion actual (valor de pass a encriptar)  k = num de rot (valor de encoding key)
    strncpy(fileName, "attacknow", STR_SIZE);
    strncpy(encryptionKey, "lemon", STR_SIZE);

    int tam = 9;
    int tamEnc = 5;
    for(int i = 0; i < tam;i++){
        int tot = ((fileName[i]-97) + (encryptionKey[i%tamEnc])-97) %26;
        printf("%d ",tot);
        fileName[i] = 97 + tot; // 97 is the
    }
    for (int j = 0; j < tam; ++j) {
        printf("%c ", fileName[j]);
    }
    /*for (int i = 0; i < 26 ;i++){
        int a = i + 97;
        char c = a;
        printf("%c", c);
    }*/



    /*
    // User choose to decode or encode a message
    printf("VIGNERE CYPHER\n");
    printf("Do you want to encode or decode a message? Write 1 for encode or 2 for decode\n");
    scanf("%d", &option);
    fgetc(stdin);   // Remove the enter left from scanf

    if(option == 1){ // Encode
        printf("Please write the name of the file.\n");
        fgets(fileName, STR_SIZE, stdin);          // Get the name of the file
        fileName[strlen(fileName)-1] = '\0';      // Remove the newline character
        printf("%s", fileName);

        // Get file and iterate through each char
        // Ignore special characters and pass them directly. if they are not within a certain range
        // Non special chars need to be evaluated with encryption key

    }else if(option == 2){ // Decode
        printf("Please write the encryption key MAX LENGTH = 50\n");
        fgets(encryptionKey, STR_SIZE, stdin);
        encryptionKey[strlen(encryptionKey)-1] = '\0';      // Remove the newline character
        printf("%s", encryptionKey);


    }else{
        printf("Please write a valid option next time\n");
    }*/

    return  0;
}

/*
 A fork is used to create a child process that will take care of the task
 of encoding or decoding. When the child finishes it will inform its parent process,
 write a new file with the prefix "encoded_" or "decoded_" and send back the name of
 the new file to its parent.

You can use any of the Inter Process Communication (IPC) methods seen so far
 (pipe, popen, fifo, passing arguments), as long as you have two way communication
 between the parent and the child processes.
 */
