#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TRUE  1     /* true value. used for boolean-like expresions  */
#define FALSE 0     /* false value. used for boolean-like expresions */
#define MAXSIZE 256 /* max size reference accross the program. used for memory allocation */

/* error codes. start from 2 to avoid overlap with true or false */
enum errors{
    UNKNOWN_ERROR = 2,
    NO_SOURCE_FILE,
    CANT_OPEN_FILE,
    CANT_WRITE_TO_FILE,
    ILLEGAL_LABEL,
    OUT_OF_MEMORY,
    INSTRUCTION_AFTER_FIRST_WORD,
    ILLEGAL_FIRST_WORD,
    UNKNOWN_COMMAND_OR_INSTRUCTION,
    LABEL_CANT_BE_SAME_NAME_AS_COMMAND,
    LONELY_LABEL,
    USED_LABEL,
    NUMBER_EXPECTED,
    TOO_MANY_PARAMETERS,
    STRING_EXPECTED,
    UNKNOWN_LABEL,
    USELESS_LABEL,
    NOT_ENOUGH_PARAMETERS,
    ILLEGAL_CONTEXT,
    UNKNOWN_MACRO,
    ILLEGAL_PARAMETER,
    LABEL_CANT_BE_SAME_NAME_AS_REGISTER,
    INDEX_MUST_BE_BIGGER_THAN_ZERO,
    CANT_DELETE_FILE
};

enum dataTypes { macro = 2, code, exter, type_data }; /* the different data types labels can refernce to - macro, code, external, and data. values start from 2 to avoid conflict with TRUE an FALSE */
enum addressingMethod { directExter = 1, directInter, immediate, reg, type_index }; /* the different addressing methods - direct external, direct internal, immediate, register, and index */

/* structure to hold environment addresses of the source and output files */
typedef struct {
    FILE *source, *entries, *externals, *object;
    char* fileName;
} File;

/* structure to represent a 14 bit binary word */
typedef struct {
    unsigned int ARE: 2,
                 VALUE: 12;
} Word;

File* src;     /* holds environment addresses for the current source and output files */
int firstPass; /* holds a boolen value (true / false). determines whether or not the current pas is the first pass */

int getSymbol(char* sLabel);                        /* returns the value of a given symbol, if its not a macro */
int checkLabel(char* cLabel);                       /* checks if a label is a legal label. returns TRUE if it is, otherwise returns an error code */
int getMacro(char* mLabel);                         /* returns the value of a given symbol, if its a macro */
int isLabelDefined(char* label);                    /* if label is defined, returns label's type. If it isn't, returns FALSE */
int addInstruction(int toAdd);                      /* adds toAdd to IC (the instruction counter) and returns the value of IC before adding toAdd */
int err(int errCode, char* line, int lnNum);        /* prints information about an error to the standard output */
int getDC();                                        /* returns the current value of the data counter */
void addSymbol(char* dLabel, int dValue, enum dataTypes dType); /* adds a symbol to the symbol list */

#endif