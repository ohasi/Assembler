
#include "assembler.h"

#define addExt(dest, str1, str2) dest = strcat(str1, str2); /* adds str3 to the end of str2, and stores the result in str1 */

void data_init();
void data_free();
/* loads the specified file, in the mode (w,r) right for it. */
int loadFile(char* fExt){
    char* path, *mode;
    if(strcmp(fExt, ".as") == 0)
        mode = "r";
    else mode = "w";
    addExt(path, src->fileName, fExt);
    if((src->source = fopen(path, mode)) == NULL)
        return err(CANT_OPEN_FILE, NULL, 0);
    return TRUE;
}
/* loads a new source file */
int loadSrcFile(char* name){
    data_init();
    errNum = 0;
    firstPass = TRUE;
    closeFiles();
    src->fileName = name;
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
    data_free();
}

void initFiles(){
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
    if(checkLabel(entryLabel) == USED_LABEL){
        entryValue = getSymbol(entryLabel);
        fprintf(src->entries, "%s %4d", entryLabel, entryValue);   
        return TRUE;
    }
    else return UNKNOWN_LABEL;
}