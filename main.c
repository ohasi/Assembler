
#include "assembler.h"

int main(int argc, char* argv[]){
    int i_main;
    if (argc < 2)  {
        err(NO_SOURCE_FILE, NULL, 0);      
        return NO_SOURCE_FILE;  
    }
    else {
        initFiles();
        for(i_main = 1; i_main < 2; i_main++){
            loadSrcFile(argv[i_main]);
            assemble();
        }
    }
    return 0;
}