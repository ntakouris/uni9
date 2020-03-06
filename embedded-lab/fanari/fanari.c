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

#define STATE_RED 0
#define STATE_ORANGE 1
#define STATE_GREEN 2

unsigned int fanari = STATE_GREEN;

void FIQ_handler(void);
PIO *pioa = NULL;
AIC *aic = NULL;
TC *tc = NULL;

unsigned int button_state = BUT_IDLE;

unsigned int ticks = 0;

unsigned int led_green = 0x0;
unsigned int led_orange = 0x1;
unsigned int led_red = 0x0;

unsigned int btn = 0x3;


int main(int argc, const char *argv[])
{
    unsigned int gen;
    STARTUP;
    // ΑΡΧΙΚΟΠΟΙΗΣΗ ΣΥΣΤΗΜΑΤΟΣ 
    tc->Channel_0.RC = 8192 ; // interrupt every 1s
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
    pioa->CODR = led_orange | led_red;
    pioa->SODR = led_green;
    // ΓΡΑΜΜΗ 1 : ∆ΥΝΑΜΙΚΟ ΕΞΟ∆ΟΥ LOW 
    pioa->OER = led_green | led_orange | led_red;
    // ΓΡΑΜΜΗ 1 : ΛΕΙΤΟΥΡΓΙΑ ΕΞΟ∆ΟΥ 
    gen = pioa->ISR;
    // PIOA : ΕΚΚΑΘΑΡΙΣΗ ΑΠΟ ΤΥΧΟΝ ∆ΙΑΚΟΠΕΣ 
    pioa->PER = led0 | led1 | led2 | btn;
    gen = tc->Channel_0.SR;
    // TC0 : ΕΚΚΑΘΑΡΙΣΗ ΑΠΟ ΤΥΧΟΝ ∆ΙΑΚΟΠΕΣ 
    aic->ICCR = (1 << PIOA_ID) | (1 << TC0_ID);
    // AIC : ΕΚΚΑΘΑΡΙΣΗ ΑΠΟ ΤΥΧΟΝ ∆ΙΑΚΟΠΕΣ 
    pioa->IER = btn;

    while (getchar() != 'e')
    {
        
    }

    aic->IDCR = (1 << PIOA_ID) | (1 << TC0_ID);
    // ∆ΙΑΚΟΠΗ ΤΩΝ AIC interrupts
    tc->Channel_0.CCR = 0x02;
    // ΑΠΕΝΕΡΓΟΠΟΙΗΣΗ ΤΟΥ Timer CLEANUP;
    return 0;
}

unsigned int walk_request = 0;
unsigned int red_ticks = 0;
unsigned int red_scheduled = 1;

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
                
                // diavasi
                walk_request = 1;
            }
        }
        else
        {
            if (button_state == BUT_PRESSED){
                button_state = BUT_IDLE;
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
        ticks++;

        if (fanari == STATE_GREEN && walk_request == 1) {
            fanari = STATE_ORANGE;
            
            pioa->CODR = led_green;
        } else if (fanari == STATE_ORANGE && walk_request == 1) {
            // blink orange 
            walk_request = 0;

            pioa->CODR = led_green;
            pioa->SODR = led_orange;
        } else if (fanari == STATE_ORANGE && walk_request == 0) {
            // red
            fanari == STATE_RED;
        }else if (fanari == STATE_RED) {
            if (red_scheduled == 1) {
                pioa->CODR = led_orange;
                pioa->SODR = led_red;
                red_scheduled = 0;
            }else{
                red_ticks++;

                if (red_ticks == 10) {  // reset to green
                    fanari = STATE_GREEN;
                    red_ticks = 0;
                    ticks = 0;

                    walk_request = 0;
                    red_scheduled = 0;

                    pioa->CODR = led_red;
                    pioa->SODR = led_green;
                }
            }
        }
        
        // re-count from initial config
        tc->Channel_0.CCR = 0x05;
    }
}