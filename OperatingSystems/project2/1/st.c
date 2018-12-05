#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string.h>

#define PROC_NUM 100

sem_t * mutex; 


// MacOS supports only named semaphores
int main(){
    char * p = ""; // shared text

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
        nSeconds[i] = i; // % n for random 0 ~ n
    }

    printf("Spawning children\n");
    for(int i = 0; i < PROC_NUM; i++){
        
        int pid = fork();
        if(pid == 0){
            /* ==== */
                sleep(nSeconds[i]);

                const char target = txt[i]; // character to concat to *p

                sem_wait(mutex); 
                char * pOld = p;

                size_t len;
                if(p == NULL){
                    len = 0;
                }else{
                    len = strlen(p);
                }

                //Malloc new string and swap pointers
                char * p2 = malloc(len + 1 + 1 ); /* one for extra char, one for trailing null/zero */

                if(p != NULL){
                    strcpy(p2, p);
                }

                p2[len] = txt[i];
                p2[len + 1] = '\0';

                printf("%s====\n", p2);

                &p = &p2;

                printf("%s----\n", *p);

                sem_post(mutex); 
            /* ==== */
            return 0;
        }
    }

    for(int i = 0; i < PROC_NUM; i++){
        wait(NULL);
    }

    printf("\n");

    printf("Concatenated string: %s\n", *p);

    sem_unlink("/mutex");
    sem_close(mutex); 

    return 0;
}