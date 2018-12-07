#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string.h>
#include <sys/mman.h>

#define PROC_NUM 100

sem_t * mutex; 

static char * p;
static int * cur_i;

// MacOS supports only named semaphores
/* Sometimes results do not get printed out, must run 2-3 times -- WTF*/
int main(){
    p = mmap(NULL, sizeof(char) * PROC_NUM, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); // shared text
    cur_i = mmap(NULL, sizeof(* cur_i), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); // shared index

    *cur_i = 0;

    memset(p, 0, sizeof(char) * PROC_NUM);

    //Length of 'txt' should be equal to PROC_NUM
    const char txt[PROC_NUM] = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean m";

    /* 
    Η αλλαγή που χρειάζεται για να βγει σωστό κείμενο είναι να βάλουμε τους χρόνους nSeconds[index] να αυξάνονται όσο αυξάνεται και το index ώστε να
    γίνουν όλα με τη σειρά. Αυτό όμως δεν είναι σωστή λύση γιατί το sleep() δεν μας εγγυάται ότι μια διεργασία θα παίξει αφού τελειώσει αμέσως, άρα
    εάν έχουμε 2+ διεργασίες που έχουν τελειώσει τον ύπνο, μπορεί να τρέξει πρώτα κάποια που τέλειωσε τον ύπνο πιο πρόσφατα και όχι πιο νωρίς.

    Ο σωστός τρόπος θα ήταν να έχουμε σημαφόρους που καθορίζουν την προτεραιότητα μεταξύ διεργασιών όπως έχουμε ήδη δει με διαγράμματα προτεραιό
    */
    int nSeconds[PROC_NUM];

    mutex = sem_open("/mutex", O_CREAT | O_EXCL, 0644, 0);

    printf("Initialising nSeconds\n");
    for(int i = 0; i < PROC_NUM; i++){
        nSeconds[i] = rand() % 6; // % n for random between 0 ~ n
        // nSeconds[i] = i; // for easy fix
    }

    printf("Spawning children\n");
    for(int i = 0; i < PROC_NUM; i++){
        
        int pid = fork();
        if(pid == 0){
            /* ==== */
                sleep(nSeconds[i]);
                const char target = txt[i]; // character to concat to *p
                printf("%d      ", i);
                sem_wait(mutex); 
                    printf("%s\n", p, *cur_i, target);

                    p[*cur_i] = target;
                    *cur_i = *cur_i + 1;
                sem_post(mutex); 
            /* ==== */
            return 0;
        }
    }

    for(int i = 0; i < PROC_NUM; i++){
        wait(NULL);
    }

    printf("\n");

    printf("Concatenated string: %s\n", p);

    sem_unlink("/mutex");
    sem_close(mutex); 

    munmap(p, sizeof(char *));

    return 0;
}