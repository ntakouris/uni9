#include "contiki.h"
#include "os/dev/gpio-hal.h"
#include "os/dev/leds.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    /* Setup a periodic timer that expires after 10 seconds. */
    etimer_set(&timer, CLOCK_SECOND);
    leds_init();
    static int counter = 0;

    leds_num_t led0 = 0;
    leds_num_t led1 = 1;
    leds_num_t led2 = 2;

    while (1)
    {
        // this ticks every second
        print("counter: %d\n", counter);
        counter++;

        if (counter > 7)
        {
            counter = 0;
        }

        int bit0 = counter & 1;
        int bit1 = (counter >> 1) & 1;
        int bit2 = (counter >> 2) & 2;

        if (bit0 == 0)
        {
            leds_single_on(led0);
        }
        else
        {
            leds_single_off(led0);
        }

        if (bit1 == 0)
        {
            leds_single_on(led1);
        }
        else
        {
            leds_single_off(led1);
        }

        if (bit2 == 0)
        {
            leds_single_on(led2);
        }
        else
        {
            leds_single_off(led2);
        }

        /* Wait for the periodic timer to expire and then restart the timer. */
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/