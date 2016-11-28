#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 5
#define M 32

struct word_pair{
    int length;
    char word[M];
};

int isValid(char * arr, int size){

    int i;
   // printf("%s", arr);
    for(i=0;i<size;i++){   
        if(isalnum(*(arr+i)) == 0){
            return 0;
        }
    }

    return 1;
}

int len(char * arr){

    int size = sizeof(arr) / sizeof(arr[0]);
    int i;
    for(i=0;i<size;i++){
        if(*(arr+i) == '\0'){
            return i;
        }
    }
    return -1;
}


void initialize(struct word_pair word_table[], int size){

    int i;
    
    for(i=0;i<size;i++){
        char wrd[M];
        do{
            scanf("%s", wrd);
        }while(isValid(wrd, M) == 0 || len(wrd) != M);

        int w_size = len(wrd);

        strcpy(word_table[i].word, wrd);
        word_table[i].length = w_size;
    }

}

void replace(struct word_pair word_table[], int size, char search_string[], char replacement_string[]) {
    int i=0;
    for(i=0;i<size;i++){
        if(strcmp(word_table[i].word, search_string)){
            strcpy(word_table[i].word ,replacement_string);
        }
    }

}

void sub_sebsequence(struct word_pair word_table[], int size, char search_string[]){
    int i=0;
    for(i=0;i<size;i++){
        if(strstr(word_table[i].word, search_string)){
            printf("Found matching entry: %s\n" , word_table[i].word);
        }
    }
}



void translate(struct word_pair word_table[], int size) {

    char lexicon[][2][10]={
        {"table", "tafel"},
        {"word", "wort"},
        {"car", "auto"},
        {"tree", "baum"},
        {"number", "anzahl"},
        {"drive" , "fahern"},
        {"bicycle", "fahrrad"}
    };

    int i=0;
    for(i=0;i<size;i++){
        int j;
        for(j=0;j<7;j++){
            char key[10];
            char value[10];

            strcpy(key , lexicon[j][0]);
            strcpy(value , lexicon[j][1]);

            if(strcmp(word_table[i].word, key)){
                strcpy(word_table[i].word ,value);
                break;
            }
        }
            
    }

}



int main(void){

    //A
    /*struct word_pair word_table[N];
    initialize(word_table ,N);

    int i=0;
    for(i=0;i<N;i++){
        printf("%s\n", word_table[i].word);
    }*/

    //B



    //C

    return 0;
}