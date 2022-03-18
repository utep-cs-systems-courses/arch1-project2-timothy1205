#include <msp430.h>
#include "libTimer.h"
#include "input.h"
#include "toy.h"

void switch_interrupt_handler(void)
{
  char p2val = P2IN;		/* switch is in P1 */

/* update switch interrupt sense to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */

//  if (p2val & SWITCHES)		/* button up */
    //P1OUT |= BIT6;
    button_unpressed(p2val);
 // else			/* button down */
 // {
    button_pressed(p2val);
    //P1OUT |= BIT6;
 //   }
}

void switch_init(void) 
{
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
}
