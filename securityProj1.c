#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <openssl/aes.h>

void functionToAlter(){};

static int checkFolder(char *myPath) {
    DIR *d;
    struct dirent *dir;

    int success = 0;
    //printf("Path: %s\n", myPath);
    if ((d = opendir(myPath)) != NULL) {
        while ((dir = readdir(d)) != NULL) {
            char *separator = "/";
            char *pathFound = dir->d_name;
            char  tempPath[1024];

            strcpy(tempPath, myPath);
            strcat(tempPath, separator);
            strcat(tempPath, pathFound);
            //printf("Files in Path: %s\n", tempPath);

            if (strstr(tempPath, "comp6370_proj1.txt")) {
                success = 1;
                remove(tempPath);
                //printf("Found: %s\n", tempPath);
            } else if (strchr(tempPath, '.') == 0) {
                success = checkFolder(tempPath);
            }
        }

        closedir(d);
    } else {
        perror("opendir() error");
    }

    return success;
}

int main(int argc, char *argv[]) {
     //char myPath[] = "/class/comp6370";
     char myPath[] = "Proj1Test";
     int success = checkFolder(myPath);

     if (success == 1) {
         printf("File successfully deleted\n");
     } else {
         printf("File to delete not found or does not exist!\n");
     }
    
    int sizeOfFile, index;  
    FILE *fp;
    //getFile
    char * move;
    char * cpy;
    char * deleteFile;
    char * fileName;
    fileName = malloc(sizeof(fileName));
    move = malloc(sizeof(move));
    cpy = malloc(sizeof(cpy));
    deleteFile = malloc(sizeof(deleteFile)); 

    fileName = argv[0];
    memcpy(fileName, &fileName[2], (strlen(fileName)-2));
    fileName[strlen(fileName)-2] = '\0';

    snprintf(move, 100, "mv %s tempFile", fileName); 
    system(move);

    snprintf(cpy, 100, "cp tempFile %s", fileName);
    system(cpy);

    snprintf(deleteFile, 100, "rm -f tempFile");
    system(deleteFile);

    free(deleteFile);
    free(cpy);
    free(move);
   
    fp = fopen(fileName, "r+b");  
   
    if (fp == NULL) { 
        fprintf(stderr, "File does not exist!\n"); exit(1);
    }
  	
    //get Size of file
    fseek(fp, 0L, SEEK_END);
    sizeOfFile = ftell(fp);
    rewind(fp);

    char * inputChars;
    char message[sizeOfFile + 1];
    int i = 0;
 
    for (i; i < sizeOfFile + 1; i++) {
        message[i] = '\0';
    }

    inputChars = malloc(sizeOfFile + 1);
    char getChar;
    int messageLength = 0;
    fseek(fp, 10807, SEEK_SET);
    while((getChar = fgetc(fp)) != EOF) {	
        message[messageLength] = getChar;
        messageLength++;
    }
   
    printf("Before Encryption: \n");
    printf("%d\n", strlen(message));

    unsigned char aes_key[128 / 8];
    memset(aes_key, 0, 128 / 8);
 
    AES_KEY enc_key;
    AES_KEY dec_key;
    AES_set_encrypt_key(aes_key, 128, &enc_key);
    AES_cbc_encrypt(aes_input, enc_out, inputslength, &enc_key, iv_enc, AES_ENCRYPT);

    printf("After Encryption: \n");
    printf("%s", message);
    printf("\n");
    return(0);
}
