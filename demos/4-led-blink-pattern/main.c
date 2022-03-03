//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);			/* CPU off, GIE on */
}

char green_on = 0xFF;
char red_on = 0;

void update_state()
{
  static enum State{GREEN_ON, RED_ON, BOTH_ON, BOTH_OFF} state = GREEN_ON; 
  state = (state + 1) % 4;

  switch (state) {
    case GREEN_ON:
      green_on = 0xFF;
      red_on = 0;
      break;
    case RED_ON:
      green_on = 0;
      red_on = 0xFF;
      break;
    case BOTH_ON:
      green_on = 0xFF;
      red_on = 0xFF;
      break;
    case BOTH_OFF:
      green_on = 0;
      red_on = 0;
      break;
  }
}

// global state vars that control blinking
int blinkLimit = 5;  // duty cycle = 1/blinkLimit
int blinkCount = 0;  // cycles 0...blinkLimit-1
int secondCount = 0; // state var representing repeating time 0…1s

void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  // handle blinking 
  blinkCount ++;
  if (blinkCount >= blinkLimit) { // on for 1 interrupt period
    blinkCount = 0;
    P1OUT |= (green_on & LED_GREEN);
    P1OUT |= (red_on & LED_RED);
  } else		          // off for blinkLimit - 1 interrupt periods
    P1OUT &= ~LEDS;

  // measure a second
  secondCount ++;
  if (secondCount >= 250) {  // once each second
    secondCount = 0;
    blinkLimit ++;	     // reduce duty cycle
    if (blinkLimit >= 8) {     // but don't let duty cycle go below 1/7.
      blinkLimit = 0;
      update_state();
    }
  }
} 

