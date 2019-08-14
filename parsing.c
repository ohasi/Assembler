#include "assembler.h"

#define CMDNUM 16
#define INSTRNUM 5
#define REGNUMBER 8
#define IN 1
#define OUT 2

char* registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
char* commands[] = {"mov", "cmp", "add", "sub", "lea", "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
char* instructions[] = {"data", "string", "entry", "extern", "define"};

int checkLabel(char* cLabel);

int isRegister(char* toCheck){
    int i;
    for(i = 0; i < REGNUMBER; i++)
        if(strcmp(registers[i], toCheck) == 0)
            return i;
    return FALSE;
}

int isLegalNumber(char* number){
    int i;
    for(i = 1; i < strlen(number); i++)
        if(!isdigit(number[i]))
            return FALSE;
    if(!isdigit(number[0])){
        if(number[0] != '+' && number[0] != '-')
            return FALSE;
    }
    return TRUE;
}
/* returns the command id of the first word, or -1, if word is not a command */
int getFirstWord(char* word){
    int i;
    if(word[0] == '.'){ /* if word is instruction word */
        word++;
        for(i = 0; i < INSTRNUM; i++)
            if(strcmp(word, instructions[i]) == 0){
                if(i != 0)
                    return i;
                else return -1;
            }
        return FALSE;
    }
    else if(isalpha(word[0])){
        for(i = 0; i < CMDNUM; i++)
            if(strcmp(word, commands[i]) == 0)
                return i + 10;
        return FALSE;
    }
    else return FALSE;
}
/* checks if the first word is a label, and if it is, adds it t the value list */
int isFirstWordLabel(char* firstWord){
    char* nextWord;
    char* extraWord;
    int value, type;
    type = code;
    value = addInstruction(0) ;
    if(firstWord[strlen(firstWord) - 1] == ':'){
        firstWord[strlen(firstWord) - 1] = '\0';
        if(checkLabel(firstWord) == ILLEGAL_LABEL)
                return ILLEGAL_LABEL;
        else if(firstPass == TRUE){
            extraWord = (firstWord + strlen(firstWord) + 2);
            if ((nextWord = (char* )calloc(7, sizeof(char))) == NULL)
                return err(OUT_OF_MEMORY, NULL, 0);
            strncpy(nextWord, extraWord, 6);
            nextWord[6] = '\0';
            if(strcmp(".data ", nextWord) == 0){
                value = getDC();
                type  = type_data;
            }
            else {
                free(nextWord);
                if((nextWord = (char*) calloc(9, sizeof(char))) == NULL) 
                    return err(OUT_OF_MEMORY, NULL, 0);
                strncpy(nextWord, extraWord, 8);
                nextWord[8] = '\0';
                if(strcmp(".string ", nextWord) == 0){
                    value = getDC();
                    type = type_data;
                }
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
            addSymbol(firstWord, value, type);
            free(nextWord);
            return TRUE;
        }
        else return TRUE;
    }
    else return FALSE;
}
/* make sure theres no occurrence of multiple spaces in the line */
void rmvLnSpaces(char* line){ 
    int i, j, state;
    char* newLn;
    j = 0;
    state = OUT;
    if((newLn = (char*) calloc(strlen(line), sizeof(char))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    for(i = 0; i < strlen(line); i++){
        if(isalnum(line[i]) || line[i] == '.' || line[i] == ':'  || line[i] == '[' || line[i] == ']' || line[i] == '#' || line[i] == '\"' || line[i] == '=' || line[i] == '+' || line[i] == '-'){
            if(state == OUT){
                if(newLn[j - 1] != ',')
                    newLn[j++] = ' ';
                state = IN;
            }
            newLn[j++] = line[i];
        }
        else if (isspace(line[i])){
            if(state == IN)
                state = OUT;
        }
        else if(line[i] == ',' ||line[i] == ';'){
            if(state == IN)
                state = OUT;
            newLn[j++] = line[i];
        }
    }
    newLn[j] = '\n';
    strcpy(line, newLn);
    free(newLn);
}

/* if there's an error, sets parameter to NULL. if type is direct, returns a negative value */
int getParameter(char* parameter, int *value){ 
    int type, returned, i;
    char* num;
    type = 0;
    if(parameter[0] == '#'){ /* check type immediate */
            parameter++;
            if(isLegalNumber(parameter))
                *value = atoi(parameter);
            else if(isLabelDefined(parameter) == macro)
                *value = getMacro(parameter);
            else {
                parameter[0] = '\000';
                return ILLEGAL_PARAMETER;
            }
            return immediate;
    }
    else if(isalpha(parameter[0])) { /* check register, direct, index */
        if((returned = isRegister(parameter)) != FALSE){ /* check register */
            *value = returned;
            return reg;
        }
        else if(parameter[strlen(parameter) - 1] == ']'){ /* check index */
            for(i = strlen(parameter) - 2; i > 0; i--)
                if(parameter[i] == '['){
                    num = calloc(strlen(parameter) - i, sizeof(char));
                    strncpy(num, (parameter + i + 1), strlen(parameter) - i - 2);
                    if(isLegalNumber(num)){
                        parameter[i] = '\0';
                        type = atoi(num) + type_index;
                        free(num);
                        if(type - type_index < 0){
                            parameter[0] = '\000';
                            if(firstPass == FALSE)
                                return INDEX_MUST_BE_BIGGER_THAN_ZERO;
                            else return TRUE;
                        }
                        break;
                    }
                    else if(firstPass == FALSE){
                        if(isLabelDefined(num) == macro){
                            parameter[i] = '\0';
                            type = getMacro(num) + type_index;
                            free(num);
                            if(type - type_index < 0){
                                parameter[0] = '\000';
                                return INDEX_MUST_BE_BIGGER_THAN_ZERO;
                            }
                            break;
                        }
                        else {
                            free(num);
                            parameter[0] = '\000';
                            return NUMBER_EXPECTED;
                        }
                    }
                    else {
                        type = type_index;
                        free(num);
                        parameter[i] = '\0';
                        break;
                    }
                }
            if(parameter[i] != '\0'){
                parameter[0] = '\000';
                return ILLEGAL_CONTEXT;
            }
        }
        if(firstPass == FALSE){
            if((returned = isLabelDefined(parameter)) == FALSE){
                parameter[0] = '\000';
                return UNKNOWN_LABEL;
            }
            else{
                *value = getSymbol(parameter);
                if(returned == exter){
                    type += directExter;
                    type *= -1;
                }   
                else type += directInter;
            }
        }
        else {
            *value = 0;
            type += directInter;
        }
    }
    return type;
}

/* check if a label is a legal label */
int checkLabel(char* cLabel){
    int i;
    if(getFirstWord(cLabel) != FALSE) 
        return LABEL_CANT_BE_SAME_NAME_AS_COMMAND;
    else if(isRegister(cLabel) != FALSE)
        return LABEL_CANT_BE_SAME_NAME_AS_REGISTER;
    else if(!(isalpha(cLabel[0])))
        return ILLEGAL_LABEL;
    else for(i = 1; i < strlen(cLabel); i++)
        if(!isalnum(cLabel[i]))
            return ILLEGAL_LABEL;
    if(isLabelDefined(cLabel) != FALSE)
        return USED_LABEL;
    return TRUE;
}