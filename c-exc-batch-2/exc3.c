#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int reduceIntArrayToBool(int * array, int size){

    int ret = 1;
    int i = 0;
    for(i = 0 ; i < size; i++){
        ret = *(array+i) ? 1 : 0;
        if(ret == 0){
            break;
        } 
    }

    return ret;
}

char * hiddenWord(char * word, int * ints , int size){
    char *ret = malloc(sizeof(char) * size);
    int i = 0;
    for(i = 0; i < size;i++){
        if(ints[i]){
            *(ret+1) = *(word+i);
        }else{
            *(ret+i) = '_';
        }
    }
    return ret;
}

int existsInWord(char c, char * word, int size){
    int i = 0;
    for(i = 0;i<size;i++){
        if(*(word+i) == c){
            return 1;
        }
    }
    return 0;
}

void charFound(char * word, int * ints , char c ,int size){
    int i = 0;
    for(i = 0;i<size;i++){
        if(*(word+i) == c){
            *(ints+i) = 1;
            printf("%d", *(ints+1));
        }
    }
}

int isCharAlreadyFound(char * word, int * ints , char c ,int size){
    int i = 0;
    for(i = 0;i<size;i++){
        //printf("word: %c , int: %d\n" , *(word+i) , *(ints+1));
        if(*(word+i) == c && *(ints+1) == 1){
            //printf("Letter already found\n");
            return 1;
        }
    }
    return 0;
}


int main(void){

    printf("Word game\n");
    char target[100];

    scanf("%s", target);
    
    int wordSize;

    int i = 0;
    for(i=0;i<100;i++){
        if(target[i]=='\0'){
            wordSize = i;
            break;
        }
    }

    int found[wordSize];
    int triesLeft = 5;
    char word[wordSize];
    
    for(i=0;i<wordSize;i++){
        word[i] = target[i];
        found[i]=0;
    }

    while(!reduceIntArrayToBool(found, wordSize) && triesLeft > 0){
        
        for(i=0;i<wordSize;i++){
            printf("Letter: %c - %d  ", *(word+i), *(found+1));
        }
        printf("\n");

        char * toPrint = hiddenWord(word, found, wordSize);
        printf("Word: %s\n", toPrint);
        char c;
        scanf("%c" , &c);
        scanf("%c", &c);

        int alreadyFound = isCharAlreadyFound(word, found , c , wordSize);

        if(alreadyFound){
            printf("Letter already found! Please try a different one!");
            continue;
        }

        int bExistsInWord = existsInWord(c, word, wordSize);

        if(bExistsInWord){
            printf("Letter found!");
            charFound(word, found , c, wordSize);
        }else{
            printf("Oops!\n");
            triesLeft--;
        }

        printf("\n\n");
    }

    if(triesLeft == 0){
        printf("No more tries left\n");
    }else{
        printf("Congrantulations!\n");
    }

    return 0;
}