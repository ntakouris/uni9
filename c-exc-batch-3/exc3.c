#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define M 15

int prefix(const char *pre,const char *str)
{
    return strncmp(pre, str, strlen(pre));
}

int main(void){
    
    //printf("%d", prefix("a", "asdf"));
    //printf("\n");
  //  printf("%d", prefix("asdf", "a"));
    
    printf("A> ");
    char a[M];
    fgets(a , M , stdin);
    char *pos;
    if ((pos=strchr(a, '\n')) != NULL){
        *pos = '\0';
    }

    printf("You entered: %s\n", a);
    
    printf("B> ");
    char b[M];
    fgets(b, M , stdin);
    if ((pos=strchr(b, '\n')) != NULL){
        *pos = '\0';
    }
    printf("You entered: %s\n", b);
    
    char ** words = malloc(M * sizeof(char *));
    
    int i = 0;
    for(i=0;i<M;i++){
        *(words+i) = malloc(M * sizeof(char));
        strcpy(*(words+i), "");
    }
    
    char * token;
    
    token = strtok(a, " ");
   
    /* walk through other tokens */
     while( token != NULL) 
    {
        
      if(strcmp(token, " ") == 0){ continue;}
      int index = -1;
      int contains = 0;
      for(i=0;i<M;i++){
          if(strcmp(*(words+i), token) == 0){
              contains = 1;
              index = i;
              break;
          }
      }
    
      if(contains == 0){
          for(i=0;i<M;i++){
              if(strcmp(*(words+i), "") == 0){//not contained
                  if ((pos=strchr(token, '\n')) != NULL){
                    *pos = '\0';
                  }
                  strcpy(*(words+i), token);
                  break;
              }
          }
      }
    
      token = strtok(NULL, " ");
     }
     
     for(i=0;i<M;i++){
        if(strcmp(*(words+i), "") == 0){
            break;
        }   
        //for each word of a
        printf("Word: %s\n", *(words+i));
        //getchar();
        
        char * token;
        
        char * b_cpy = malloc(sizeof(b));
        strcpy(b_cpy, b);
        token = strtok(b_cpy, " ");
       
        /* walk through other tokens */
         while( token != NULL) 
        {
           // printf("token: %s - word: %s  \n", token , *(words+i));
            if(strcmp(token, *(words+i)) == 0){
                printf("%ld - ", token-b_cpy);    
            }
            
            token = strtok(NULL, " ");
        }
        
        
        printf("\n");
        
     }
    
    return 0;
}