#include <msp430.h>
#include "libTimer.h"

#define LED_RED BIT6               // P1.6
#define LED_GREEN BIT0             // P1.0
#define LEDS (LED_RED | LED_GREEN)

#define SW1 BIT3		/* switch1 is p1.3 */
#define SWITCHES SW1		/* only 1 switch on this board */
#define BLINK_FAST 32
#define BLINK_SLOW 128

void switch_init() {
  P1REN |= SWITCHES;		/* enables resistors for switches */
  P1IE |= SWITCHES;		/* enable interrupts from switches */
  P1OUT |= SWITCHES;		/* pull-ups for switches */
  P1DIR &= ~SWITCHES;		/* set switches' bits for input */
}

void led_init() {
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;		/* leds initially off */
}

void wdt_init() {
  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
}

void main(void) 
{  
  switch_init();
  led_init();
  wdt_init();
    
  or_sr(0x18);  // CPU off, GIE on
} 

static char green_on = 0;
static enum State{OFF, RED_FAST_GREEN_SLOW, GREEN_FAST_RED_SLOW, SLOW_SWITCH, BOTH_ON} state = OFF; // Default to off

static void next_state(void)
{
  state = (state + 1) % 5;
}

void
switch_interrupt_handler()
{
  char p1val = P1IN;		/* switch is in P1 */

/* update switch interrupt sense to detect changes from current buttons */
  P1IES |= (p1val & SWITCHES);	/* if switch up, sense down */
  P1IES &= (p1val | ~SWITCHES);	/* if switch down, sense up */

  if (!(p1val & SW1)) {		/* button down */
    next_state();
  }
}


/* Switch on P1 (S2) */
void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if (P1IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P1IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

static void trigger_led(int blink_var, char led)
{
  if (!blink_var)
    P1OUT |= led;
  else
    P1OUT &= ~led;
}

static inline void trigger_led_with_input(char input, int blink_var, char led)
{
  if (!blink_var && input)
    P1OUT |= led;
  else
    P1OUT &= ~led;
}


void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  static int blink_count_fast = 0;
  static int blink_count_slow = 0;

  if (++blink_count_fast >= BLINK_FAST) {
    blink_count_fast = 0;
  }

  if (++blink_count_slow >= BLINK_SLOW) {
    blink_count_slow = 0;
    green_on = !green_on;
  }

  switch (state) {
    case OFF:
      P1OUT &= ~LEDS;
      break;
    case RED_FAST_GREEN_SLOW:
      trigger_led(blink_count_fast, LED_RED);
      trigger_led(blink_count_slow, LED_GREEN);
      break;
    case GREEN_FAST_RED_SLOW:
      trigger_led(blink_count_fast, LED_GREEN);
      trigger_led(blink_count_slow, LED_RED);
      break;
    case SLOW_SWITCH:
      trigger_led_with_input(green_on, blink_count_slow, LED_GREEN);
      trigger_led_with_input(!green_on, blink_count_slow, LED_RED);
      break;
    case BOTH_ON:
      P1OUT |= LEDS;
      break;
  }
} 
