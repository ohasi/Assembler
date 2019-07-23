#include "assembler.h"
#include <ctype.h>

#define CMDNUM 16
#define INSTRNUM 5

char* commands[] = {"mov", "cmp", "add", "sub", "lea", "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
char* instructions[] = {".data", ".string", ".entry", ".extern", ".define"};

int checkLabel(char* cLabel);

int getDC();
/* returns the command id of the first word, or -1, if word is not a command */
int getFirstWord(char* word){
    int i2;
    if(word[0] == '.'){ /* if word is instruction word */
        for(i2 = 0; i2 < INSTRNUM; i2++)
            if(strcmp(word, instructions[i2]) == 0)
                return i2;
        return UNKNOWN_INSTRUCTION;
    }
    else if(isalpha(word[0])){
        for(i2 = 0; i2 < CMDNUM; i2++)
            if(strcmp(word, commands[i2]) == 0)
                return i2 + 10;
        return UNKNOWN_COMMAND;
    }
    else return -1;
}
/* checks if the first word is a label, and if it is, adds it t the data list */
int isFirstWordLabel(char* firstWord, int lnNumber){
    char* nextWord;
    char* extraWord;
    int data;
    if(firstWord[strlen(firstWord) - 1] == ':'){
        firstWord[strlen(firstWord) - 1] = '\0';
        if(checkLabel(firstWord) == ILLEGAL_LABEL)
                return ILLEGAL_LABEL;
        else {
            extraWord = (firstWord + strlen(firstWord) + 3);
            if ((nextWord = (char* )calloc(7, sizeof(char))) == NULL)
                return err(OUT_OF_MEMORY, NULL, 0);
            strncpy(nextWord, extraWord, 6);
            nextWord[6] = '\0';
            if(strcmp(".data ", nextWord) == 0)
                data = getDC();
            else {
                free(nextWord);
                if((nextWord = (char*) calloc(9, sizeof(char))) == NULL) 
                    return err(OUT_OF_MEMORY, NULL, 0);
                strncpy(nextWord, nextWord, 8);
                nextWord[8] = '\0';
                if(strcmp(".string ", nextWord) == 0)
                    data = getDC();
                else if(strcmp(".extern ", nextWord) == 0)
                    return USELESS_LABEL;
                else if(strcmp(".define ", nextWord) == 0)
                    return ILLEGAL_FIRST_WORD;
                else {
                    if((nextWord = (char*) calloc(8, sizeof(char))) == NULL) 
                        return err(OUT_OF_MEMORY, NULL, 0);
                    strncpy(nextWord, extraWord, 7);
                    nextWord[7] = '\0';
                    if(strcmp(".entry ", nextWord) == 0)
                        return USELESS_LABEL;
                }
            }
            addSymbol(firstWord, data, code);
            free(nextWord);
            return TRUE;
        }
    }
    else return FALSE;
}
/* make sure theres no occurrence of multiple spaces in the line */
void rmvLnSpaces(char* line){ 
    int i, j;
    char* newLn;
    if((newLn = (char*) calloc(strlen(line), sizeof(char))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    for(i = 0; i < strlen(line); i++)
        if(__isascii(line[i])){
            if(isspace(line[i])){
                if(i > 0){
                    if(!(isspace(line[i-1]))){
                        *(newLn + j) = ' ';
                        j++;
                    }
                }
            }
            else {
                newLn[j] = line[i];
                j++; 
            }
        }
}