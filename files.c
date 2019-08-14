
#include "assembler.h"

#define FOURLENGTH 7

void data_init();
void data_free();
void err_init();
int getErrNum();
int getData(int index);

/* loads the specified file, in the mode (w,r) right for it. */
int loadFile(char* fExt){
    char* path, *mode;
    path = malloc((strlen(src->fileName) + 1) * sizeof(char));
    strcpy(path, src->fileName);
    strcat(path, fExt);
    if(strcmp(fExt, ".as") == 0)
        mode = "r";
    else mode = "w";
    if(strcmp(fExt, ".as") == 0){
        if((src->source = fopen(path, mode)) == NULL)
            return err(CANT_OPEN_FILE, NULL, 0);
    }
    else if(strcmp(fExt, ".ob") == 0){
        if((src->object = fopen(path, mode)) == NULL)
            return err(CANT_OPEN_FILE, NULL, 0);
    }
    else if(strcmp(fExt, ".ent") == 0){
        if((src->entries = fopen(path, mode)) == NULL)
            return err(CANT_OPEN_FILE, NULL, 0);
    }
    else if(strcmp(fExt, ".ext") == 0){
        if((src->externals = fopen(path, mode)) == NULL)
            return err(CANT_OPEN_FILE, NULL, 0);
    }
    free(path);
    return TRUE;
}
/* loads a new source file */
int loadSrcFile(char* name){
    data_init();
    err_init();
    firstPass = TRUE;
    src->fileName = name;
    return loadFile(".as");
}

void delFile(const char* fExt){
    char* fName;
    if((fName = (char*) malloc((strlen(src->fileName) + 5) * sizeof(char))) == NULL) /* memory allocation and error checking */
        err(OUT_OF_MEMORY, NULL, 0);
    strcpy(fName, src->fileName);
    if(remove(strcat(fName, fExt)) != 0)
        err(CANT_DELETE_FILE, fName, 0);
    free(fName);
}
/* closes currently open Files */
void closeFiles(){
    if(src->entries != NULL){
        if(getErrNum() > 0)
            delFile(".ent");
        else fclose(src->entries);
    }
    if(src->externals != NULL){
        if(getErrNum() > 0)
            delFile(".ext");
        else fclose(src->externals);
    }
    if(src->object != NULL){
        if(getErrNum() > 0)
            delFile(".ob");
        else fclose(src->object);
    }
    if(src->source != NULL)
        fclose(src->source); 
    data_free();
    free(src);
}

void files_init(){
    if((src = malloc(sizeof(File))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    src->entries = NULL;
    src->externals = NULL;
    src->fileName = NULL;
    src->object = NULL;
    src->source = NULL;
}

int writeEntry(char* entryLabel){
    int entryValue;
    if(src->entries == NULL)
        loadFile(".ent");
    if(isLabelDefined(entryLabel) > 0){
        entryValue = getSymbol(entryLabel);
        fprintf(src->entries, "%s %04d\n", entryLabel, entryValue);   
        return TRUE;
    }
    else return UNKNOWN_LABEL;
}

void writeExtern(char* externLabel, int value){
    if(src->externals == NULL)
        loadFile(".ext");
    fprintf(src->externals, "%s %04d\n", externLabel, value);   
}

char* decimalToFour(int decimal){
    int i, currentBits;
    char* four;
    if((four = calloc(8, sizeof(char))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    for(i = 0; i < FOURLENGTH; i++){
        currentBits = (decimal >> (i*2)) & 3;
        switch(currentBits){
            case 0:
                four[FOURLENGTH - i - 1] = '*';
                break;
            case 1:
                four[FOURLENGTH - i - 1] = '#';
                break;
            case 2:
                four[FOURLENGTH - i - 1] = '%';
                break;
            case 3:
                four[FOURLENGTH - i - 1] = '!';
                break;
        }
    }
    four[7] = '\0';
    return four;
}

char* binaryToFour(Word* binary){
    int fullWord;
    char* four;
    fullWord = binary->VALUE * 4 + binary->ARE;
    four = decimalToFour(fullWord);
    return four;
}

void writeLine(Word* line, int length){
    int i;
    if(firstPass == FALSE && getErrNum() == 0){
        if(src->object == NULL)
            loadFile(".ob");
        for(i = 0; i < length; i++)
            fprintf(src->object, "%04d %7s\n", addInstruction(1), binaryToFour(line + i));
    }
    else addInstruction(length);
}

void writeData(){
    int i;
    if(firstPass == FALSE && getErrNum() == 0){
        if(src->object == NULL){
            loadFile(".ob");}
        for(i = 0; i < getDC(); i++)
            fprintf(src->object, "%04d %7s\n", addInstruction(1), decimalToFour(getData(i)));
    }        
}

void writeFirstLine(){
    if(firstPass == FALSE && getErrNum() == 0){
        if(src->object == NULL)
            loadFile(".ob");
        fprintf(src->object, "%d %d\n", addInstruction(0) - 100, getDC());
    }
}