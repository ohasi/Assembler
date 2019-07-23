#include "assembler.h"
#include <ctype.h>

int getFirstWord(char* word, int lnNumber);
/* prints information about an error to the standard output */
int err(int errCode, char* line, int lnNum){ 
    errNum++;
    if(lnNum > 0)
            printf("%s.as: %d: ", src->fileName, lnNum);
    switch (errCode){
        case NO_SOURCE_FILE:
            printf("assembler: error: no input files\ncompilation terminated.\n");
            break;
        case CANT_OPEN_FILE: 
            printf("assembler: error trying to open file %s.as\n", src->fileName);
            break;
        default:
            printf("unknown error.");
           
    }
    if(line != NULL)
        printf("\t%s\n", line);
    return errCode;
}

int isLegalNumber(char* number){
    int i_isLegalNumber;
    for(i_isLegalNumber = 1; i_isLegalNumber < strlen(number); i_isLegalNumber++)
        if(!isdigit(number[i_isLegalNumber]))
            return FALSE;
    if(!isdigit(number[0])){
        if(number[0] != '+' && number[0] != '-')
            return FALSE;
    }
    return TRUE;
}