#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <header.h>
#include <time.h>

#define PIOA_ID 2
#define TC0_ID 17
#define BUT_IDLE 0

#define BUT_PRESSED 1
#define BUT_RELEASED 2
#define LED_IDLE 0
#define LED_FLASHING 1

#define STATE_IDLE 0
#define STATE_HOLD 1

unsigned int clock_state = STATE_IDLE;

void FIQ_handler(void);
PIO *pioa = NULL;
AIC *aic = NULL;
TC *tc = NULL;
unsigned int button_state = BUT_IDLE;
unsigned int led_state = LED_IDLE;

unsigned int ticks = 0;

unsigned int led1 = 0x0;
unsigned int led2 = 0x1;
unsigned int led0 = 0x0;

unsigned int btn = 0x3;

unsigned int display_clk = 1, display_idle = 0;
unsigned int hold_ticks = 0;
unsigned int clock_rst = 0;

int main(int argc, const char *argv[])
{
    unsigned int gen;
    STARTUP;
    // ΑΡΧΙΚΟΠΟΙΗΣΗ ΣΥΣΤΗΜΑΤΟΣ 
    tc->Channel_0.RC = 8192 / 2; // interrupt every 0.5s
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
    pioa->PUER = btn;
    // ΕΝΕΡΓΟΠΟΙΗΣΗ ΣΤΗ ΓΡΑΜΜΗ 0 : PULL−UP 
    pioa->ODR = btn;
    // ΓΡΑΜΜΗ 0 : ΛΕΙΤΟΥΡΓΙΑ ΕΙΣΟ∆ΟΥ 
    pioa->CODR = led0 | led1 | led2;
    // ΓΡΑΜΜΗ 1 : ∆ΥΝΑΜΙΚΟ ΕΞΟ∆ΟΥ LOW 
    pioa->OER = led0 | led1 | led2;
    // ΓΡΑΜΜΗ 1 : ΛΕΙΤΟΥΡΓΙΑ ΕΞΟ∆ΟΥ 
    gen = pioa->ISR;
    // PIOA : ΕΚΚΑΘΑΡΙΣΗ ΑΠΟ ΤΥΧΟΝ ∆ΙΑΚΟΠΕΣ 
    pioa->PER = led0 | led1 | led2 | btn;
    gen = tc->Channel_0.SR;
    // TC0 : ΕΚΚΑΘΑΡΙΣΗ ΑΠΟ ΤΥΧΟΝ ∆ΙΑΚΟΠΕΣ 
    aic->ICCR = (1 << PIOA_ID) | (1 << TC0_ID);
    // AIC : ΕΚΚΑΘΑΡΙΣΗ ΑΠΟ ΤΥΧΟΝ ∆ΙΑΚΟΠΕΣ 
    pioa->IER = btn;
    unsigned int data_out;

    while (getchar() != 'e')
    {
        if (clock_rst == 1) {
            clock_rst = 0;
            pioa->CODR = led0 | led1 | led2;
        }

        // runs 2 times for each second
        if (display_clk == 1) {
            display_clk = 0;
            display_idle++;

            // flip led 1
            data_out = pioa->ODSR;
            pioa->SODR = data_out | led1;
            pioa->CODR = data_out & led1;
        }

        // runs each second
        if (display_clk == 2) {
            display_clk = 0;
            display_idle++;

            printf("%u s", ticks);

            int first_digit = ticks % 10;
            int second_digit = (ticks / 10) % 10;
            
            if (second_digit != 0) {
                int t = 10 / second_digit;

                if (first_digit % t == 0) {
                    pioa->SODR = data_out | led2;
                    pioa->CODR = data_out & led2;
                }
            }
        }

        // runs each 2 seconds
        if (display_idle == 2) {
            if (clock_state == STATE_HOLD) {
                // flip led0
                pioa->SODR = data_out | led0;
                pioa->CODR = data_out & led0;
            }
        }
    }

    aic->IDCR = (1 << PIOA_ID) | (1 << TC0_ID);
    // ∆ΙΑΚΟΠΗ ΤΩΝ AIC interrupts
    tc->Channel_0.CCR = 0x02;
    // ΑΠΕΝΕΡΓΟΠΟΙΗΣΗ ΤΟΥ Timer CLEANUP;
    return 0;
}

unsigned int press_time = 0;
unsigned int subtick = 0;

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
        if (!(data_in & btn)) // pull up resistor
        {
            // ∆ΙΑΚΟΠΤΗΣ ΠΑΤΗΜΕΝΟΣ
            if (button_state == BUT_IDLE)
            {
                button_state = BUT_PRESSED;
                press_time = time(NULL);
                // button down event
                
                if (clock_state == STATE_IDLE) {
                    clock_state = STATE_HOLD;
                    hold_ticks = ticks;

                    pioa->SODR = led0;
                } else if (clock_state == STATE_HOLD) {
                    clock_state = STATE_IDLE;

                    pioa->CODR = led0;
                }

            }
        }
        else
        {
            if (button_state == BUT_PRESSED){
                button_state = BUT_IDLE;

                if (time(NULL) - press_time > 1) {
                    clock_state = STATE_IDLE;
                    ticks = 0;
                    clock_rst = 1;
                }
            }
        }
    }

    // timer interrupt
    if (fiq & (1 << TC0_ID))
    {
        data_out = tc->Channel_0.SR;
        // ΕΚΚΑΘΑΡΙΣΗ ΤΗΣ ΠΗΓΗΣ ΤΗΣ ∆ΙΑΚΟΠΗΣ 
        aic->ICCR = (1 << TC0_ID);
        // ΕΚΚΑΘΑΡΙΣΗ ∆ΙΑΚΟΠΗΣ ΚΑΙ ΑΠΟ AIC 
        data_out = pioa->ODSR;
        // ΑΝΑΓΝΩΣΗ ΤΙΜΩΝ ΕΞΟ∆ΟΥ 
        subtick++;

        if (subtick < 2) {
            subtick++;
            if (clock_state == STATE_IDLE) {
                display_clk = 1;
            }
            display_clk = 1;
            return;
        }
        // always increment clock
        ticks++;

        if (ticks > 59) {
            ticks = 0;
            clock_rst = 1;
        }
        
        // re-count from initial config
        tc->Channel_0.CCR = 0x05;
    }
}