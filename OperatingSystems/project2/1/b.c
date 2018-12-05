#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int pid;

    for (int i = 0; i < 4; i++){
        pid = fork();
        if(pid == 0){
            // kid #i 
            printf("%d\n", i);
            break;
        }
    }
}