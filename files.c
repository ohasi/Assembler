
#include "assembler.h"

#define addExt(dest, str1, str2) dest = strcat(str1, str2); /* adds str3 to the end of str2, and stores the result in str1 */
/* loads the specified file, in the mode (w,r) right for it. */
int loadFile(char* fExt){
    char* path, *mode;
    if(strcmp(fExt, ".as") == 0)
        mode = "r";
    else mode = "w";
    addExt(path, src->fileName, fExt);
    if((src->source = fopen(path, mode)) == NULL){
        err(CANT_OPEN_FILE, NULL, 0);
        return 1;
    }
    return 0;
}
/* loads a new source file */
int loadSrcFile(char* name){
    DC = 0;
    IC = 100;
    SC = 0;
    firstPass = TRUE;
    closeFiles();
    src->fileName = name;
    if((symbolList = (Symbol*) calloc(MAXSIZE / 4, sizeof(Symbol))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    if((dataList = (int*) calloc(MAXSIZE / 4, sizeof(int))) == NULL)
        err(OUT_OF_MEMORY, NULL, 0);
    return loadFile(".as");
}

/* closes currently open Files */
void closeFiles(){
    if(src->entries != NULL)
        fclose(src->entries);
    if(src->externals != NULL)
        fclose(src->externals);
    if(src->object != NULL)
        fclose(src->object);
    if(src->source != NULL)
        fclose(src->source); 
    free(dataList);
    free(symbolList);
}

void initFiles(){
    src->entries = NULL;
    src->externals = NULL;
    src->fileName = NULL;
    src->object = NULL;
    src->source = NULL;
}