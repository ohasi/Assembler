#include "assembler.h"

/* structure to represent a symbol. stores a label, a value and a type */
typedef struct {
    char* label;
    int value;
    enum dataTypes type;
} Symbol;

int DC, /* counts amount of data entrys */
    IC, /* counts amount of instructions */
    SC; /* counts amount of symbols */
long *dataList;     /* holds all data entrys of the file */
Symbol* symbolList; /* holds all symbol entrys of the file */

/* initialize data variables */
void data_init(){
    /* initialie DC, IC and SC to their correc values */
    DC = 0;
    IC = 100;
    SC = 0;
    /* memory allocation and error checking */
    if((symbolList = (Symbol*) calloc(MAXSIZE / 4, sizeof(Symbol))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    if((dataList = (long*) calloc(MAXSIZE / 4, sizeof(int))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    symbolList[0].label = NULL;
}

/* free all memory used by data */
void data_free(){
    free(dataList);
    free(symbolList);
}

/* >add a symbol to symbolList.
   >PARAM sLabel: the symbol's label
   >PARAM sValue: the symbol's value
   >PARAM sType: the symbol's type (macro, code, etc.) */
void addSymbol(char* sLabel, int sValue, enum dataTypes sType){
     if(SC != 0 && SC % 64 == 0) /* if symbolList is full */
        if((symbolList = (Symbol*) realloc(symbolList, (SC + (MAXSIZE / 4)) * sizeof(Symbol))) == NULL) /* memory allocation and error checking */
            err(OUT_OF_MEMORY, NULL, 0);
    if((symbolList[SC].label = malloc(sizeof(char) * (strlen(sLabel) + 1))) == NULL) /* memory allocation and error checking */
        err(OUT_OF_MEMORY, NULL, 0);
    strcpy(symbolList[SC].label, sLabel);   /* set the correct label */
    symbolList[SC].value = sValue;          /* set the correct value */
    symbolList[SC].type = sType;            /* set the correct type */
    SC++; /* add 1 to the symbol counter */
}

/* >add data to the dataList.
   >PARAM value: the value to add to the dataList */
void addData(long value){
    if(DC != 0 && DC % 64 == 0) /* if dataList is full */
        if((dataList = (long*) realloc(dataList, (DC + (MAXSIZE / 4)) * sizeof(long))) == NULL) /* memory allocation and error checking */
            err(OUT_OF_MEMORY, NULL, 0);
    dataList[DC] = value; /* add the value to the dataList */
    DC++; /* add 1 to the dataCounter */
}

/* >get the value of a given symbol (not macro!). If symbol exists, return the value of the symbol. Otherwise, return FALSE.
   >PARAM sLabel: the symbol's label */
int getSymbol(char* sLabel){
    int i;
    for(i = 0; i < SC; i++) /* search for the symbol */
        if(strcmp(sLabel, symbolList[i].label) == 0 && symbolList[i].type != macro)
            return symbolList[i].value; /* if symbol is found, return it's value */
    return FALSE; /* if symbol isn't found, return FALSE */
}

/* >get the value of a given macro (not symbol!). If macro exists, return the value of the macro. Otherwise, return FALSE.
   >PARAM mLabel: the macro's label */
int getMacro(char* mLabel){
    int i;
    for(i = 0; i < SC; i++) /* search for the macro */
        if(strcmp(mLabel, symbolList[i].label) == 0 && symbolList[i].type == macro)
            return symbolList[i].value; /* if macro is found, return it's value */
    return FALSE; /* otherwise, return FALSE */
}

/* >add toAdd to IC, and return the value of IC before the change.
   >PARAM toAdd: tha value to add to IC */
int addInstruction(int toAdd){
    int toReturn = IC; /* get IC's value before the change */
    IC += toAdd; /* add toAdd to IC */
    return toReturn; /* return the value before the change */
}

/* returns DC */
int getDC(){
    return DC;
}

/* >check if a label is defined. If it is, return it's type. Otherwise, return FALSE
   >PARAM label: the label to search for */
int isLabelDefined(char* label){
    int i;
    for(i = 0; i < SC; i++) /* search for the label */
            if(strcmp(symbolList[i].label, label) == 0)
                return symbolList[i].type; /* if label is found, return it's type */
    return FALSE; /* if it isn;t found, return FALSE */
}

/* prepare the data values of several integers to the second pass */
void prepDataForSecondPass(){
    int i;
    /* add IC to the value of all symbols that "point" to data */
    if(symbolList[0].label != NULL)
        for(i = 0; i < SC; i++)
            if(symbolList[i].type == type_data)
                symbolList[i].value += (IC);
    IC = 100; /* reset IC */
}

/* return the value of the data at the given index. NOTICE: there isn't any good way to notice when an error occurred, as data might have the value of FALSE */
int getData(int index){
    if(index >= DC)  /* if index is invalid */
        return FALSE; /* return FALSE */
    else return dataList[index]; /* else return the value of the data at the given index */
}

/* >change the value of the first occurrance of a data without value to the given value
   >PARAM value: the value to change to */
void changeData(long value){
    int i;
    for(i = 0; i < DC; i++) /* search for data without value */
        if(dataList[i] == 8192) 
            dataList[i] = value; /* if it's found, change it's value. otherwise, the function does nothing */
}