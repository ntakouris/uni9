#include <stdio.h>
#include <math.h>

int main(void){

    int numbers = 10;
    int ints[numbers];

    int i;
    for(i = 0 ; i < numbers ;i++){
        scanf("%d", ints+i);
    }

    int maxOccurences = -1;
    int maxOccurenceElement;

    int currentOccurences;
    int currentElement;

    for(i = 0 ; i < numbers ;i++){
        currentOccurences = -1;
        currentElement = ints[i];
        int j;
        for(j=0;j < numbers; j++){
            int e = ints[j];
            if(currentElement == e){
                currentOccurences++;
            }
        }
        if(currentOccurences > maxOccurences){
            maxOccurences = currentOccurences;
            maxOccurenceElement = currentElement;
        }
    }

    printf("Max occurence(s): %d - Number: %d\n", maxOccurences, maxOccurenceElement);


    return 0;
}