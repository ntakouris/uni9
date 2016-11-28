#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

char * asterisks(int num){

    if(num <= 0){
        char * ret = malloc(sizeof(char));
        *ret = ' ';
        return ret;
    }

    char *ret = malloc(sizeof(char) * num);
    for(int i = 0; i < num; i++){
        ret[i] = '*';
    }

    return ret;
}

int main(void){

    
    char sentence[] = " 1 2 3 aa bb ggg cc abc fgh 666666    7777777 ";
    int char_number = sizeof(sentence)/sizeof(sentence[0]);;

    //get max word size
    int maxWordSize = 0;
    int wordSize = 0;
    for(int i = 0; i < char_number; i++){
        if(sentence[i] == ' ' || sentence[i] == '\0'){
            if(wordSize == 0){
                continue;
            }
            if(wordSize > maxWordSize){
                maxWordSize = wordSize;
            }
            wordSize = 0;
            continue;
        }
        if(i == char_number - 1){
            if(wordSize!=0){
                    wordSize++;
                    if(wordSize > maxWordSize){
                        maxWordSize = wordSize;
                    }
            }
        }else{
            wordSize++;
        }
    }

    printf("Max word size: %d\n", maxWordSize);
    //initialize occurences with 0s
    int occurences[maxWordSize + 1];
    for(int i = 1; i < maxWordSize + 1; i++){
        occurences[i] = 0;
    }

    //populate occurences
    wordSize = 0;
    for(int i = 0; i < char_number; i++){
        if(sentence[i] == ' ' || sentence[i] == '\0'){
            if(wordSize == 0){
                continue;
            }
            occurences[wordSize]++;
            wordSize = 0;    
            continue;
        }
        wordSize++;
        if(i == char_number){
            occurences[wordSize]++;
        }
    }

    //print results
    printf("Word Length\t|Number of Occurences\n");

    for(int i = 1; i < maxWordSize + 1; i++){
        char * asterisk = asterisks(occurences[i]);
        printf("%d\t\t|%s\n", i, asterisk);
        //printf("%d\t\t|%d\n", i, occurences[i]);
    }


    return 0;
}

