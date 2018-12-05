#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

//If childpid = -1, calling process did not create a child one
//Only happens for last one (#5)
void print_data(int childpid){
    printf("%d %d %d\n", getppid(), getpid(), childpid);
}

int main()
{
    int pid;
    for (int i = 0; i < 4; i++){
        pid = fork();
        if(pid != 0){
            // parent #i n
            print_data(pid);
            wait(NULL);
            break;
        }else{
            if(i == 3){
                print_data(-1);
            }
        }
    }

    return 0;
}