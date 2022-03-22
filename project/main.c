#include <msp430.h>
#include "libTimer.h"
#include "input.h"
#include "output.h"
#include "toy.h"

void wdt_init(void)
{
  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
}

void main(void) 
{  
  buzzer_init();
  switch_init();
  led_init();
  wdt_init();
  
  toy_reset();
  
  or_sr(0x18);  // CPU off, GIE on
} 

void __interrupt_vec(PORT2_VECTOR) Port_2()
{
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

void __interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  buzzer_timer_interrupt();
  led_timer_interrupt();
} 
