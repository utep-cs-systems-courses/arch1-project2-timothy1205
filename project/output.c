#include <msp430.h>
#include "libTimer.h"
#include "output.h"

void buzzer_init(void)
{
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR = BIT6;		/* enable output to speaker (P2.6) */


}

void buzzer_set_period(short cycles)
{
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}

static LedState led_green = LED_OFF;
static LedState led_red = LED_OFF;
static unsigned char led_counter = 0;
static unsigned char led_blink_on = 1;

void led_init(void)
{
  led_green = LED_OFF;
  led_red = LED_OFF;
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;		/* leds initially off */
}

void led_green_on(void)
{
  led_green = LED_ON;
  P1OUT |= LED_GREEN;
}

void led_green_off(void)
{
  led_green = LED_OFF;
  P1OUT &= ~LED_GREEN;
}

void led_green_blink(void)
{
  if (led_green == LED_BLINK)
    led_red_off();

  led_green = LED_BLINK;
  led_counter = 0;
  led_blink_on = 1;
  P1OUT |= LED_GREEN;
}

void led_red_on(void)
{
  led_red = LED_ON;
  P1OUT |= LED_GREEN;
}

void led_red_off(void)
{
  led_red = LED_OFF;
  P1OUT &= ~LED_RED;
}

void led_void_blink(void)
{
  if (led_green == LED_BLINK)
    led_green_off();

  led_red = LED_BLINK;
  led_counter = 0;
  led_blink_on = 1;
  P1OUT |= LED_RED;
}

inline static void led_blink_toggle(void)
{
  led_counter = 0;
  led_blink_on = !led_blink_on;
  P1OUT ^= (led_green == LED_BLINK ? LED_GREEN : LED_RED);
}

void led_timer_interrupt(void)
{
  // If no LEDS are blinking exit early
  if (led_green != LED_BLINK && LED_RED != LED_BLINK)
    return;

  // At most only one LED will ever be blinking
  if (led_blink_on && led_counter++ >= LED_ON_INTERRUPTS)
    led_blink_toggle();
  else if (!led_blink_on && led_counter++ >= LED_OFF_INTERRUPTS)
    led_blink_toggle();
}
