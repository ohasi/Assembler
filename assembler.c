
#include "assembler.h"

void closeFiles();
/* declares words for expressing each command's value */
enum cmdIds {mov, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, red, prn, jsr, rts, stop};

int oneParamCommand(Word** line, int cmdId);    /* for commands with one parameter - after its operation, line points to the current line in binary 14-bit. if theres an error in the current line, line is set to NULL */
int isFirstWordLabel(char* firstWord);          /* if the first word is a label, returns either TRUE or an error code of whats wrong with the label. otherwise, returns FALSE */
int getFirstWord(char* word);                   /* returns the command's id if first word is a command. otherwise, returns FALSE */
int data();                                     /* handles the .data command, returns TRUE if everything went smoothly. otherwise, returns an error code */
int string();                                   /* handles the .string command, returns TRUE if everything went smoothly. otherwise, returns an error code */                          
int external();                                 /* handles the .extern command, returns TRUE if everything went smoothly. otherwise, returns an error code */
int entry();                                    /* handles the .entry command, returns TRUE if everything went smoothly. otherwise, returns an error code */
int define();                                   /* handles the .define command, returns TRUE if everything went smoothly. otherwise, returns an error code */
int twoParamCommand(Word** line, int cmdId);    /* for commands with two parameters - after its operation, line points to the current line in binary 14-bit. if theres an error in the current line, line is set to NULL */
void prepDataForSecondPass();                   /* prepares all data for the second pass - adds IC to all data pointers, and resets IC to 0 */
void writeData();                               /* writes the data portion of the machine code to the .ob file */
void writeFirstLine();                          /* writes the first line, with the length of the instruction and data portions in the file, to the file */
void rmvLnSpaces(char* line);                   /* removes all spaces from a line, excluding spaces following a word or a symbol(,+- etc) */
void writeLine(Word* line, int length);         /* writes the given line to the file, in 7-bit machine code - if it's the second pass and there weren't any errors */         

