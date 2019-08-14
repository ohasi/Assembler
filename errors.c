#include "assembler.h"

int errNum;

void err_init(){
    errNum = 0;
}

/* prints information about an error to the standard output */
int err(int errCode, char* line, int lnNum){ 
    if(errCode != LONELY_LABEL && errCode != USELESS_LABEL)
        fprintf(stderr, "assembler: ERROR: ");
    else fprintf(stderr, "assembler: WARNING: ");
    switch (errCode){
        case NO_SOURCE_FILE:
            errNum++;
            fprintf(stderr, "no input files, assembly terminated.\n");
            exit(NO_SOURCE_FILE);
            break;
        case CANT_OPEN_FILE: 
            errNum++;
            fprintf(stderr, "can't open file %s.as\n", src->fileName);
            break;
        case CANT_WRITE_TO_FILE:
            errNum++;
            fprintf(stderr, "can't write to file %s.as\n", src->fileName);
            break;
        case ILLEGAL_LABEL:
            errNum++;
            fprintf(stderr, "illegal label.\n");
            break;
        case OUT_OF_MEMORY:
            errNum++;
            fprintf(stderr, "out of memory, assembly terminated.\n");
            exit(OUT_OF_MEMORY);
            break;
        case INSTRUCTION_AFTER_FIRST_WORD:
            errNum++;
            fprintf(stderr, "instruction after first word.\n");
            break;
        case ILLEGAL_FIRST_WORD:
            errNum++;
            fprintf(stderr, "illegal first word.\n");
            break;
        case UNKNOWN_COMMAND_OR_INSTRUCTION:
            errNum++;
            fprintf(stderr, "unknown command or instruction.\n");
            break;
        case LABEL_CANT_BE_SAME_NAME_AS_COMMAND:
            errNum++;
            fprintf(stderr, "a label cannot have the same name as a command.\n");
            break;
        case LONELY_LABEL:
            fprintf(stderr, "label with no use or value. ignoring.\n");
            break;
        case USED_LABEL:
            errNum++;
            fprintf(stderr, "this label is already in use.\n");
            break;
        case NUMBER_EXPECTED:
            errNum++;
            fprintf(stderr, "number expected.\n");
            break;
        case TOO_MANY_PARAMETERS:
            errNum++;
            fprintf(stderr, "too many parameters.\n");
            break;
        case STRING_EXPECTED:
            errNum++;
            fprintf(stderr, "string expected.\n");
            break;
        case UNKNOWN_LABEL:
            errNum++;
            fprintf(stderr, "undefined label.\n");
            break;
        case USELESS_LABEL:
            fprintf(stderr, "label with no use or value. ignoring.\n");
            break;
        case NOT_ENOUGH_PARAMETERS:
            errNum++;
            fprintf(stderr, "not enough parameters.\n");
            break;
        case ILLEGAL_CONTEXT:
            errNum++;
            fprintf(stderr, "illegal context.\n");
            break;
        case UNKNOWN_MACRO:
            errNum++;
            fprintf(stderr, "undefined macro\n");
            break;
        case ILLEGAL_PARAMETER:
            errNum++;
            fprintf(stderr, "illegal parameter\n");
            break;
        case LABEL_CANT_BE_SAME_NAME_AS_REGISTER:
            errNum++;
            fprintf(stderr, "a label cannot have the same name as a register.\n");
            break;
        case INDEX_MUST_BE_BIGGER_THAN_ZERO:
            errNum++;
            fprintf(stderr, "an index's value must be bigger than or equal to zero.\n");
            break;
        case CANT_DELETE_FILE:
            errNum++;
            fprintf(stderr, "can't delete file %s.\n", line);
            break;
        case UNKNOWN_ERROR:
            errNum++;
            fprintf(stderr, "unknown error.\n");
            break;
        default:
            fprintf(stderr, "unknown error.\n"); 
    }
    if(lnNum > 0)
            fprintf(stderr, "at %s.as: %d:\n", src->fileName, lnNum);
    if(line != NULL)
        fprintf(stderr, "\t%s\n", line);
    return errCode;
}

int getErrNum(){
    return errNum;
}