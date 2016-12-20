#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define M 15

int main(void){
    
    printf("A> ");
    char a[M];
    fgets(a , M , stdin);
    a[M] = '\0';
    printf("You entered: %s", a);
    
    printf("B> ");
    char b[M];
    fgets(b, M , stdin);
    b[M] = '\0';
    printf("%s", b);
    
    char * words = malloc(M * sizeof(char));
    
    int i = 0;
    for(i=0;i<M;i++){
        strcpy((words+i), "");
    }
    
    char * token;
    
    token = strtok(a, " ");
   
    /* walk through other tokens */
     while( token != NULL ) 
    {
      printf("Token: %s\n", token);
      int index = -1;
      int contains = 0;
      for(i=0;i<M;i++){
          if(strcmp((words+i), token) == 0){
              contains = 1;
              index = i;
              break;
          }
      }
    
      if(contains == 0){
          for(i=0;i<M;i++){
              if(strcmp((words+i), "") == 0){//not contained
                  strcpy((words+i), token);
                  break;
              }
          }
      }
    
      token = strtok(NULL, " ");
     }
     
     for(i=0;i<M;i++){
        if(strcmp((words+i), "") == 0){
            break;
        }   
        
        printf("Word: %s", words+i);
        getchar();
        char *needle = malloc(M * sizeof(char));
        char *p = b;
        while ( (p=strstr(p,needle)) != NULL ) {
                printf("-%ld", p-b);
                p += strlen(needle);
        }
        
     }
    
    return 0;
}