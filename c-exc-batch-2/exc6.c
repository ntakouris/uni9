#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


void print2dArr(int ** arr , int n , int m){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            printf("%d \t" , *(*(arr+i)+j));
        }
        printf("\n");
    } 
}

int * sums(int ** arr , int n , int m){
    int *ret = malloc(sizeof(int) * m);
    int i,j;
    for(i=0;i<n;i++){
        int s = 0;
        for(j=0;j<m;j++){
            s+= *(*(arr+i)+j);
        }
        *(ret+i) = s;
    } 

    return ret;
}

int main(void){

    int n,m;

    do{
        printf("n> ");
        scanf("%d", &n);
    }while(n <= 0);

    do{
        printf("m> ");
        scanf("%d", &m);
    }while(m <= 0);

    int i,j;

    int **arr = malloc(sizeof(int*) * n);
    for(i=0;i<n;i++){
        *(arr+i)=malloc(sizeof(int) * m);
    }

    printf("Fill me up!\n");

    int e;
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            printf("Element (%d, %d) > ", i , j);
            scanf("%d", &e);
            *(*(arr+i)+j) = e;
        }
    }
    printf("\n\n");
    print2dArr(arr, n , m);
    printf("\n\n");

    int * sumArr = sums(arr, n , m);

    for(i=0;i<n;i++){
            printf("Sum of row %d = %d\n" , i , *(sumArr+i));
    }

    printf("\n\n");

    return 0;
}