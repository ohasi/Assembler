
#include "assembler.h"

int main(int argc, char* argv[]){
    int fi;
    if (argc < 2)  {
        err(NO_SOURCE_FILE, NULL, 0);      
        return NO_SOURCE_FILE;  
    }
    else {
        initFiles();
        for(fi = 1; fi < 2; fi++){
            loadSrcFile(argv[fi]);
            assemble();
        }
    }
    return 0;
}