#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

/*
   Inserts charIn at the head of the temporary array tempChar
   It then copies the chars from charArr into tempChar at
   tempChar[i+1] until it finds charIn in charArr.
   It then finishes copying the array normally. eg
   charArr[i] -> tempChar[i]
   It then copies tempChar back into charArr with the char
   now at the beginning of the array.
*/
void insertAtHead(char charIn, char charArr[], int sizeOfArr) {
    char tempChar[sizeOfArr + 1]; //temp Array
    int charFound = 0; //has charIn been found 0 = no
    tempChar[0] = charIn; //insert at head
   
    //Copying of charArr into tempChar
    for(int i = 0; i < sizeOfArr; i++) {
        if (charFound == 0 && charArr[i] != charIn && charIn != '\n') {
            //charIn not found       
            tempChar[i+1] = charArr[i];
        } else if (charArr[i] != charIn) {
            //charIn found
            tempChar[i] = charArr[i];
        } else {
            //charIn found variable update
            charFound = 1;
        }
    }
   
    //Copy tempChar back into charArr
    for (int i = 0; i < sizeOfArr; i++) {
        charArr[i] = tempChar[i];
    }
}

//--------------------------------------------------

int main(int argc, char *argv[]) {
    int sizeOfFile;
    int index;  
    FILE *fp;
    //getFile

    if (argv[1] != NULL) {
        fp = fopen(argv[1], "r");
    } else {
        fprintf(stderr, "No Arguments. File name must be given as an argument!\n"); exit(1);
    }
   
    if (fp == NULL) {
        fprintf(stderr, "File does not exist!\n"); exit(1);
    }

    //get Size of file
    fseek(fp, 0L, SEEK_END);
    sizeOfFile = ftell(fp);
    rewind(fp);
  
    //Double size to allow for extra characters
    sizeOfFile = sizeOfFile * 2;
 
    //Initialize arrays
    char message[sizeOfFile + 1];
    char exploredChars[sizeOfFile + 1];
    char encMessage[sizeOfFile + 1]
    char decMessage[sizeOfFile + 1];
    char * inputChars;
  
    inputChars = malloc(sizeOfFile + 1);
  
    //Prep arrays by inserting null terminating strings in all indexes for safety.
    for (int i = 0; i < sizeOfFile + 1; i++) {
        message[i] = '\0';
        encMessage[i] = '\0';
        exploredChars[i] = '\0';
        decMessage[i] = '\0';
    }
  
    //read in chars into array 
    //Reads each line of the file at a time
    //stores that into message
    //concatenates message into inputChars
    while (fgets(&message[0], sizeOfFile + 1, fp) != NULL) {
        strcat(inputChars, message);
    }

    //Copy inputChars back into message
    strcpy(message, inputChars);

    //Close file
    fclose (fp); 
  
    //Handles the individual chars or double digit numbers
    //Numbers shouldn't get over 99
    char result[2];

    for (int i = 0; i < strlen(message) - 1; i++) {
        //Insure reset of array
        result[0] = '\0';
        result[1] = '\0';
     
        //Check if char is a number
        if(isdigit(message[i])) {
            //If the next char is a space, this is a single digit number
            //first part insures that message[i+1] isn't out of bounds
            if ((i + 1) < strlen(message) && message[i+1] == ' ') {
                index = message[i] - '0'; //Convert to int
                result[0] = exploredChars[index]; //get char at that index
                strcat(encMessage, result); //add to the decodedMessage
                //Insert character at head of the array
                insertAtHead(exploredChars[index], exploredChars, sizeOfFile); 
            } else {//This is a double digit number
                //Store both numbers into result             
                result[0] = message[i];
                result[1] = message[i + 1];
                index = atoi(result); //convert number to int
                result[1] = '\0'; //clear this index
                result[0] = exploredChars[index]; //store the char
                strcat(encMessage, result); //add to decodedMessage

                //Insert character at head of the array
                insertAtHead(exploredChars[index], exploredChars, sizeOfFile);
                i++;
            }
        } else {
            //ignore all spaces except when there are 3 in a row
            //Always the first occurence of a space in the message
            if(message[i] != ' ' || (message[i] == ' '  && message[i+1] == ' ' && message[i+2] == ' ')) {
                //Store
                result[0] = message[i];
                //Add
                strncat(encMessage, result, 1);
                //Insert character at head of the array
                insertAtHead(message[i], exploredChars, sizeOfFile);
            }
        }
    }
    printf("Encoded Message: %s\n", message);
    printf("Decoded Message: %s\n", encMessage);
    
}


