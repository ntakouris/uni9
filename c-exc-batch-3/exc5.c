#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int main(void){

    FILE * fo;
    FILE * fn;
    char lineo[256];
    char * linen[256];
    char line[256];

    fo = fopen("/home/ubuntu/workspace/c-exc-batch-3/file_old.txt", "r");
    fn = fopen("/home/ubuntu/workspace/c-exc-batch-3/file_new.txt", "r");

    if (fo == NULL || fn == NULL){
        return 0;
    }

    int counter = 0;
    while (fgets(lineo, sizeof(line), fo) != NULL && fgets(linen, sizeof(line), fn) != NULL) {
        counter++;
        if(strcmp(lineo, linen) != 0){
            printf("---Line %d\n", counter);
            printf("file_old: %s", lineo);
            printf("file_new: %s", linen);
        }
    }

    fclose(fo);
    fclose(fn);
    
    return 0;
}