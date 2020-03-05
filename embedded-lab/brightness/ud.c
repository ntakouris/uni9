#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <header.h>
#define PIOA_ID 2

#define MODE_SEGMENT 0
#define MODE_LEDBAR 1
#define MODE MODE_LEDBAR

// pins for output 
unsigned int out = MODE == MODE_SEGMENT ? 0xFF : 0x3FF;

PIO *pioa = NULL;

int main(int argc, const char *argv[])
{

    unsigned int pwm = 100;
    unsigned int gen;
    STARTUP;

    // high output
    pioa->SODR = out;
    // enable those for output
    pioa->OER = out;
    // gpio enable
    pioa->PER = out;

    char c;
    while ((c = getchar()) != 'e'){
        
        if (c == 'd') 
        {
            pwm--;
        }
        if (c == 'u'){
            pwm++;
        }

        // clamp
        if (pwm < 0){ pwm = 0;}
        if (pwm > 100) {pwm = 100;}	
        
        printf("pwm = %u", pwm);

        // pwm modulation %
        for (int i = 0; i < 100; i++) {
            if (i <= pwm) {
                pioa->SODR=out; 
            } else {
                pioa->CODR=out;
            }
        }
        
    }

    CLEANUP;
    return 0;
}