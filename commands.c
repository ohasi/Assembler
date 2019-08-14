
#include "assembler.h"

void writeExtern(char* externLabel, int value);
int isLegalNumber(char* number);
void changeData(long value);
int writeEntry(char* entryLabel);
int getParameter(char* parameter, int *value);
void addData(long value);
int isLegalNumber(char* number);

/* handles the .data command, returns TRUE if everything went smoothly. otherwise, returns an error code */
int data(){
    char* nextData;                                   /* holds the next data (number \ macro) in string form */
    while((nextData = strtok(NULL, ",\n")) != NULL){  /* as long as ther's more data */
        if(firstPass == TRUE){                          /* if it is the first pass in the file */
            if(isalpha(nextData[0])){                       /* if data is alphabetic */
                if(isLabelDefined(nextData) == macro)           /* if it is a macro's label */
                    addData(getMacro(nextData));                    /* add the macro's value to the dataList */
                else return UNKNOWN_MACRO;                      /* if its not return an error */
            }
            else if(!isLegalNumber(nextData))               /* if data is neither alphabetic or a number */
                return NUMBER_EXPECTED;                         /* return an error */
            else addData(atoi(nextData));                   /* if data is not alphabetic and it is a number, add it to the dataList */
        }
    }
    return TRUE; /* if its the second pass, return true */
}

/* handles the .string command, returns TRUE if everything went smoothly. otherwise, returns an error code */
int string(){
    int i;
    char* nextWord; /* holds the next word in the input line */
    if(firstPass == TRUE){ /* if it's the first pass on the file */
        nextWord = strtok(NULL, " \n"); /* get the next word */
        if(strtok(NULL, " \n") != NULL) /* if there are too many parameters */
            return TOO_MANY_PARAMETERS; 
        else if (!(nextWord[0] == '\"' && nextWord[strlen(nextWord) - 1] == '\"')) /* if string isn't inside parantheses */
            return STRING_EXPECTED;
        else for(i = 1; i < strlen(nextWord) - 1; i++) /* if string is inside parantheses, add the string to the dataList */
            addData(nextWord[i]);
        addData('\0'); /* add a closing 0 to the end of the string in the dataList */
        return TRUE;
    }
    else return TRUE; /* if its the second pass, return true */
}

/* handles the .entry command, returns TRUE if everything went smoothly. otherwise, returns an error code */
int entry(){
    int returned; /* holds the value returned from a function called inside entry */
    char* nextWord; /* holds the next word in the input line */
    if(firstPass == FALSE) { /* if it's the second pass on the file */
        nextWord = strtok(NULL, " \n"); /* get the next word */
        if(strtok(NULL, " \n") != NULL) /* if there are too many parameters */
            return TOO_MANY_PARAMETERS;
        else if((returned = checkLabel(nextWord)) != USED_LABEL || getSymbol(nextWord) == macro){ /* if parameter is illegal */
                if(returned == USED_LABEL)
                    returned = ILLEGAL_PARAMETER;
                return returned;
            }
        else return writeEntry(nextWord);
    }
    else return TRUE; /* if it's the first pass, return true */
}

/* handles the .extern command, returns TRUE if everything went smoothly. otherwise, returns an error code */
int external(){ 
    int returned; /* holds the value returned from a function called inside external */
    char* nextWord; /* holds the next word in the input line */
    if(firstPass == TRUE){ /* if it's the first pass n the file */
        nextWord = strtok(NULL, " \n"); /* get the next word */
        if(strtok(NULL, " \n") != NULL) /* if there are too many parameters */
                return TOO_MANY_PARAMETERS;
        else if((returned = checkLabel(nextWord)) != TRUE) /* if label is illegal */
            return returned;
        else addSymbol(nextWord, 0, exter); /* add the label to the symbolList */
    }
    return TRUE; /* if it's the second pass on the file, return true */
}

/* handles the .define command, returns TRUE if everything went smoothly. otherwise, returns an error code */
int define(){
    int returned,   /* holds the value returned from a function called inside external */
        cValue;     /* holds the value of the macro */
    char *nextWord, /* holds the next word in the input line */
         *cLabel;   /* holds the macro's label */
    if(firstPass == TRUE){ /* if it's the first pass on the file */
        if((cLabel = strtok(NULL, " ")) == NULL) /* if there isn't a label in the line */
            return NOT_ENOUGH_PARAMETERS;
        else if(!(returned  = checkLabel(cLabel))) /* if label is illegal */
            return returned; /* return the error code */
        else { /* if label is legal */
            nextWord = strtok(NULL, " "); /* check if next word is a = sign */
            if(strcmp(nextWord, "=") != 0)
                return ILLEGAL_CONTEXT; /* if not return error */
            else {
                nextWord = strtok(NULL, "\n"); /* get the macro's value */
                if(!isLegalNumber(nextWord)) /* if value isn't a number */
                    return NUMBER_EXPECTED;
                else { /* if value is a number */
                    cValue = atoi(nextWord); /* get the value */
                    if ((nextWord = strtok(NULL, " ")) == NULL){ /* if there aren't extra words on the line */
                        addSymbol(cLabel, cValue, macro);
                        return TRUE;
                    }
                    else return TOO_MANY_PARAMETERS; /* if there are extra words on the line */
                }
            }
        }
        return UNKNOWN_ERROR; /* ift his line was somehow reached */
    }
    return TRUE; /* if it's the second pass on the file, return TRUE */
}

