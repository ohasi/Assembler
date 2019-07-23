#include <ctype.h>
#include "assembler.h"

int isLegalNumber(char* number);

int data(){
    char* nextData;
    while((nextData = strtok(NULL, ",\n")) != NULL){
        if(firstPass == TRUE){
            if(!isLegalNumber(nextData))
                return NUMBER_EXPECTED;
            else addData(atoi(nextData));
        }
    }
    return TRUE;
}

int string(){
    int i_string;
    char* nextWord;
    nextWord = strtok(NULL, " \n");
    if(firstPass == TRUE){
        if(strtok(NULL, " \n") != NULL) /* if there are too many parameters */
            return TOO_MANY_PARAMETERS;
        else if (!(nextWord[0] == '\"' && nextWord[strlen(nextWord) - 1] == '\"')) /* if string isn't inside parantheses */
            return STRING_EXPECTED;
        else for(i_string = 1; i_string < strlen(nextWord) - 1; i_string++)
            addData(nextWord[i_string]);
        return 0;
    }
    else return 0;
}

int entry(){
    int returned;
    char* nextWord;
    nextWord = strtok(NULL, " \n");
    if(firstPass == FALSE){
        if(strtok(NULL, " \n") != NULL)
            return TOO_MANY_PARAMETERS;
        else if((returned = checkLabel(nextWord)) != USED_LABEL)
                return returned;
            else return writeEntry(nextWord);
        return UNKNOWN_ERROR;
    }
    else return 0;
}

int external(){
    int returned;
    char* nextWord;
    nextWord = strtok(NULL, " \n");
    if(firstPass == TRUE){
        if(strtok(NULL, " \n") != NULL)
                return TOO_MANY_PARAMETERS;
        else if(!(returned = checkLabel(nextWord)))
            return returned;
        else addSymbol(nextWord, 0, exter);
    }
    else return 0;
}

int define(){
    int returned, cValue;
    char* nextWord, *cLabel;
    if(firstPass == FALSE){
        if((cLabel = strtok(NULL, " ")) == NULL)
            return NOT_ENOUGH_PARAMETERS;
        else if(!(returned  = checkLabel(cLabel)))
            return returned;
        else {
            nextWord = strtok(NULL, " ");
            if(strcmp(nextWord, "=") != 0)
                return ILLEGAL_CONTEXT;
            else {
                nextWord = strtok(NULL, " ");
                if(!isLegalNumber(nextWord))
                    return NUMBER_EXPECTED;
                else {
                    cValue = atoi(nextWord);
                    if ((nextWord = strtok(NULL, " ")) == NULL){
                        addSymbol(cLabel, cValue, macro);
                        return TRUE;
                    }
                    else return TOO_MANY_PARAMETERS;
                }
            }
        }
        return UNKNOWN_ERROR;
    }
    return TRUE;
}

int mov(){
    char *par1, *par2;
    int par1Value, par2Value;
    if((par1 = strtok(NULL, ",")) != NULL){
        nextInstruction();
        if(par1[0] == '#' && firstPass == FALSE){
            par1++;
            if(isalpha(par1[0]) && par1[0] != '-' && par1[0] != '+')
                if(isMacroDefined(par1))
                    par1Value = getMacro(par1);
                else return UNKNOWN_MACRO;
            else if(isLegalNumber(par1) == 0)
                par1Value = atoi(par1);
            else return ILLEGAL_PARAMETER;
        }
        else {
            if(isalpha(par1[0]))
                if(checkLabel(par1) == 0)
                    ;
        }
    }
    else return NOT_ENOUGH_PARAMETERS;
}