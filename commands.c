#include <ctype.h>
#include "assembler.h"

int checkLabel(char* cLabel);

int data(int linenum){
    int i3;
    char* nextData;
    while(nextData = strtok(NULL, ",\n") != NULL){
        for(i3 = 0; i3 < strlen(nextData); i3++)
            if(!(isdigit(nextData[i3]) || nextData[i3] == '+' || nextData[i3] == '-')){
                err(NUMBER_EXPECTED, nextData, linenum);
                return NUMBER_EXPECTED;
            }
        addData(atoi(nextData));
    }
}

int string(int lineNum){
    int i4;
    char* nextWord;
    nextWord = strtok(NULL, " \n");
    if(strtok(NULL, " \n") != NULL){
        err(TOO_MANY_PARAMETERS, nextWord, lineNum);
        return TOO_MANY_PARAMETERS;
    }
    else if (!(nextWord[0] == '\"' && nextWord[strlen(nextWord) - 1] == '\"')){
        err(STRING_EXPECTED, nextWord, lineNum);
        return STRING_EXPECTED;
    }
    else for(i4 = 0; i4 < strlen(nextWord); i4++)
        addData(nextWord[i4]);
    return 0;
}

int entry(int lineNum){
    int i4, returned;
    char* nextWord;
    nextWord = strtok(NULL, " \n");
    if(strtok(NULL, " \n") != NULL){
        err(TOO_MANY_PARAMETERS, nextWord, lineNum);
        return TOO_MANY_PARAMETERS;
    }
    else if((returned = checkLabel(nextWord)) != 0)
        if (returned != USED_LABEL)
            return ILLEGAL_LABEL;
        else;
    else if (firstPass == TRUE)
        return 0;
    else {
        if(returned != USED_LABEL){
            err(UNKNOWN_LABEL, nextWord, lineNum);
            return UNKNOWN_LABEL;
        }
        else if(src->entries == NULL)
            loadFile(".ent");
        for(int i4 = 0; i4 < SC; i4++)
            if(strcmp(nextWord, symbolList[i4].label) == 0)
                if(symbolList[i4].type != macro){
                    fprintf(src->entries, "%s %4d", symbolList[i4].label, symbolList[i4].value);
                }
            return UNKNOWN_LABEL;
    }
}