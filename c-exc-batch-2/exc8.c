#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int main(void){

    int n;
    do{
        printf("Enter n>1: ");
        scanf("%d", &n);
    }while(n < 1);

    int arr[n-1];
    int i, j;
    printf("Initializing array\n");
    for(i=0;i<n-1;i++){
        arr[i] = 1;
    }

    printf("Performing heavy calculations\n");
    for(i=2;i<sqrt(n);i++){
        if(arr[i]){
            for(j=(i*i); j < n-1 ; j+=i){
                arr[j] = 0;
            }
        }
    }

    printf("Results\n");
    for(i=0;i<n-1;i++){
        if(arr[i]){
            printf("%d\n", i);
        }
    }

    return 0;

}