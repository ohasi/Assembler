#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE  1 /* true value. used for boolean-like expresions  */
#define FALSE 0 /* false value. used for boolean-like expresions */
#define MAXSIZE 256
/* error codes */
enum errors{
    UNKNOWN_ERROR = 1,
    NO_SOURCE_FILE,
    CANT_OPEN_FILE,
    CANT_WRITE_TO_FILE,
    ILLEGAL_LABEL,
    OUT_OF_MEMORY,
    INSTRUCTION_AFTER_FIRST_WORD,
    ILLEGAL_FIRST_WORD,
    UNKNOWN_INSTRUCTION,
    UNKNOWN_COMMAND,
    LABEL_CANT_BE_SAME_NAME_AS_COMMAND,
    LONELY_LABEL,
    USED_LABEL,
    NUMBER_EXPECTED,
    TOO_MANY_PARAMETERS,
    STRING_EXPECTED,
    UNKNOWN_LABEL
};

enum { 
    macro, 
    code
};
/* structure to hold environment addresses of files */
typedef struct {
    FILE *source, *entries, *externals, *object;
    char* fileName;
} File;
/* structure that represents a word in memory */
typedef struct {
    unsigned int ARE : 2;
    unsigned int DEST : 2;
    unsigned int SRC : 2;
    unsigned int OPCODE : 4;
    unsigned int EXT : 4;
} FirstWord;

typedef struct {
    unsigned int ARE: 2;
    unsigned int VALUE: 12;
} Word;

typedef struct {
    unsigned int VALUE: 14
} InstructionWord;
/* structure to hold a data entry's name and value */
typedef struct {
    char* label;
    int value;
    int type;
} Symbol;

int DC, /* counts amount of data entrys */
    IC, /* counts amount of instructions */
    SC,
    firstPass;
    *dataList;
Symbol* symbolList; /* holds all data entrys of the file */
File* src; /* holds environment addresses for the current source file */

int loadFile(char* fExt); /* loads the current file, in the mode (w,r) right for it. */
int loadSrcFile(char* name);
int assemble();
int addSymbol(char* dLabel, int dValue, int type);
int addData(int value);
void closeFiles();
void initFiles();
void err(int errCode, char* word, int lnNum);

#endif