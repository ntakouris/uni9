#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct node{

    int data;
    struct node *next;

} Node;

Node * GetLast(Node **headRef){

    if(*headRef == NULL){
        return NULL;
    }

    Node *curr = *headRef;
    while((*curr).next != NULL){
        curr = (*curr).next;
    }

    return curr;
}

void Insert(Node **headRef, int newData){

    Node *toAdd = malloc(sizeof(Node));
    (*toAdd).data = newData;
    (*toAdd).next = NULL;

    if(*headRef == NULL){
        
        *headRef = toAdd;
        return;
    }

    Node *last = GetLast(headRef);
    (*last).next = toAdd;
}

int Delete(Node **headRef){

    if(*headRef == NULL){
        return INT_MIN;
    }

    Node *curr = *headRef;
    Node *next = (*curr).next;

    *headRef = next;

    int data = (*curr).data;
    free(curr);

    return data;
}

int GetNth(Node **headRef, int index){

    if(*headRef == NULL){
        return INT_MIN;
    }

    int i;
    Node *curr = *headRef;
    for(i=0;i<index;i++){
        if((*curr).next == NULL){//index out of bounds - return data value of last element
            printf("Out of bounds\n");
            break;
        }
        curr = (*curr).next;
    }
    return (*curr).data;
}

int Count(Node **headRef, int searchFor){

    if(*headRef == NULL){
        return -1;
    }

    int i = 0;
    Node *curr = *headRef;
    for(;;){
        if((*curr).data == searchFor){
            break;
        }
        if(!(*curr).next){//index out of bounds - return index of last element
            break;
        }
        curr = (*curr).next;
        i++;
    }
    return i;
}

int main(void){

    int input;
    Node *emptyPtr = NULL;
    Node **headRef = &emptyPtr;

    do{
        printf("1.Insert\n2.Delete\n3.Get-Nth\n4.Count\n5.Exit\n\n");
        scanf("%d", &input);

        int d;
        switch(input){
            case 1:
                printf("Enter new data to insert> ");
                scanf("%d", &d);
                Insert(headRef , d);
                break;
            case 2: 
                printf("%d\n", Delete(headRef));
            case 3:
                printf("Enter index to count> ");
                scanf("%d", &d);
                printf("%d\n", GetNth(headRef , d));
                break;

            case 4:
                printf("Search for int> ");
                scanf("%d", &d);
                printf("%d\n", Count(headRef , d));
                break;
            default:
                break;
        }
    }while(input != 5);


    return 0;
}