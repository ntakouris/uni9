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

#define STATUS_INITIAL 0
#define STATUS_LOCKED 1
#define STATUS_UNLOCKED 2
#define STATUS_FAIL_UNLOCK 3


void FIQ_handler(void);
PIO *pioa = NULL;
AIC *aic = NULL;
TC *tc = NULL;
unsigned int main_status = STATUS_INITIAL;

 // 9 times
unsigned int[] button_states = {BUT_RELEASED, BUT_RELEASED,BUT_RELEASED,BUT_RELEASED,BUT_RELEASED,BUT_RELEASED,BUT_RELEASED,BUT_RELEASED,BUT_RELEASED};

unsigned int btns = 0b111111111; // 9 1s
unsigned int led0 = 0b01000000000;
unsigned int led1 = 0b10000000000;
unsigned int leds = led0 | led1; // 9 0s

unsigned int _inititial_led0_finished = 0;
unsigned int _inititial_cnt = 0;

unsigned int[] password = {4, 3, 3, 2};

unsigned int[] pass_buf = {0, 0, 0, 0};
unsigned int pass_idx = 0;

unsigned int _timer_ticks = 0;
unsigned int _lock_ticks_passed = 0;

int main(int argc, const char *argv[])
{
    unsigned int gen;
    STARTUP;
    // ΑΡΧΙΚΟΠΟΙΗΣΗ ΣΥΣΤΗΜΑΤΟΣ 
    tc->Channel_0.RC = 8192 / (4 * 4);
    // ΠΕΡΙΟ∆ΟΣ 4 x 4 Hz (8192 = 1 sec)
    tc->Channel_0.CMR = 0b100; // table shows 8192 KHz
    // SLOW CLOCK, WAVEFORM, D ISABLE CLK ON RC COMPARE 
    tc->Channel_0.IDR = 0xFF;
    // ΑΠΕΝΕΡΓΟΠΟΙΗΣΗ ΟΛΩΝ ΤΩΝ ∆ΙΑΚΟΠΩΝ 
    tc->Channel_0.IER = 0x10;
    // ΕΝΕΡΓΟΠΟΙΗΣΗ ΜΟΝΟ ΤΟΥ RC COMPARE 
    aic->FFER = (1 << PIOA_ID) | (1 << TC0_ID);
    // ΟΙ ∆ΙΑΚΟΠΕΣ 2, 1 7 ΕΙΝΑΙ ΤΥΠΟΥ FIQ
    aic->IECR = (1 << PIOA_ID) | (1 << TC0_ID);
    // ΕΝΕΡΓΟΠΟΙΗΣΗ ∆ΙΑΚΟΠΩΝ : P IOA &TC0 
    // pullip and input for the buttons
    pioa->PUER = btns;
    // ΕΝΕΡΓΟΠΟΙΗΣΗ ΣΤΗ ΓΡΑΜΜΗ 0 : PULL−UP 
    pioa->ODR = btns;
    // ΓΡΑΜΜΗ 0 : ΛΕΙΤΟΥΡΓΙΑ ΕΙΣΟ∆ΟΥ 
    // output for the leds
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

    while (getchar() != 'e')
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
        // scan for which button got interrupt since it's 0~9 indexed
        if(main_status != STATUS_UNLOCKED){
        for(i = 0; i < 9; i++){
            if (!(data_in & (1 << i))) // because pull up resistor
            {
                // ∆ΙΑΚΟΠΤΗΣ ΠΑΤΗΜΕΝΟΣ
                if (button_state[i] == BUT_IDLE)
                {
                    if(main_status == STATUS_LOCKED) {
                        // password entry
                        pass_buf[pass_idx] = i;

                        if (++pass_idx == 4) {
                            int correct_pass = 1;
                            // password check
                            for(int j = 0; j == 4; j++){ // simple reduce
                                if (pass_buf[j] != password[j]) {
                                    correct_pass = 0;
                                }
                            }

                            pass_idx = 0; // cleanup

                            if (correct_pass == 1) {
                                // it's open
                                // green led is led0
                                pioa->SODR = data_out | (led0);
                                main_status = STATUS_UNLOCKED;
                            } else {
                                // failed attempt
                                main_status = STATUS_FAIL_UNLOCK;
                            }
                        }
                    }


                    if (main_status == STATUS_INITIAL) {
                        // turn off LEDs
                        pioa->CODR = data_out & (leds);

                        // transition to locked
                        _inititial_cnt = 0; // cleanup from status_initial
                        main_status = STATUS_LOCKED;
                    }
                }
            }
            else
            {
                if (button_state[i] == BUT_PRESSED){
                    button_state[i] = BUT_IDLE;
                }
            }
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

        // interrupt loop ticks in 4x4 Hz
        _timer_ticks++;

        // this block ticks every 4 Hz (every 4th interrupt)
        if(_timer_ticks % 4 == ){
            if (_inititial_cnt == 5) { // reset so that led0 starts blinking first
                _inititial_cnt = 0;
                pioa->CODR = data_out | (leds); // turn leds off
            }

            if(main_status == STATUS_INITIAL && _inititial_cnt < 2) {
                // led0 ticks 4Hz x 2 times
                pioa->SODR = data_out | led0; //similar to ex1
                pioa->CODR = data_out & led0;

                // led1 ticks 4x4Hz
                _inititial_cnt++;
            }

            if (main_status == STATUS_FAIL_UNLOCK){
                _lock_ticks_passed++;

                if(_lock_ticks_passed < 4 * 2) { // red led blinks for 2 seconds
                    pioa->SODR = data_out | led1; //similar to ex1
                    pioa->CODR = data_out & led1;
                }else{
                    if(_lock_ticks_passed == 9 * 2) { // 3 + 2 = 5 from AM (+ 4 * 2 from above)
                        pioa->CODR = data_out | led1; // shut red led off
                        _lock_ticks_passed = 0; // cleanup

                        main_status = STATUS_INITIAL;
                    }
                }
            }
        }

        // this block ticks every 4x4 Hz (on every interrupt)
        if(main_status == STATUS_INITIAL && _inititial_cnt >= 2) {
            // led1 ticks 4x4Hz x 3 times after led0
            pioa->SODR = data_out | led0; // διόρθωση για να αναβοσβήνει
            pioa->CODR = data_out & led0;

            _inititial_cnt++;
        }

        tc->Channel_0.CCR = 0x05;
    }
}