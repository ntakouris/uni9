#define HIGH 0x1
#define LOW 0x0

//AT91 special
#define BUT_IDLE 0
#define BUT_PRESSED 1
#define BUT_RELEASED 2
#define LED_IDLE 0
#define LED_FLASHING 1

//Arduino special
#define continue_count 0x01
#define stop_count 0x02
#define reset_and_start_count 0x05

unsigned int PIOA_int; //ομοίως int με πιθανές τιμές 0|1
unsigned int previous_button_state;

//Μεταβλητές και σταθερές εργαστηριακής άσκησης
unsigned int button_state = BUT_IDLE;
unsigned int led_state = LED_IDLE;

void setup()
{
    // pinMode(0, INPUT_PULLUP); //ODR, PUER

    pinMode(1, OUTPUT);       //OER
    
    previous_button_state = BUT_PRESSED;
    PIOA_int = LOW;
}

#define BUT_IDLE 0

#define BUT_PRESSED 1
#define BUT_RELEASED 2

#define MODE_SEGMENT 0
#define MODE_LEDBAR 1
#define MODE MODE_LEDBAR

#define BRIGHTNESS_MOVING 0
#define BRIGHTNESS_IDLE 100

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1

unsigned int out = MODE == MODE_SEGMENT ? 0xFF : 0x3FF;
unsigned int button = 0x400; // button on pin 11 (1024d)
unsigned int brightness = BRIGHTNESS_IDLE;
unsigned int direction = DIRECTION_DOWN;

unsigned int button_state = BUT_IDLE;

unsigned int pwm = 100;
unsigned int gen;
void loop()
{
    // button_state = digitalRead(0);
    // if (button_state != previous_button_state)
    // {
    //     previous_button_state = button_state;
    //     PIOA_int = HIGH;
    // }

    // if (PIOA_int)
    // {
    //     FIQ_handler();
    // }

    if (brightness == BRIGHTNESS_MOVING)
    {
        if (direction == DIRECTION_DOWN)
        {
            pwm--;
        }
        else
        {
            pwm++;
        }
    }

    // clamp and direction flip
    if (pwm < 0)
    {
        pwm = 0;
        direction = DIRECTION_UP;
    }
    if (pwm > 100)
    {
        pwm = 100;
        direction = DIRECTION_DOWN;
    }

    printf("pwm = %u\n", pwm);

    // pwm modulation %
    for (int k = 0; k < 20; k++)
    {
        for (int i = 0; i < 100; i++)
        {
            if (i <= pwm)
            {
                // pioa->SODR = out;
            }
            else
            {
                // pioa->CODR = out;
            }
        }
    }
}