/* >if parameter is legal return the parameter's addressing type and sets the value and ARE fields of the corresponding word(s) to the correct value for the parameter.
   otherwise, returns an error code, ad sets line to NULL. 
   >PARAM par:    the parameter to work on
   >PARAM line:   the binary line to put the data into
   >PARAM length: the length of the line in binary without the parameter being parsed and any parameters coming after it */
int paramToWord(char* par, Word** line, int length){
    int parType,  /* holds the addressing type of the parameter */
        parValue, /* holds the value of the parameter - the register's number, the macro's value, etc */
        tmp = 0;  /* used to hold temporary values that are later put into other variables */
    parType = getParameter(par, &parValue); /* get the parameter's type */
    (*line)[length].VALUE = 0; /* initialize the value */
    if(par[0] == '\000'){ /* if there's an error in the parameter */
        free(*line);
        (*line) = NULL;
        return parType;
        }
    else if(parType == reg){ /* if parameter is a register */
        (*line)[length].VALUE += parValue; /* add the register's number to the correspinding word in the line */
        if(length == 1)                    /* if it's the source operand */
            (*line)[length].VALUE <<= 3;   /* move it 3 bits left */
        (*line)[length].ARE = 0;           /* set ARE in the correpsonding word to 0 */
        return 3;                          /* return 3: register */
    }
    else if(parType == immediate){ /* if addressing type is immidiate */
        (*line)[length].ARE = 0;          /* set ARE in the correpsonding word to 0 */
        (*line)[length].VALUE = parValue; /* set the value in the correpsonding word to the value stored in the parameter */
        return 0;                         /* return 0: immidiate */
    }
    else { /* if addressing type is direct or index */
        if(parType < 0){        /* if the label is external */
            tmp = directExter;  
            parType *= -1; 
        }
        else tmp = directInter; 
        (*line)[length].ARE = tmp; /* set ARE in the correpsonding word to 01 or 10, depending if the addressing type is direct external or direct internal */
        (*line)[length].VALUE = parValue; /* set the value field in the correpsonding word to the parameter's value */
        if(parType - tmp == 0) /* if the addressing type is not index, then parType - tmp will be equal to 0. if it isn't, it means addressing type is index */
            return 1;
        else { /* if addressing type is index */
            parType -= (tmp + type_index); /* get the index */
            if((realloc(*line, sizeof(Word) * (length + 1))) == NULL) /* add another word to the line, and check for memory errors */
                return err(OUT_OF_MEMORY, NULL, 0);
            (*line)[length + 1].ARE = 0; /* set ARE in the correpsonding word to 0 */
            (*line)[length + 1].VALUE = parType; /* set the value field in the corresponding word to the index */
            return 2; /* return 2: addressing type index */
        }
    }
    return FALSE; /* if parameter doesn't fall into any of the criteria checked above, return FALSE */
}

/* >convert a command with 2 parameters to binary. If there are no errors, returns the amount of words the line takes in binary.
   otherwise, set line to NULL and return the error code
   >PARAM line:  the binary line o put the data into
   >PARAM cmdId: the ID of the command tho work on */
