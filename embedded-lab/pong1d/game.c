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
#define TC0_ID 17
#define BUT_IDLE 0

#define BUT_PRESSED 1
#define BUT_RELEASED 2
#define LED_IDLE 0
#define LED_FLASHING 1

void FIQ_handler(void);
PIO *pioa = NULL;
AIC *aic = NULL;
TC *tc = NULL;

unsigned int p1_a_state = BUT_IDLE;
unsigned int p1_b_state = BUT_IDLE;

unsigned int p2_a_state = BUT_IDLE;
unsigned int p2_b_state = BUT_IDLE;

// buttons
// a = simple pass
// b = strong pass
unsigned int p1_a = 0;
unsigned int p1_b = 1;

unsigned int p2_a = 1 << 12; // 12
unsigned int p2_b = 1 << 13; // 13

// leds from 2 ~ 11
#define BALL_START 2
#define BALL_END 11

unsigned int leds = 0b0000111111111100;

// game specific stuff
#define INITIAL_BALL_SPEED 5
int ball_speed = INITIAL_BALL_SPEED; // in 1/5 Hz

int ball_position = BALL_START;

int p1_score = 0;
int p2_score = 0;

int game_over = 0;

int blink_ball = 0;
int blink_ball_count = 0;

unsigned int ticks = 0;

