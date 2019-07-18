#include "assembler.h"

/* prints information about an error to the standard output */
void err(int errCode, char* word, int lnNum){ 
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
    if(word != NULL)
        printf("\t%s\n", word);
}