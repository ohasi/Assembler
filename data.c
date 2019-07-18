#include "assembler.h"

/* adds an entry to the data table */
int addSymbol(char* dLabel, int dValue, int sType){
    if((sizeof(symbolList) / sizeof(Symbol)) == SC - 1)
        if((symbolList = (Symbol*) realloc(symbolList, sizeof(symbolList) + ((MAXSIZE / 4) * sizeof(Symbol)))) == NULL)
            err(OUT_OF_MEMORY, NULL, 0);
    if((symbolList[SC].label = malloc(sizeof(dLabel))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    strcpy(symbolList[SC].label, dLabel);
    symbolList[SC].value = dValue;
    symbolList[SC].type = sType;
    SC++;
    return 0;
}

int addData(int value){
    if((sizeof(dataList) / sizeof(int)) == DC - 1)
        if((dataList = (int*) realloc(dataList, sizeof(dataList) + ((MAXSIZE / 4) * sizeof(int)))) == NULL)
            err(OUT_OF_MEMORY, NULL, 0);
    dataList[DC] = value;
    DC++;
    return 0;
}