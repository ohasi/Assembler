
#include "assembler.h"

void files_init();
int loadSrcFile(char* name);
void assemble();

int main(int argc, char* argv[]){
    int i;
    if (argc < 2)  {
        err(NO_SOURCE_FILE, NULL, 0);      
        return NO_SOURCE_FILE;  
    }
    else {
        for(i = 1; i < argc; i++){
            files_init();
            if(loadSrcFile(argv[i]) != TRUE)
                continue;
            assemble();
        }
    }
    return 0;
}