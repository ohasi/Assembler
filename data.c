#include "assembler.h"
#include <ctype.h>

typedef struct {
    char* label;
    int value;
    int type;
} Symbol;

int DC, /* counts amount of data entrys */
    IC, /* counts amount of instructions */
    SC,
    *dataList;
Symbol* symbolList; /* holds all data entrys of the file */

int getFirstWord(char* word);

/* adds an entry to the data table */
void data_init(){
    DC = 0;
    IC = 100;
    SC = 0;
    if((symbolList = (Symbol*) calloc(MAXSIZE / 4, sizeof(Symbol))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    if((dataList = (int*) calloc(MAXSIZE / 4, sizeof(int))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
}

void data_free(){
    free(dataList);
    free(symbolList);
}

void addSymbol(char* dLabel, int dValue, int sType){
    if((sizeof(symbolList) / sizeof(Symbol)) == SC - 1)
        if((symbolList = (Symbol*) realloc(symbolList, sizeof(symbolList) + ((MAXSIZE / 4) * sizeof(Symbol)))) == NULL)
            err(OUT_OF_MEMORY, NULL, 0);
    if((symbolList[SC].label = malloc(sizeof(dLabel))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    strcpy(symbolList[SC].label, dLabel);
    symbolList[SC].value = dValue;
    symbolList[SC].type = sType;
    SC++;
}

void addData(int value){
    if((sizeof(dataList) / sizeof(int)) == DC - 1)
        if((dataList = (int*) realloc(dataList, sizeof(dataList) + ((MAXSIZE / 4) * sizeof(int)))) == NULL)
            err(OUT_OF_MEMORY, NULL, 0);
    dataList[DC] = value;
    DC++;
}

int getSymbol(char* symbol){
    int i_getSymbol;
    for(i_getSymbol = 0; i_getSymbol < SC; i_getSymbol++)
        if(strcmp(symbol, symbolList[i_getSymbol].label) == 0 && symbolList[i_getSymbol].type != macro)
            return symbolList[i_getSymbol].value;
    return FALSE;
}

int getMacro(char* macroLabel){
    int i_getMacro;
    for(i_getMacro = 0; i_getMacro < SC; i_getMacro++)
        if(strcmp(macroLabel, symbolList[i_getMacro].label) == 0 && symbolList[i_getMacro].type != macro)
            return symbolList[i_getMacro].value;
    return FALSE;
}

void nextInstruction(){
    IC++;
}

int getIC(){
    return IC;
}

int getDC(){
    return DC;
}
/* check if a label is a legal label */
int checkLabel(char* cLabel){
    int i_checkLabel;
    if(getFirstWord(cLabel) != -1) 
        return LABEL_CANT_BE_SAME_NAME_AS_COMMAND;
    else if(!(isalpha(cLabel[0])))
        return ILLEGAL_LABEL;
    else for(i_checkLabel = 1; i_checkLabel < strlen(cLabel); i_checkLabel++)
        if(!isalnum(cLabel[i_checkLabel]))
            return ILLEGAL_LABEL;
    for(i_checkLabel = 0; i_checkLabel < SC; i_checkLabel++)
        if(strcmp(symbolList[i_checkLabel].label, cLabel) == 0)
            return USED_LABEL;
    return TRUE;
}

int isMacroDefined(char* macroLabel){
    int i_isMacroDefined;
    for(i_isMacroDefined = 0; i_isMacroDefined < SC; i_isMacroDefined++)
            if(strcmp(symbolList[i_isMacroDefined].label, macroLabel) == 0 && symbolList[i_isMacroDefined].type == macro)
                return TRUE;
    return FALSE;
}