int main(int argc, const char *argv[])
{
    unsigned int buttons = p1_a | p1_b | p2_a | p2_b;

    unsigned int gen;
    STARTUP;
    // ΑΡΧΙΚΟΠΟΙΗΣΗ ΣΥΣΤΗΜΑΤΟΣ 
    tc->Channel_0.RC = 8192;
    // ΠΕΡΙΟ∆ΟΣ 1 ∆ΕΥΤΕΡΟΛΕΠΤΟ 
    tc->Channel_0.CMR = 2084;
    // SLOW CLOCK, WAVEFORM, D ISABLE CLK ON RC COMPARE 
    tc->Channel_0.IDR = 0xFF;
    // ΑΠΕΝΕΡΓΟΠΟΙΗΣΗ ΟΛΩΝ ΤΩΝ ∆ΙΑΚΟΠΩΝ 
    tc->Channel_0.IER = 0x10;
    // ΕΝΕΡΓΟΠΟΙΗΣΗ ΜΟΝΟ ΤΟΥ RC COMPARE 
    aic->FFER = (1 << PIOA_ID) | (1 << TC0_ID);
    // ΟΙ ∆ΙΑΚΟΠΕΣ 2, 1 7 ΕΙΝΑΙ ΤΥΠΟΥ FIQ
    aic->IECR = (1 << PIOA_ID) | (1 << TC0_ID);
    // ΕΝΕΡΓΟΠΟΙΗΣΗ ∆ΙΑΚΟΠΩΝ : P IOA &TC0 
    pioa->PUER = buttons;
    // ΕΝΕΡΓΟΠΟΙΗΣΗ ΣΤΗ ΓΡΑΜΜΗ 0 : PULL−UP 
    pioa->ODR = buttons;
    // ΓΡΑΜΜΗ 0 : ΛΕΙΤΟΥΡΓΙΑ ΕΙΣΟ∆ΟΥ 
    pioa->CODR = leds;
    // ΓΡΑΜΜΗ 1 : ∆ΥΝΑΜΙΚΟ ΕΞΟ∆ΟΥ LOW 
    pioa->OER = leds;
    // ΓΡΑΜΜΗ 1 : ΛΕΙΤΟΥΡΓΙΑ ΕΞΟ∆ΟΥ 
    gen = pioa->ISR;
    // PIOA : ΕΚΚΑΘΑΡΙΣΗ ΑΠΟ ΤΥΧΟΝ ∆ΙΑΚΟΠΕΣ 
    pioa->PER = 0x03;
    // ΓΡΑΜΜΕΣ 0, 1 : ΓΕΝΙΚΟΥ ΣΚΟΠΟΥ
    gen = tc->Channel_0.SR;
    // TC0 : ΕΚΚΑΘΑΡΙΣΗ ΑΠΟ ΤΥΧΟΝ ∆ΙΑΚΟΠΕΣ 
    aic->ICCR = (1 << PIOA_ID) | (1 << TC0_ID);
    // AIC : ΕΚΚΑΘΑΡΙΣΗ ΑΠΟ ΤΥΧΟΝ ∆ΙΑΚΟΠΕΣ 
    pioa->IER = 0x01;

    while (getchar() != 'e' && game_over == 0)
    {
    }

    aic->IDCR = (1 << PIOA_ID) | (1 << TC0_ID);
    // ∆ΙΑΚΟΠΗ ΤΩΝ AIC interrupts
    tc->Channel_0.CCR = 0x02;
    // ΑΠΕΝΕΡΓΟΠΟΙΗΣΗ ΤΟΥ Timer CLEANUP;
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
        // ΕΚΚΑΘΑΡΙΣΗ ΤΗΣ ∆ΙΑΚΟΠΗΣ ΑΠΟ AIC 
        data_in = pioa->PDSR;
        // ΑΝΑΓΝΩΣΗ ΤΙΜΩΝ ΕΙΣΟ∆ΟΥ 

        // --- PLAYER 1 CONTROLS

        /* p1_a */
        if (!(data_in & p1_a)) // because pull up resistor
        {
            // ∆ΙΑΚΟΠΤΗΣ ΠΑΤΗΜΕΝΟΣ
            if (p1_a_state == BUT_IDLE)
            {
                p1_a_state = BUT_PRESSED;
                
                if (ball_position == BALL_START) {
                    ball_speed = INITIAL_BALL_SPEED;
                }
            }
        }
        else
        {
            if (p1_a_state == BUT_PRESSED){
                p1_a_state = BUT_IDLE;
            }
        }

        /* p1_b */
        if (!(data_in & p1_b)) // because pull up resistor
        {
            // ∆ΙΑΚΟΠΤΗΣ ΠΑΤΗΜΕΝΟΣ
            if (p1_b_state == BUT_IDLE)
            {
                p1_b_state = BUT_PRESSED;
                
                if (ball_position == BALL_START) {
                    ball_speed = -ball_speed * 5;
                }
            }
        }
        else
        {
            if (p1_b_state == BUT_PRESSED){
                p1_b_state = BUT_IDLE;
            }
        }

        // --- PLAYER 2 CONTROLS

        /* p2_a */
        if (!(data_in & p2_a)) // because pull up resistor
        {
            // ∆ΙΑΚΟΠΤΗΣ ΠΑΤΗΜΕΝΟΣ
            if (p2_a_state == BUT_IDLE)
            {
                p2_a_state = BUT_PRESSED;
                
                if (ball_position == BALL_END) {
                    ball_speed = -INITIAL_BALL_SPEED;
                }
            }
        }
        else
        {
            if (p2_a_state == BUT_PRESSED){
                p2_a_state = BUT_IDLE;
            }
        }

        /* p2_b */
        if (!(data_in & p2_b)) // because pull up resistor
        {
            // ∆ΙΑΚΟΠΤΗΣ ΠΑΤΗΜΕΝΟΣ
            if (p2_b_state == BUT_IDLE)
            {
                p2_b_state = BUT_PRESSED;
                
                if (ball_position == BALL_END) {
                    ball_speed = -ball_speed * 5;
                }
            }
        }
        else
        {
            if (p2_b_state == BUT_PRESSED){
                p2_b_state = BUT_IDLE;
            }
        }
    }

    if (fiq & (1 << TC0_ID))
    {
        data_out = tc->Channel_0.SR;
        // ΕΚΚΑΘΑΡΙΣΗ ΤΗΣ ΠΗΓΗΣ ΤΗΣ ∆ΙΑΚΟΠΗΣ 
        aic->ICCR = (1 << TC0_ID);
        // ΕΚΚΑΘΑΡΙΣΗ ∆ΙΑΚΟΠΗΣ ΚΑΙ ΑΠΟ AIC 
        data_out = pioa->ODSR;
        // ΑΝΑΓΝΩΣΗ ΤΙΜΩΝ ΕΞΟ∆ΟΥ 

        // timer interrupt every 1/5 of a second    
        ticks++; // dont care about overflow we only use modulo

        if (blink_ball == 0){ // if goal has not happened and we are not blinking in place
            if (ticks % ball_speed == 0 || ticks % -ball_speed == 0) {
                if (ball_speed > 0) {
                    ball_position++;
                }

                if (ball_speed < 0) {
                    ball_position--;
                }
            }
        }

        if (blink_ball == 1) {
            //blink ball_position led in place for 6 times

            unsigned int target_led = 1 << ball_position;
            // flip
            pioa->CODR = data_out & target_led;
            pioa->SODR = data_out | target_led;

            blink_ball_count++;
        }

        if (blink_ball_count == 6) {
            blink_ball = 0;
        }

        if (ball_position == BALL_END + 1) {
            // p1 score goal
            p1_score++;
            
            ball_speed = -INITIAL_BALL_SPEED;
            blink_ball = 1;
        }

        if (ball_position == BALL_START - 1){
            // p2 score goal
            p2_score++;

            ball_speed = INITIAL_BALL_SPEED;
            blink_ball = 1;
        }

        if (blink_ball == 0){
            // turn off all leds
            pioa->CODR = data_out & leds;
            // turn on correct one
            pioa->SODR = data_out | (1 << ball_position);
        }


        // -- Game over checks
        if (p1_score == p2_score + 2) {
            printf("p1 win");
            game_over = 1;
        }

        if (p2_score == p1_score + 2) {
            printf("p2 win");
            game_over = 1;
        }

        tc->Channel_0.CCR = 0x05;
    }
}