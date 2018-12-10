#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string.h>
#include <sys/mman.h>

sem_t * s14, s24, s35, s45; 

void p(int i){
    printf("Process: %d\n", (i+1));
}

// untested
int main(){
    s14 = sem_open("/mutex14", O_CREAT | O_EXCL, 0644, 0);
    s24 = sem_open("/mutex24", O_CREAT | O_EXCL, 0644, 0);
    s35 = sem_open("/mutex35", O_CREAT | O_EXCL, 0644, 0);
    s45 = sem_open("/mutex45", O_CREAT | O_EXCL, 0644, 0);

    for(int i = 0; i < 5; i++){
        
        int pid = fork();
        if(pid == 0){
            switch (i+1){
                /* Δi */
                case 1:
                    p(i);
                    sem_post(s14);
                    break;
                case 2:
                    p(i);
                    sem_post(s24);
                    break;
                case 3:
                    p(i);
                    sem_post(s35);
                    break;
                case 4:
                    sem_wait(s14); sem_wait(s24);
                    p(i);                    
                    sem_post(s45);
                    break;
                case 5:
                    sem_wait(s35); sem_wait(s45);
                    p(i);
                    break;
            }
            return 0;
        }
    }

    for(int i = 0; i < 5; i++){
        wait(NULL);
    }

    sem_unlink("/mutex14");
    sem_close(s14); 

    sem_unlink("/mutex24");
    sem_close(s24); 

    sem_unlink("/mutex35");
    sem_close(s35); 
    
    sem_unlink("/mutex45");
    sem_close(s45); 

    return 0;
}