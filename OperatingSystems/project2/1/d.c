#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

// 100 -> 0s (sorry)
// 100000 -> 1s 
// 1000000 -> 6s
// On MacBook Pro 13.3" 2017
#define PROC_NUM 1000000

// This is a bit more complex to have a bigger difference 
// Without many processes
//Be sure to compile with explicit 'turn off optimizations' flag -O0
int nothing(){
    int x = 0;
    x++;
    int y = 5;
    y--;
    return y - x;
}

int main()
{
    time_t start, end;

    start = time(NULL);

    printf("Start time: %ld s\n", start);

    for(int i = 0; i < PROC_NUM; i++){
        int pid = fork();

        if(pid == 0){
            nothing();
            return 0;
        }
    }

    for(int i = 0; i < PROC_NUM; i++){
        wait(NULL);
    }

    end = time(NULL);

    printf("End time: %ld s\n", end);

    printf("Diff time: %ld s\n", (end - start));

    return 0;
}