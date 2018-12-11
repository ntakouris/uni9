#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

// 100     -> 0s (sorry)     | 0s (sorry)
// 100000  -> 1s             | 6s
// 1000000 -> 6s             | 59s
// On MacBook Pro 13.3" 2017 | On Celeron N3350 2.4 GHz, Debian 9.6 (Native)
#define PROC_NUM 100000

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
    printf("Average time: %0.9f s\n", (end - start)/(float)PROC_NUM);

    return 0;
}