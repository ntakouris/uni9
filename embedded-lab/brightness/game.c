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

#define MAX_TRIES 3
#define INITIAL_CYCLES 20
#define MIN_CYCLES 15

#define SCORE_TRESHOLD 80

#define STATE_WAITING 0
#define STATE_PLAYING 1

unsigned int game_state = STATE_WAITING;

void FIQ_handler(void);
PIO *pioa = NULL;
AIC *aic = NULL;
TC *tc = NULL;

// pins for output 
unsigned int out = MODE == MODE_SEGMENT ? 0xFF : 0x3FF;
unsigned int button = 0x400; // button on pin 11 (1024d)

unsigned int button_state = BUT_IDLE;

unsigned int pwm = 0;
unsigned int tries = 0;
unsigned int cycles = INITIAL_CYCLES;

unsigned int won_game = 0;
unsigned int lost_game = 0;
unsigned int next_level = 0;

int main(int argc, const char *argv[])
{
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

    printf("Click button to start playing\n");

    char c;
    while ((c = getchar()) != 'e'){
        
        if (next_level == 1) {
            printf("Next level! cycles = %u\n", cycles);
            pwm = 0;
            tries = 0;

            next_level = 0;
        }

        if (lost_game == 1) {
            printf("You lost at cycles = %u\n", cycles);
        }

        if (won_game == 1) {
            printf("You won!\n");
        }

        if (lost_game == 1 || won_game == 1) {
            // reset
            pwm = 0;
            tries = 0;
            cycles = INITIAL_CYCLES;

            won_game = 0;
            lost_game = 0;

            game_state = STATE_WAITING;

            printf("Click button to play again\n");
        }

        pwm++;

        // clamp and check
        if (pwm > 100) {
            if (tries < MAX_TRIES) {
                tries++;
            } else {
                pwm = 0;
            }
        }	
        
        printf("pwm = %u", pwm);

        // pwm modulation %
        for (int k = 0; k < cycles; k++) {
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

            // if pending reset ignore interrupt
            if (won_game == 1 | lost_game == 1) {
                return;
            }

            // ∆ΙΑΚΟΠΤΗΣ ΠΑΤΗΜΕΝΟΣ;
            if (button_state == BUT_IDLE)
            {
                button_state = BUT_PRESSED;

                // treat button press here

                // start game
                if (game_state == STATE_WAITING) {
                    game_state = STATE_PLAYING;
                } else if (game_state == STATE_PLAYING) {
                    if (pwm >= SCORE_TRESHOLD) {
                        // success
                        if (cycles >= MIN_CYCLES) {
                            cycles--;
                            // next level
                            next_level = 1;
                        } else {
                            // you won
                            won_game = 1;
                        }
                    } else {
                        if (tries < MAX_TRIES) {
                            tries++;
                            pwm = 0;
                        } else {
                            // game over
                            lost_game = 1;
                        }
                    }
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