/* one pass on the source file */
void pass(){
    Word *line;     /* holds the read line in binary */
    char *currLine, /* the line currently being interpreted */
         *currWord, /* the word currently being interpreted */
         *readLine; /* the line that was read, unchanged */
    int lnNum,      /* the number of the line currently being interpreted */
        returned;   /* the value returned from a function called from this function */
    /* memory allocation */
    if((currLine = (char*) calloc(MAXSIZE, sizeof(char))) == NULL) 
        err(OUT_OF_MEMORY, NULL, 0);
    if((readLine = (char*) calloc(MAXSIZE, sizeof(char))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    lnNum = 1; /* set the line number to 1 when starting to go over the file */
    while((fgets(currLine, MAXSIZE, src->source)) != NULL){ /* as long as there's another line in the source file */
        strcpy(readLine, currLine); /* save the line as-is before interpreting it */
        rmvLnSpaces(currLine);
        currWord = strtok(currLine, " \n"); /* get the first word */
        if(currWord == NULL || currLine[0] == ';'){ /* if there are no words in the line, or the line is a comment line, skip to the next line */
            lnNum++;                                
            continue; 
        }
        /* checking if first word is a label. if it is and there are errors regarding it, calls err to print them and proceeds to the next line */
        returned = isFirstWordLabel(currWord); 
        if(returned == TRUE || returned == USELESS_LABEL){
            if((currWord = strtok(NULL, " \n")) == NULL){
                if(firstPass == TRUE)
                    err(LONELY_LABEL, readLine, lnNum);
                continue;
            }
        }
        else if(returned != FALSE) {
            if(firstPass == TRUE)
                err(returned, readLine, lnNum);
            continue;
        }
        /* if first word is illegal, print an error and proceed to the next line */
        if((returned = getFirstWord(currWord)) == FALSE) {  
            if(firstPass == TRUE)
                err(UNKNOWN_COMMAND_OR_INSTRUCTION, readLine, lnNum);
            continue;
        }
        /* if first word is legal */
        else switch (returned){ 
            /* switch form:
                :call command
                :if there's an error with the command
                    :print error 
                :if not, and command is not instruction (.data, .exter, etc) 
                    :call writeLine
                    like
            in some commands, there;s a line similar to this:
                if((3 & line[0].VALUE) == 0) 
            this checks if the addressing method is viable for this command, an prints an error if it isn't. */
            case -1: /* .data */
                if((returned = data()) != TRUE)
                    err(returned, readLine, lnNum);
                break;
            case 1: /* .string */
                if((returned = string()) != TRUE)
                    err(returned, readLine, lnNum);
                break;
            case 2: /* .entry */
                if((returned = entry()) != TRUE)
                    err(returned, readLine, lnNum);
                break;
            case 3: /* .extern */
               if((returned = external()) != TRUE)
                    err(returned, readLine, lnNum);
                break;
            case 4: /* .define */
                if((returned = define()) != TRUE)
                    err(returned, readLine, lnNum);
                break;
            case 10: /* mov */
                returned = twoParamCommand(&line, mov);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                if(((3 << 2) & line[0].VALUE) == 0 || (3 & line[0].VALUE) == 0)
                    if(firstPass == FALSE)
                        err(ILLEGAL_PARAMETER, readLine, lnNum);
                writeLine(line, returned);
                free(line);
                break;
            case 11: /* cmp */
                returned = twoParamCommand(&line, cmp);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                writeLine(line, returned);
                free(line);
                break;
            case 12: /* add */
                returned = twoParamCommand(&line, add);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                if((3 & line[0].VALUE) == 0)
                    if(firstPass == FALSE)
                        err(ILLEGAL_PARAMETER, readLine, lnNum);
                writeLine(line, returned);
                free(line);
                break;
            case 13: /* sub */
                returned = twoParamCommand(&line, sub);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                if((3 & line[0].VALUE) == 0)
                    if(firstPass == FALSE)
                        err(ILLEGAL_PARAMETER, readLine, lnNum);
                writeLine(line, returned);
                free(line);
                break;
            case 14: /* lea */
                returned = twoParamCommand(&line, lea);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                if((3 & line[0].VALUE) == 0)
                    if(firstPass == FALSE)
                        err(ILLEGAL_PARAMETER, readLine, lnNum);
                writeLine(line, returned);
                free(line);
                break;
            case 15: /* not */
                returned = oneParamCommand(&line, not);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                if((3 & line[0].VALUE) == 0)
                    if(firstPass == FALSE)
                        err(ILLEGAL_PARAMETER, readLine, lnNum);
                writeLine(line, returned);
                free(line);
                break;
            case 16: /* clr */
                returned = oneParamCommand(&line, clr);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                if((3 & line[0].VALUE) == 0)
                    if(firstPass == FALSE)
                        err(ILLEGAL_PARAMETER, readLine, lnNum);
                writeLine(line, returned);
                free(line);
                break;
            case 17: /* inc */
                returned = oneParamCommand(&line, inc);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                if((3 & line[0].VALUE) == 0)
                    if(firstPass == FALSE)
                        err(ILLEGAL_PARAMETER, readLine, lnNum);
                writeLine(line, returned);
                free(line);
                break;
            case 18: /* dec */
                returned = oneParamCommand(&line, dec);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                if((3 & line[0].VALUE) == 0)
                    if(firstPass == FALSE)
                        err(ILLEGAL_PARAMETER, readLine, lnNum);
                writeLine(line, returned);
                free(line);
                break;
            case 19: /* jmp */
                returned = oneParamCommand(&line, jmp);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                if((3 & line[0].VALUE) == 0 || (3 & line[0].VALUE) == 2)
                    if(firstPass == FALSE)
                        err(ILLEGAL_PARAMETER, readLine, lnNum);
                writeLine(line, returned);
                free(line);
                break;
            case 20: /* bne */
                returned = oneParamCommand(&line, bne);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                if((3 & line[0].VALUE) == 0 || (3 & line[0].VALUE) == 2)
                    if(firstPass == FALSE)
                        err(ILLEGAL_PARAMETER, readLine, lnNum);
                writeLine(line, returned);
                free(line);
                break;
            case 21: /* red */
                returned = oneParamCommand(&line, red);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                if((3 & line[0].VALUE) == 0)
                    if(firstPass == FALSE)
                        err(ILLEGAL_PARAMETER, readLine, lnNum);
                writeLine(line, returned);
                free(line);
                break;
            case 22: /* prn */
                returned = oneParamCommand(&line, prn);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                writeLine(line, returned);
                free(line);
                break;
            case 23: /* jsr */
                returned = oneParamCommand(&line, jsr);
                if(line == NULL){
                    err(returned, readLine, lnNum);
                    break;
                }
                if((3 & line[0].VALUE) == 0 || (3 & line[0].VALUE) == 2)
                    err(ILLEGAL_PARAMETER, readLine, lnNum);
                writeLine(line, returned);
                free(line);
                break;
            case 24: /* rts */
                if((line = malloc(sizeof(Word))) == NULL)
                    err(OUT_OF_MEMORY, NULL, 0);
                line->ARE = 0;
                line->VALUE = rts << 4;
                writeLine(line, 1);
                free(line);
                break;
            case 25: /* stop */
                if((line = malloc(sizeof(Word))) == NULL)
                    err(OUT_OF_MEMORY, NULL, 0);
                line->ARE = 0;
                line->VALUE = stop << 4;
                writeLine(line, 1);
                free(line);
                break;
            }
        lnNum++; /* add 1 to the line number, before going over to the next line */
    }
    /* deallocating memory */
    free(currLine);
    free(readLine);
}

/* runs the assembler on the current source file in src */
void assemble(){
    pass();                  /* first pass on te file */   
    firstPass = FALSE;       /* no longer in the first pass */                     
    writeFirstLine();       
    prepDataForSecondPass();
    rewind(src->source);     /* go back to the beggining of the source file */
    pass();                  /* second pass on the file */
    writeData();             
    closeFiles();
}