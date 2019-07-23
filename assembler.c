#include <ctype.h>
#include "assembler.h"

int lnNumber, returned;

void rmvLnSpaces(char* line); /* make sure theres no occurrence of multiple spaces in the line */

int isFirstWordLabel(char* firstWord);
int getFirstWord(char* word);
int data();
int string();
int external();
int entry();
int define();

int assemble(){
    char *currLine, *currWord, *readLine;
    if((currLine = (char*) calloc(MAXSIZE, sizeof(char))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    if((readLine = (char*) calloc(MAXSIZE, sizeof(char))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    lnNumber = 1; /* set the line number to 1 when starting to go over the file */
    while((fgets(currLine, MAXSIZE, src->source)) != NULL){ /* as long as there's another line */
        strcpy(readLine, currLine);
        rmvLnSpaces(currLine);
        currWord = strtok(currLine, " \n");
        if(strlen(currWord) == 0 || currLine[0] == ';')
            continue;
        if((returned = isFirstWordLabel(currWord)) > 0)
            continue;
        else if (returned == 0) {/* if label is illegal */
            if((currWord = strtok(NULL, " \n")) == NULL)
                err(LONELY_LABEL, currLine, lnNumber);
        }
        if((returned = getFirstWord(currWord)) == -1) {/* if first word is illegal */ 
            err(ILLEGAL_FIRST_WORD, currWord, lnNumber);
            break;
        }
        else switch (returned){
            case 0: /* .data */
                if((returned = data()) != TRUE)
                    err(returned, readLine, lnNumber);
                break;
            case 1: /* .string */
                if((returned = string()) != TRUE)
                    err(returned, readLine, lnNumber);
                break;
            case 2: /* .entry */
                if((returned = entry()) != TRUE)
                    err(returned, readLine, lnNumber);
            case 4: /* .extern */
               if((returned = external()) != TRUE)
                    err(returned, readLine, lnNumber);
            case 5: /* .define */
                if((returned = define()) != TRUE)
                    err(returned, readLine, lnNumber);
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
    free(currWord);
    free(readLine);
    return 0;
}