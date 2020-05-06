//AT91 emulation code by Theodoros Simopoulos
#define HIGH 0x1
#define LOW 0x0

//AT91 special
#define BUT_IDLE 1
#define BUT_PRESSED 0
#define BUT_RELEASED 2
#define LED_IDLE 0
#define LED_FLASHING 1

#define BALL_START 2
#define BALL_END 11
#define INITIAL_BALL_SPEED 25

//Arduino special
#define continue_count 0x01
#define stop_count 0x02
#define reset_and_start_count 0x05

unsigned int PIOA_int, TC_int, TC_src; //ομοίως int με πιθανές τιμές 0|1
unsigned long clk_pulse_counter;
unsigned int Channel_0_CCR;
unsigned int previous_button_state_p1_a = BUT_IDLE;
unsigned int previous_button_state_p2_a = BUT_IDLE;
unsigned int previous_button_state_p1_b = BUT_IDLE;
unsigned int previous_button_state_p2_b = BUT_IDLE;
unsigned long Channel_0_RC;

//Μεταβλητές και σταθερές εργαστηριακής άσκησης
unsigned int p1_a_state = BUT_IDLE;
unsigned int p1_b_state = BUT_IDLE;

unsigned int p2_a_state = BUT_IDLE;
unsigned int p2_b_state = BUT_IDLE;

unsigned int p1_a = 14;
unsigned int p1_b = 15;

unsigned int p2_a = 16; // 12
unsigned int p2_b = 17; // 13

int button_state = BUT_IDLE;

int ball_speed = INITIAL_BALL_SPEED / 10 ; // in 1/5 Hz

int ball_position = BALL_START;

int p1_score = 0;
int p2_score = 0;

int game_over = 0;

int blink_ball = 0;
int blink_ball_count = 0;

unsigned int ticks = 0;

unsigned int temp = 0;

void setup()
{
    pinMode(14, INPUT_PULLUP); //ODR, PUER
    pinMode(15, INPUT_PULLUP);
    pinMode(2, OUTPUT); //OER
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(16, INPUT_PULLUP);
    pinMode(17, INPUT_PULLUP);
    clk_pulse_counter = 0;
    Channel_0_CCR = stop_count;
    Channel_0_RC = 8192 / 2;
    Channel_0_CCR = 0x05;
    PIOA_int = LOW;
    TC_int = LOW;
    Serial.begin(9600);
}

//main
void loop()
{

    //Ενεργοποίηση διακοπής από τον timer/counter
    if (Channel_0_CCR == reset_and_start_count)
    {
        clk_pulse_counter = 0;
        Channel_0_CCR = continue_count;
    }

    if (Channel_0_CCR == continue_count)
    {
        clk_pulse_counter = clk_pulse_counter + 1;
        if (clk_pulse_counter == Channel_0_RC)
        {
            clk_pulse_counter = 0;
            TC_int = HIGH;
        }
    }

    //Ενεργοποίηση διακοπής από την παράλληλη Α (PIOA)
    int button_state_p1_a = digitalRead(p1_a);
    if (button_state_p1_a != previous_button_state_p1_a)
    {
        previous_button_state_p1_a = button_state_p1_a;
        PIOA_int = HIGH;
        TC_src = p1_a;
    }
  
	int button_state_p1_b = digitalRead(p1_b);
    if (button_state_p1_b != previous_button_state_p1_b)
    {
        previous_button_state_p1_b = button_state_p1_b;
        PIOA_int = HIGH;
        TC_src = p1_b;
    }

  	int button_state_p2_a = digitalRead(p2_a);
    if (button_state_p2_a != previous_button_state_p2_a)
    {
        previous_button_state_p2_a = button_state_p2_a;
        PIOA_int = HIGH;
        TC_src = p2_a;
    }

    int button_state_p2_b = digitalRead(p2_b);
    if (button_state_p2_b != previous_button_state_p2_b)
    {
        previous_button_state_p2_b = button_state_p2_b;
        PIOA_int = HIGH;
        TC_src = p2_b;
    }

    if (PIOA_int | TC_int) //aic->ICCR
    {
        FIQ_handler();
    }
}