int twoParamCommand(Word** line, int cmdId){
    char *par1,    /* holds the first parameter of the command in string form */
         *par2,    /* holds the second parameter of the command in string form */
         returned; /* holds the value returned from functions called inside this function */
    int length;    /* holds the number of binary words generated from this command */
    length = 1; /* set length to 1 */
     if((par1 = strtok(NULL, ",")) == NULL) /* if there isn't a first parameter */
        return err(NOT_ENOUGH_PARAMETERS, NULL, 0); 
    else if((par2 = strtok(NULL, " \n")) == NULL) /* if there isn't a second parameter */
        return err(NOT_ENOUGH_PARAMETERS, NULL, 0); 
    else if(((*line) = (Word*) calloc(2, sizeof(Word))) == NULL) /* memory allocation and error checking */
        return err(OUT_OF_MEMORY, NULL, 0);
    else if(strtok(NULL, "\n") != NULL){ /* if there are too many parameters */
        (*line) = NULL;
        return TOO_MANY_PARAMETERS;
    }
    (*line)[0].ARE = 0; /* set ARE in the first word to 0 (ARE is always 0 in the first word) */
    (*line)[0].VALUE = cmdId; /* add the command's ID to the first word's VALUE field */
    (*line)[0].VALUE <<= 2; /* move the first word's value field 2 bits to the left */
    returned = paramToWord(par1, line, length); 
    if(*line == NULL) /* error checking */
        return returned;
    (*line)[0].VALUE += returned; /* add the source parameter's addressing type to the first word's VALUE field */
    (*line)[0].VALUE <<= 2; /* move the first word's value field 2 bits to the left */
    if(returned == 2){ /* if addressing type was index */
        length += 2; /* add 2 to the amount of words */
        if((*line)[length - 1].ARE == 1) /* if there was an external reference */
            writeExtern(par1, addInstruction(0) + length - 1);
    }
    else if(returned == 1){ /* if addressing type was direct */
        length++; /* add 1 to the amount of words */
        if((*line)[length].ARE == 1) /* if there was an external reference */
            writeExtern(par1, addInstruction(0) + length - 1);
    }
    else length++; /* add 1 to the amount of words */
    if((realloc(*line, sizeof(Word) * (length + 1))) == NULL) /* memory allocation and error checking */
        return err(OUT_OF_MEMORY, NULL, 0);
    returned = paramToWord(par2, line, length);
    if(*line == NULL) /* error checking */
        return returned; 
    (*line)[0].VALUE += returned; /* add the destination parameter's addressing type to the first word's VALUE field */
    if(returned == 2){ /* if addressing type is index */
        if((*line)[length - 1].ARE == 1) /* if there was an external reference */
            writeExtern(par1, addInstruction(0) + length);
        length += 2; /* add 2 to the amount of words */
    }
    else if(returned == 1){ /* if addressing type was direct */
        if((*line)[length].ARE == 1) /* if there was an external reference */
            writeExtern(par1, addInstruction(0) + length);
        length++;
    }
    else if((*line)[0].VALUE - (cmdId * 16) == 15) /* if both parameters were registers */
        (*line)[1].VALUE += (*line)[2].VALUE; /* combine them to one word */
    else length++; /* add 1 to the amount of words */
    return length; /* return the amount of words */
}

/* >convert a command with one parameter to binary. If there are no errors, returns the amount of words the line takes in binary.
   otherwise, set line to NULL and return the error code
   >PARAM line:  the binary line o put the data into
   >PARAM cmdId: the ID of the command tho work on */
int oneParamCommand(Word** line, int cmdId){
    char *par;    /* holds the parameter in string form */
    int length,   /* holds the amount of binary words the command takes */
        returned; /* holds the value returned from functions called inside this one */
    length = 1; /* set the amount of words to 1 */
    if((par = strtok(NULL, " \n")) == NULL) /* if there isn't a parameter */
        return err(NOT_ENOUGH_PARAMETERS, NULL, 0);
    else if(((*line) = calloc(2, sizeof(Word))) == NULL) /* memory allocation and error checking */
        return err(OUT_OF_MEMORY, NULL, 0);
    else if(strtok(NULL, "\n") != NULL){ /* if there are too many parameters */
        (*line) = NULL;
        return TOO_MANY_PARAMETERS;
    }
    (*line)[0].ARE = 0;  /* set ARE in the first word to 0 (ARE is always 0 in the first word) */
    (*line)[0].VALUE = cmdId; /* add the command's ID to the first word's VALUE field */
    (*line)[0].VALUE <<= 4; /* move the first word's value field 4 bits to the left */
    returned = paramToWord(par, line, length);
    if(*line == NULL) /* error checking */
        return returned;
    (*line)[0].VALUE += returned; /* add the parameter's addressing type to the first word's VALUE field */
    if(returned == 3){ /* if addressing type is register */
        (*line)[1].VALUE >>= 2; /* move the VALUE field of he first word 2 bits to the right (it has been moved to the left in paramToWord which is good for 2 parameter commands but not for one parameter commands) */
        length++; /* add 1 to the amount of words */
    }
    else if(returned == 2){ /* if addressing type is index */
        if((*line)[length].ARE == 1) /* if there was an external reference */
            writeExtern(par, addInstruction(0) + length);
        length += 2; /* add 2 to the amount of words */
    }
    else if(returned == 1){ /* if addressing type was direct */
        if((*line)[length].ARE == 1) /* if there was an external reference */
            writeExtern(par, addInstruction(0) + length);
        length++; /* add 1 to the amount of words */
    }
    else length++; /* add 1 to the amount of words */
    return length; /* return the amount of words */
}