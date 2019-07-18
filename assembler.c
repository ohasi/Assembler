#include <ctype.h>
#include "assembler.h"

#define CMDNUM 16
#define INSTRNUM 5


int lnNumber, returned;
char* commands[] = {"mov", "cmp", "add", "sub", "lea", "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"},
      instructions[] = {".data", ".string", ".entry", ".extern", ".define"};

void rmvLnSpaces(char* line); /* make sure theres no occurrence of multiple spaces in the line */
int checkLabel(char* label);
int getFirstWord(char* word);

int assemble(){
    char* currLine;
    char* currWord;
    if((currLine = (char*) calloc(MAXSIZE, sizeof(char))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    lnNumber = 1; /* set the line number to 1 when starting to go over the file */
    while((fgets(currLine, MAXSIZE, src->source)) != NULL){ /* as long as there's another line */
        rmvLnSpaces(currLine);
        currWord = strtok(currLine, " \n");
        if(strlen(currWord) == 0 || currLine[0] == ';')
            continue;
        if(returned = isFirstWordLabel(currWord) > 0)
            continue;
        else if (returned == 0) {/* if label is illegal */
            if((currWord = strtok(NULL, " \n")) == NULL)
                err(LONELY_LABEL, currLine, lnNumber);
        }
        if((returned = getFirstWord) == -1) {/* if first word is illegal */ 
            err(ILLEGAL_FIRST_WORD, currWord, lnNumber);
            break;
        }
        else switch (returned){
            case 0: /* .data */
                data(lnNumber);
                break;
            case 1: /* .string */
                string(lnNumber);
                break;
            case 2: /* .entry */
                entry(lnNumber);
                break;
            case 4: /* .extern */
                break;
            case 5: /* .define */
                break;
            case 10: /* mov */
                break;
            case 11: /* cmp */
                break;
            case 12: /* add */
                break;
            case 13: /* sub */
                break;
            case 14: /* lea */
                break;
            case 15: /* not */
                break;
            case 16: /* clr */
                break;
            case 17: /* inc */
                break;
            case 18: /* dec */
                break;
            case 19: /* jmp */
                break;
            case 20: /* bne */
                break;
            case 21: /* red */
                break;
            case 22: /* prn */
                break;
            case 23: /* jsr */
                break;
            case 24: /* rts */
                break;
            case 25: /* stop */
                break;
            }
        lnNumber++;
    }
    free(currLine);
    return 0;
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
/* check if a label is a legal label */
int checkLabel(char* cLabel){
    int i1;
    if(getFirstWord(cLabel) != -1) {
        err(LABEL_CANT_BE_SAME_NAME_AS_COMMAND, cLabel, lnNumber);
        return LABEL_CANT_BE_SAME_NAME_AS_COMMAND;
    }
    else for(i1 = 0; i1 < strlen(cLabel) - 1; i1++)
        if(!isalnum(cLabel[i1])){
            err(ILLEGAL_LABEL, cLabel, lnNumber);
            return ILLEGAL_LABEL;
        }
    for(i1 = 0; i1 < SC; i1++)
        if(strcmp(symbolList[SC].label, cLabel) == 0){
            err(USED_LABEL, cLabel, lnNumber);
            return USED_LABEL;
        }
    if(!(isalpha(cLabel[0]))){
        err(ILLEGAL_LABEL, cLabel, lnNumber);
        return ILLEGAL_LABEL;
    }
    return 0;
}
/* checks if the first word is a label, and if it is, adds it t the data list */
int isFirstWordLabel(char* firstWord){
    char* nextWord;
    int data;
    if(firstWord[strlen(firstWord) - 1] == ':'){
        firstWord[strlen(firstWord) - 1] = '\0';
        if(checkLabel(firstWord) == ILLEGAL_LABEL)
                return ILLEGAL_LABEL;
        else {
            if (nextWord = calloc(7, sizeof(char)) == NULL)
                err(OUT_OF_MEMORY, NULL, 0);
            strncpy(nextWord, firstWord[strlen(firstWord) + 3], 6);
            nextWord[6] = '\0';
            if(strcmp(".data ", nextWord) == 0)
                data = DC;
            else {
                free(nextWord);
                if(nextWord = calloc(9, sizeof(char)) == NULL) 
                    err(OUT_OF_MEMORY, NULL, 0);
                strncpy(nextWord, firstWord[strlen(firstWord) + 3], 8);
                nextWord[8] = '\0';
                if(strcmp(".string ", nextWord) == 0)
                    data = DC;
                else if(strcmp(".extern ", nextWord) == 0)
                    return 0;
                else {
                    if(nextWord = calloc(8, sizeof(char)) == NULL) 
                    err(OUT_OF_MEMORY, NULL, 0);
                    strncpy(nextWord, firstWord[strlen(firstWord) + 3], 7);
                    nextWord[7] = '\0';
                    if(strcmp(".entry ", nextWord) == 0)
                    return 0;
                }
            }
            addSymbol(firstWord, data, code);
            free(nextWord);
            return 0;
        }
    }
    else return -1;
}
/* returns the command id of the first word, or -1, if word is not a command */
int getFirstWord(char* word){
    int i2;
    if(word[0] == '.'){ /* if word is instruction word */
        for(i2 = 0; i2 < INSTRNUM; i2++)
            if(strcmp(word, instructions[i2]) == 0)
                return i2;
        err(UNKNOWN_INSTRUCTION, word, lnNumber);
        return UNKNOWN_INSTRUCTION;
    }
    else if(isalpha(word[0])){
        for(i2 = 0; i2 < CMDNUM; i2++)
            if(strcmp(word, commands[i2]) == 0)
                return i2 + 10;
        err(UNKNOWN_COMMAND, word, lnNumber);
        return UNKNOWN_COMMAND;
    }
    else return -1;
}