void FIQ_handler(void)
{
    //Καθαρισμός της μονάδας που προκάλεσε την διακοπή

    if (PIOA_int == HIGH) // Η διακοπή προκλήθηκε από την PIOA
    {
        PIOA_int = LOW;
        if (TC_src == p1_a) // because pull up resistor
        {
            // ∆ΙΑΚΟΠΤΗΣ ΠΑΤΗΜΕΝΟΣ
            if (p1_a_state == BUT_PRESSED)
            {
                Serial.println("p1_a");
                p1_a_state = BUT_IDLE;

                if (ball_position == BALL_START && blink_ball == 0)
                {
                    Serial.println("slow pong");
                    ball_speed = INITIAL_BALL_SPEED / 5;
                  	ball_position++ ;
                }
            }
            else
            {
                if (p1_a_state == BUT_IDLE)
                {
                    p1_a_state = BUT_PRESSED;
                }
            }
        }

        /* p1_b */
        if (TC_src == p1_b) // because pull up resistor
        {
            // ∆ΙΑΚΟΠΤΗΣ ΠΑΤΗΜΕΝΟΣ
            if (p1_b_state == BUT_PRESSED)
            {
                Serial.println("p1_b");
                p1_b_state = BUT_IDLE;

                if (ball_position == BALL_START && blink_ball == 0)
                {
                    Serial.println("fast_pong");
                    ball_speed = INITIAL_BALL_SPEED / 25;
                  	ball_position++;
                }
            }
            else
            {
                if (p1_b_state == BUT_IDLE)
                {
                    p1_b_state = BUT_PRESSED;
                }
            }
        }

        // --- PLAYER 2 CONTROLS

        /* p2_a */
        if (TC_src == p2_a) // because pull up resistor
        {
            // ∆ΙΑΚΟΠΤΗΣ ΠΑΤΗΜΕΝΟΣ
            if (p2_a_state == BUT_PRESSED)
            {
                Serial.println("p2_a");
                p2_a_state = BUT_IDLE;

                if (ball_position == BALL_END && blink_ball == 0)
                {
                    Serial.println("slow_pong");
                    ball_speed = -INITIAL_BALL_SPEED / 5;
                  	ball_position--;
                }
            }
            else
            {
                if (p2_a_state == BUT_IDLE)
                {
                    p2_a_state = BUT_PRESSED;
                }
            }
        }

        /* p2_b */
        if (TC_src == p2_b) // because pull up resistor
        {
            // ∆ΙΑΚΟΠΤΗΣ ΠΑΤΗΜΕΝΟΣ
            if (p2_b_state == BUT_PRESSED)
            {
                Serial.println("p2_b");
                p2_b_state = BUT_IDLE;

                if (ball_position == BALL_END && blink_ball == 0)
                {
                    Serial.println("fast_pong");
                    ball_speed = -INITIAL_BALL_SPEED / 25;
                  	ball_position--;
                }
            }
            else
            {
                if (p2_b_state == BUT_IDLE)
                {
                    p2_b_state = BUT_PRESSED;
                }
            }
        }
    }

    if (TC_int == HIGH) // Η διακοπή προκλήθηκε από τον timer/counter
    {
        TC_int = LOW;
        ticks++; // dont care about overflow we only use modulo

        if (blink_ball == 0)
        { // if goal has not happened and we are not blinking in place
            if (ticks % ball_speed == 0 || ticks % -ball_speed == 0)
            {
                if (ball_speed > 0)
                {
                    ball_position++;
                }

                if (ball_speed < 0)
                {
                    ball_position--;
                }
            }
        }

        if (blink_ball == 1)
        {
          if(ball_position == 1){
          	ball_position = 2; 
          }
          if(ball_position == 12){
          	ball_position = 11;
          }
            //blink ball_position led in place for 6 times
            temp = !digitalRead(ball_position);
            digitalWrite(ball_position, temp);

            blink_ball_count++;
        }

        Serial.println(ball_position);

        if (blink_ball_count == 6)
        {
            Serial.println("stop blinking in place");
            blink_ball = 0;
        }

        if (blink_ball == 0)
        {
            if (ball_position == BALL_END + 1)
            {
                Serial.println("p1 score");
                p1_score++;

                ball_speed = -INITIAL_BALL_SPEED / 25;
                blink_ball = 1;
            }

            if (ball_position == BALL_START - 1)
            {
                Serial.println("p2 score");
                p2_score++;

                ball_speed = INITIAL_BALL_SPEED / 25;
                blink_ball = 1;
            }
        }

        if (blink_ball == 0)
        {
            // turn off all leds
            PORTD = 0x00;
            PORTB = 0x00;
            // turn on correct one
            digitalWrite(ball_position, HIGH);
        }

        if (blink_ball == 1){
            // -- Game over checks
            if (p1_score >= p2_score + 2)
            {
                Serial.println("p1 win");
                game_over = 1;
            }

            if (p2_score >= p1_score + 2)
            {
                Serial.println("p2 win");
                game_over = 1;
            }
        }
    }
}