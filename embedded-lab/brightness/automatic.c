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

#define BUT_IDLE 0

#define BUT_PRESSED 1
#define BUT_RELEASED 2

#define MODE_SEGMENT 0
#define MODE_LEDBAR 1
#define MODE MODE_LEDBAR

# define BRIGHTNESS_MOVING 0
# define BRIGHTNESS_IDLE 100

# define DIRECTION_UP 0
# define DIRECTION_DOWN 1

void FIQ_handler(void);
PIO *pioa = NULL;
AIC *aic = NULL;
TC *tc = NULL;

// pins for output 
unsigned int out = MODE == MODE_SEGMENT ? 0xFF : 0x3FF;
unsigned int button = 0x400; // button on pin 11 (1024d)
unsigned int brightness = BRIGHTNESS_IDLE;
unsigned int direction = DIRECTION_DOWN;

unsigned int button_state = BUT_IDLE;

int main(int argc, const char *argv[])
{

    unsigned int pwm = 100;
    unsigned int gen;
    STARTUP;

    aic->FFER = (1 << PIOA_ID);
    // ΟΙ ∆ΙΑΚΟΠΕΣ 2, 17 ΕΙΝΑΙ ΤΥΠΟΥ FIQ
    aic->IECR = (1 << PIOA_ID);
    // ΕΝΕΡΓΟΠΟΙΗΣΗ ∆ΙΑΚΟΠΩΝ : PIOA
    pioa->PUER = button;
    // ΕΝΕΡΓΟΠΟΙΗΣΗ ΣΤΗ ΓΡΑΜΜΗ 11 : PULL−UP 
    pioa->ODR = button;
    // ΓΡΑΜΜΗ 0 : ΛΕΙΤΟΥΡΓΙΑ ΕΙΣΟ∆ΟΥ 

    // high output
    pioa->SODR = out;
    // enable those for output
    pioa->OER = out;

    // clear any pending misfired interrupts
    gen = pioa->ISR;

    // gpio enable
    pioa->PER = 0x4FF; // button + out leds

    // PIOA : ΕΚΚΑΘΑΡΙΣΗ ΑΠΟ ΤΥΧΟΝ ∆ΙΑΚΟΠΕΣ 
    aic->ICCR = (1 << PIOA_ID);
    // AIC : ΕΚΚΑΘΑΡΙΣΗ ΑΠΟ ΤΥΧΟΝ ∆ΙΑΚΟΠΕΣ 
    pioa->IER = button;

    char c;
    while ((c = getchar()) != 'e'){
        
        if (brightness == BRIGHTNESS_MOVING) {
            if (direction == DIRECTION_DOWN)
            {
                pwm--;
            } else {
                pwm++;
            }
        }

        // clamp and direction flip
        if (pwm == 0){ pwm = 0; direction = DIRECTION_UP;}
        if (pwm == 100) {pwm = 100; direction = DIRECTION_DOWN;}	
        
        printf("pwm = %u\n", pwm);

        // pwm modulation %
        for (int k = 0; k < 20; k++) {
            for (int i = 0; i < 100; i++) {
                if (i <= pwm) {
                    pioa->SODR=out; 
                } else {
                    pioa->CODR=out;
                }
            }
        }
    }

    aic->IDCR = (1 << PIOA_ID);

    CLEANUP;
    return 0;
}

void FIQ_handler(void)
{
    unsigned int data_in = 0;
    unsigned int fiq = 0;
    unsigned int data_out;
    fiq = aic->IPR;
    // ΕΝΤΟΠΙΣΜΟΣ ΠΕΡΙΦΕΡΕΙΑΚΟΥ ΠΟΥ ΠΡΟΚΑΛΕΣΕ ΤΗ ∆ΙΑΚΟΠΗ 
    if (fiq & (1 << PIOA_ID)) 
    {
        // ΕΛΕΓΧΟΣ ΓΙΑ PIOA 
        data_in = pioa->ISR;
        // ΕΚΚΑΘΑΡΙΣΗ ΤΗΣ ΠΗΓΗΣ ΤΗΣ ∆ΙΑΚΟΠΗΣ 
        aic->ICCR = (1 << PIOA_ID);
        // ΕΚΚΑΘΑΡΙΣΗ ΤΗΣ ∆ΙΑΚΟΠΗΣ ΑΠΟ A I C 
        data_in = pioa->PDSR;
        // ΑΝΑΓΝΩΣΗ ΤΙΜΩΝ ΕΙΣΟ∆ΟΥ 
        if (!(data_in & button)) // button down
        {
            // ∆ΙΑΚΟΠΤΗΣ ΠΑΤΗΜΕΝΟΣ;
            if (button_state == BUT_IDLE)
            {
                button_state = BUT_PRESSED;

                if (brightness == BRIGHTNESS_MOVING) {
                    brightness = BRIGHTNESS_IDLE;
                } else {
                    brightness = BRIGHTNESS_MOVING;
                }
            }
        }
        else
        {
            if (button_state == BUT_PRESSED){
                button_state = BUT_IDLE;
            }
        }
    }
}
