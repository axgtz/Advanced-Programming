//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-09-15.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 100

int main () {
    FILE * file_ptr_read = NULL;    // Pointer to file being read
    FILE * file_ptr_write = NULL;   // Pointer to file being writen

    int option = 0;
    char fileName[STR_SIZE];                // Name of the file to read
    char encryptionKey[STR_SIZE];          // Encryption key
    char lineFileBuffer[STR_SIZE];        // Line from file

    // Used to test, should be used as input
    strncpy(fileName, "example_encodings/aladdin.txt", STR_SIZE);
    /*
     *  char *resultFileName = malloc(tamFile + 12 + 1);
    strcpy(resultFileName, "encoded_");
    strcat(resultFileName, fileName);
    strcat(resultFileName, ".txt");
     * */
    strncpy(encryptionKey, "lemon", STR_SIZE);

    file_ptr_read = fopen(fileName, "r");
    printf("%s", fileName);
    if (!file_ptr_read){// Check that file is valid
        perror("Error! reading the file");
        // Program exits if the file pointer returns NULL.
        exit(EXIT_FAILURE);
    }

    size_t tamFile = strlen(lineFileBuffer); // Size of the stored data inside the buffer
    size_t tamEncKey = strlen(encryptionKey); // Size of the encryption key
    fgets(lineFileBuffer, STR_SIZE, file_ptr_read);

    fclose(file_ptr_read);

    // Encryption
    for(int i = 0; i < tamFile;i++){
        if(lineFileBuffer[i] > 96 && lineFileBuffer[i] < 123){ // if its within the alphabet
            int tot = ((lineFileBuffer[i]-97) + (encryptionKey[i%tamEncKey])-97) %26;
            lineFileBuffer[i] = 97 + tot; // 97 is the ASCII number for 'a'
        } else {// Non-alphabet character, keeps it the same
            lineFileBuffer[i] = lineFileBuffer[i];
        }
    }
    for (int j = 0; j < tamFile; ++j) { // Print result encoded
        printf("%c", lineFileBuffer[j]);
    }
    // Opening a file for writing
    // Create new name for the result file
    char *resultFileName = malloc(tamFile + 12 + 1);
    strcpy(resultFileName, "encoded_");
    strcat(resultFileName, fileName);
    strcat(resultFileName, ".txt");

    // Open file for writing
    file_ptr_write = fopen(resultFileName, "w");

    if (!file_ptr_write){// Check that file is valid
        perror("Error! creating the file");
        // Program exits if the file pointer returns NULL.
        exit(EXIT_FAILURE);
    }
    fprintf(file_ptr_write, "%s",lineFileBuffer); // Write all the lineFileBuffer into the file
    fclose(file_ptr_write);

    free(resultFileName); // Free memory of string
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
        printf("Please write the encryption key MAX LENGTH = 99\n");
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
/* for (int j = 0; j < tamFile; ++j) { // Print result encoded
     printf("%c", lineFile[j]);
 }*/


/* Print the ASCII characters
 * for (int i = 0; i < 26 ;i++){
    int a = i + 97;
    char c = a;
    printf("%c", c);
}*/


